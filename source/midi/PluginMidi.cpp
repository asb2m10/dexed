#include "PluginProcessor.h"

void DexedAudioProcessor::processMidiMessages(MidiBufferIterator &cur, MidiBufferIterator &end, int samplePos) {
    for(;cur != end;++cur) {
        juce::MidiMessageMetadata meta = *cur;
        if ( samplePos > meta.samplePosition )
            return;
        const MidiMessage message = meta.getMessage();
        processMidiMessage(&message);
    }
}

void DexedAudioProcessor::processMidiMessage(const MidiMessage *msg) {
    if ( msg->isSysEx() ) {
        handleIncomingMidiMessage(NULL, *msg);
        return;
    }

    const uint8 *buf  = msg->getRawData();
    uint8_t cmd = buf[0];
    uint8_t cf0 = cmd & 0xf0;
    auto channel = msg->getChannel();


    if( controllers.mpeEnabled && channel != 1 &&
        (
            (cf0 == 0xb0 && buf[1] == 74 ) || //timbre
            (cf0 == 0xd0 ) || // aftertouch
            (cf0 == 0xe0 ) // pb
            )
        )
    {
        // OK so find my voice index
        int voiceIndex = -1;
        for( int i=0; i<MAX_ACTIVE_NOTES; ++i )
        {
            if( voices[i].keydown && voices[i].channel == channel )
            {
                voiceIndex = i;
                break;
            }
        }
        if( voiceIndex >= 0 )
        {
            int i = voiceIndex;
            switch(cf0) {
            // THIS IS COMMENTED SINCE mepTimbre and mpePressure is not used
            // case 0xb0:
            //     voices[i].mpeTimbre = (int)buf[2];
            //     voices[i].dx7_note->mpeTimbre = (int)buf[2];
            //     break;
            // case 0xd0:
            //     voices[i].mpePressure = (int)buf[1];
            //     voices[i].dx7_note->mpePressure = (int)buf[1];
            //     break;
            case 0xe0:
                voices[i].mpePitchBend = (int)( buf[1] | (buf[2] << 7) );
                voices[i].dx7_note->mpePitchBend = (int)( buf[1] | ( buf[2] << 7 ) );
                break;
            }
        }
    }
    else
    {
        switch(cmd & 0xf0) {
        case 0x80 :
            keyup(channel, buf[1], buf[2]);
        return;

        case 0x90 :
            if (!synthTuningState->is_standard_tuning() || !buf[2] ||
                !MTS_HasMaster(mtsClient) || !MTS_ShouldFilterNote(mtsClient, buf[1], channel - 1))
                keydown(channel, buf[1], buf[2]);
        return;

        case 0xb0 : {
            int ctrl = buf[1];
            int value = buf[2];
                switch(ctrl) {
                case 1:
                    controllers.modwheel_cc = value;
                    controllers.refresh();
                    break;
                case 2:
                    controllers.breath_cc = value;
                    controllers.refresh();
                    break;
                case 4:
                    controllers.foot_cc = value;
                    controllers.refresh();
                    break;
                case 5:
                    controllers.portamento_cc = value;
                    break;
                case 64:
                    sustain = value > 63;
                    if (!sustain) {
                        for (int note = 0; note < MAX_ACTIVE_NOTES; note++) {
                            if (voices[note].sustained && !voices[note].keydown) {
                                voices[note].dx7_note->keyup();
                                voices[note].sustained = false;
                            }
                        }
                    }
                    break;
                case 65:
                    controllers.portamento_enable_cc = value >= 64;
                    break;
                case 120:
                    panic();
                    break;
                case 123:
                    for (int note = 0; note < MAX_ACTIVE_NOTES; note++) {
                        if (voices[note].keydown)
                            keyup(channel, voices[note].midi_note, 0);
                    }
                    break;
                default:
                    TRACE("handle channel %d CC %d = %d", channel, ctrl, value);
                    int channel_cc = (channel << 8) | ctrl;
                    if ( mappedMidiCC.contains(channel_cc) ) {
                        Ctrl *linkedCtrl = mappedMidiCC[channel_cc];

                        // We are not publishing this in the DSP thread, moving that in the
                        // event thread
                        //linkedCtrl->publishValueAsync((float) value / 127);
                    }
                    // this is used to notify the dialog that a CC value was received.
                    lastCCUsed.setValue(channel_cc);
                }
            }
            return;

        case 0xc0 :
            setCurrentProgram(buf[1]);
            return;

        case 0xd0 :
            controllers.aftertouch_cc = buf[1];
            controllers.refresh();
            return;

		case 0xe0 :
			controllers.values_[kControllerPitch] = buf[1] | (buf[2] << 7);
            return;
        }
    }
}

void DexedAudioProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) {
    if ( message.isActiveSense() )
        return;

#ifdef IMPLEMENT_MidiMonitor
    sysexComm.inActivity = true; // indicate to MidiMonitor that a MIDI messages (other than Active Sense) is received
#endif //IMPLEMENT_MidiMonitor

    const uint8 *buf = message.getRawData();
    int sz = message.getRawDataSize();

    //TRACE("%X %X %X %X %X %X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);

    if ( ! message.isSysEx() )
        return;

    // test if it is a Yamaha Sysex
    if ( buf[1] != 0x43 ) {
        TRACE("not a yamaha sysex %d", buf[1]);
        return;
    }

    int substatus = buf[2] >> 4;
    switch(substatus) {
        case 0 : {
            // single voice dump
            if ( buf[3] == 0 ) {
                if ( sz < 156 ) {
                    TRACE("wrong single voice datasize %d", sz);
                    return;
                }
                if ( updateProgramFromSysex(buf+6) )
                    TRACE("bad checksum when updating program from sysex message");
            }

            // 32 voice dump
            if ( buf[3] == 9 ) {
                if ( sz < 4104 ) {
                    TRACE("wrong 32 voice dump data size %d", sz);
                    return;
                }

                Cartridge received;
                if ( received.load(buf, sz) == 0 ) {
                    loadCartridge(received);
                    setCurrentProgram(0);
                }
            }
        }
        break;
        case 1 : {
            // parameter change
            if ( sz < 7 ) {
            TRACE("wrong single voice datasize %d", sz);
            return;
            }

            uint8 offset = (buf[3] << 7) + buf[4];
            uint8 value = buf[5];

            TRACE("parameter change message offset:%d value:%d", offset, value);

            if ( offset > 155 ) {
                TRACE("wrong offset size");
                return;
            }

            setDxValue(offset, value);
        }
        break;
        case 2: {
            if ( buf[3] == 0 ) {
                // single voice request
                sendCurrentSysexProgram();
            } else if ( buf[3] == 9 ) {
                // cart request
                sendCurrentSysexCartridge();
            } else {
                TRACE("Unknown voice request: %d", buf[3]);
            }
        }
        return;

        default: {
            TRACE("unknown sysex substatus: %d", substatus);
        }
        return;
    }

    /** TODO: reset parameters */
}
