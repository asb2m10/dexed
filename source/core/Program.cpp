#include "Program.h"
#include "Dexed.h"

Program::Program(const juce::MidiMessage message) {

}

void Program::setName(const juce::String &name) {
    for (int i=0; i < 10; i++) {
        char c = (char) name[i];
        if ( c == 0 ) {
            // Fill remaining positions with spaces
            for(int j = i; j < 10; j++) {
                data[145+j] = ' ';
            }
            break;
        }
        c = c < 32 ? ' ' : c;
        c = c > 127 ? ' ' : c;
        data[145+i] = c;
    }
}

void Program::pushToParameters(const DexedApvts &apvts) const {
    //juce::ScopedValueSetter<bool> setter(apvts.pushToParameterInProgress, true);

    uint8_t check[PROGRAM_SIZE];
    memcpy(check, data, PROGRAM_SIZE);

    for (const auto &param : dxParameters) {
        juce::RangedAudioParameter *parameter = apvts.getParameter(param.name);
        if ( parameter != nullptr ) {
            uint8_t value = data[param.pos];
            float displayValue = static_cast<float>(value - param.displayOffset);
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(parameter->convertTo0to1(displayValue));
            parameter->endChangeGesture();
        }
    }

    for (int i=0;i<6;i++) {
        juce::RangedAudioParameter *parameter = apvts.getParameter( IDs::on.op(i).name);
        if ( parameter != nullptr ) {
            bool opOn =  data[155] & (1 << (5-i));
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(parameter->convertTo0to1(opOn));
            parameter->endChangeGesture();
        }
    }

    for (const auto &param : dxParameters) {
        if ( check[param.pos] != data[param.pos] ) {
            TRACE("Warning : parameter(%s[%d]) %d -> %d changed during pushToParameters()", param.name.toRawUTF8(), param.pos, check[param.pos], data[param.pos]);
        }
    }
}

juce::MidiMessage Program::toMidiMessage(int targetChannel) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x00, 0x01, 0x1B };
    uint8_t dest[163];

    header[2] = (targetChannel & 0x0F);
    memcpy(dest, header, 6);
    memcpy(dest+6, data, 155);

    uint8_t footer[] = { sysexChecksum(data, 155), 0xF7 };

    memcpy(dest+161, footer, 2);
    return {dest, 163};
}

juce::String Program::normalizePgmName(const char *sysexName) {
    char buffer[11];

    memcpy(buffer, sysexName, 10);

    for (int j = 0; j < 10; j++) {
        char c = (unsigned char) buffer[j];
        c &= 0x7F; // strip don't care most-significant bit from name
        switch (c) {
            case 92:
                c = 'Y';
                break; /* yen */
            case 126:
                c = '>';
                break; /* >> */
            case 127:
                c = '<';
                break; /* << */
            default:
                if (c < 32 || c > 127)
                    c = 32;
                break;
        }
        buffer[j] = c;
    }
    buffer[10] = 0;

    return {buffer};
}

Program Program::initialProgram() {
    Program initialProgram;

    const char init_voice[] =
    { 99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
      99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
      73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };

    for(int i=0;i<sizeof(init_voice);i++) {
        initialProgram.data[i] = init_voice[i];
    }

    return initialProgram;
}

uint8_t sysexChecksum(const uint8_t *sysex, int size) {
    int sum = 0;
    int i;

    for (i = 0; i < size; sum -= sysex[i++]);
    return sum & 0x7F;
}
