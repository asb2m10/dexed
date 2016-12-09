// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include "dexed.peg"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"
#include "msfa/freqlut.h"
#include "msfa/controllers.h"
#include "PluginFx.h"
#include <unistd.h>

Dexed::Dexed(double rate) : lvtk::Synth<DexedVoice, Dexed>(p_n_ports, p_midi_in)
{
  TRACE("Hi");

  add_voices(new DexedVoice(rate));
  //add_voices(new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate));

  add_audio_outputs(p_audio_out);

  init(rate);

  TRACE("Bye");
}

Dexed::~Dexed()
{
  TRACE("Hi");

  delete [] outbuf_;

  currentNote = -1;

  for (uint8_t note = 0; note < MAX_ACTIVE_NOTES; ++note) {
    if ( voices[note].dx7_note != NULL ) {
      delete voices[note].dx7_note;
      voices[note].dx7_note = NULL;
    } 
    voices[note].keydown = false;
    voices[note].sustained = false;
    voices[note].live = false;
  }

  TRACE("Bye");
}

void Dexed::activate(void)
{
  TRACE("Hi");

  Plugin::activate();
  set_params();
  refreshVoice=true;

  TRACE("Bye");
}

void Dexed::deactivate(void)
{
  TRACE("Hi");

  Plugin::deactivate();

  TRACE("Bye");
}

void Dexed::set_params(void)
{
  TRACE("Hi");

  // Dexed-Unisono
  if(isMonoMode()!=bool(*p(p_unisono)))
    setMonoMode(bool(*p(p_unisono)));

  // Dexed-Engine
  if(controllers.core==NULL || getEngineType()!=uint8_t(*p(p_engine))-1)
  {
    setEngineType(uint8_t(*p(p_engine))-1);
    refreshVoice=true;
  }

  // Dexed-Filter
  if(fx.uiCutoff!=*p(p_cutoff))
  {
    fx.uiCutoff=*p(p_cutoff);
    refreshVoice=true;
  }
  if(fx.uiReso!=*p(p_resonance))
  {
    fx.uiReso=*p(p_resonance);
    refreshVoice=true;
  }
  if(fx.uiGain!=*p(p_output))
  {
    fx.uiGain=*p(p_output);
    refreshVoice=true;
  }

  // OP6
  onParam(0,static_cast<char>(*p(p_op6_eg_rate_1)));
  onParam(1,static_cast<char>(*p(p_op6_eg_rate_2)));
  onParam(2,static_cast<char>(*p(p_op6_eg_rate_3)));
  onParam(3,static_cast<char>(*p(p_op6_eg_rate_4)));
  onParam(4,static_cast<char>(*p(p_op6_eg_level_1)));
  onParam(5,static_cast<char>(*p(p_op6_eg_level_2)));
  onParam(6,static_cast<char>(*p(p_op6_eg_level_3)));
  onParam(7,static_cast<char>(*p(p_op6_eg_level_4)));
  onParam(8,static_cast<char>(*p(p_op6_kbd_lev_scl_brk_pt)));
  onParam(9,static_cast<char>(*p(p_op6_kbd_lev_scl_lft_depth)));
  onParam(10,static_cast<char>(*p(p_op6_kbd_lev_scl_rht_depth)));
  onParam(11,static_cast<char>(*p(p_op6_kbd_lev_scl_lft_curve)));
  onParam(12,static_cast<char>(*p(p_op6_kbd_lev_scl_rht_curve)));
  onParam(13,static_cast<char>(*p(p_op6_kbd_rate_scaling)));
  onParam(14,static_cast<char>(*p(p_op6_amp_mod_sensitivity)));
  onParam(15,static_cast<char>(*p(p_op6_key_vel_sensitivity)));
  onParam(16,static_cast<char>(*p(p_op6_operator_output_level)));
  onParam(17,static_cast<char>(*p(p_op6_osc_mode)));
  onParam(18,static_cast<char>(*p(p_op6_osc_freq_coarse)));
  onParam(19,static_cast<char>(*p(p_op6_osc_freq_fine)));
  onParam(20,static_cast<char>(*p(p_op6_osc_detune)+7));
  // OP5
  onParam(21,static_cast<char>(*p(p_op5_eg_rate_1)));
  onParam(22,static_cast<char>(*p(p_op5_eg_rate_2)));
  onParam(23,static_cast<char>(*p(p_op5_eg_rate_3)));
  onParam(24,static_cast<char>(*p(p_op5_eg_rate_4)));
  onParam(25,static_cast<char>(*p(p_op5_eg_level_1)));
  onParam(26,static_cast<char>(*p(p_op5_eg_level_2)));
  onParam(27,static_cast<char>(*p(p_op5_eg_level_3)));
  onParam(28,static_cast<char>(*p(p_op5_eg_level_4)));
  onParam(29,static_cast<char>(*p(p_op5_kbd_lev_scl_brk_pt)));
  onParam(30,static_cast<char>(*p(p_op5_kbd_lev_scl_lft_depth)));
  onParam(31,static_cast<char>(*p(p_op5_kbd_lev_scl_rht_depth)));
  onParam(32,static_cast<char>(*p(p_op5_kbd_lev_scl_lft_curve)));
  onParam(33,static_cast<char>(*p(p_op5_kbd_lev_scl_rht_curve)));
  onParam(34,static_cast<char>(*p(p_op5_kbd_rate_scaling)));
  onParam(35,static_cast<char>(*p(p_op5_amp_mod_sensitivity)));
  onParam(36,static_cast<char>(*p(p_op5_key_vel_sensitivity)));
  onParam(37,static_cast<char>(*p(p_op5_operator_output_level)));
  onParam(38,static_cast<char>(*p(p_op5_osc_mode)));
  onParam(39,static_cast<char>(*p(p_op5_osc_freq_coarse)));
  onParam(40,static_cast<char>(*p(p_op5_osc_freq_fine)));
  onParam(41,static_cast<char>(*p(p_op5_osc_detune)+7));
  // OP4
  onParam(42,static_cast<char>(*p(p_op4_eg_rate_1)));
  onParam(43,static_cast<char>(*p(p_op4_eg_rate_2)));
  onParam(44,static_cast<char>(*p(p_op4_eg_rate_3)));
  onParam(45,static_cast<char>(*p(p_op4_eg_rate_4)));
  onParam(46,static_cast<char>(*p(p_op4_eg_level_1)));
  onParam(47,static_cast<char>(*p(p_op4_eg_level_2)));
  onParam(48,static_cast<char>(*p(p_op4_eg_level_3)));
  onParam(49,static_cast<char>(*p(p_op4_eg_level_4)));
  onParam(50,static_cast<char>(*p(p_op4_kbd_lev_scl_brk_pt)));
  onParam(51,static_cast<char>(*p(p_op4_kbd_lev_scl_lft_depth)));
  onParam(52,static_cast<char>(*p(p_op4_kbd_lev_scl_rht_depth)));
  onParam(53,static_cast<char>(*p(p_op4_kbd_lev_scl_lft_curve)));
  onParam(54,static_cast<char>(*p(p_op4_kbd_lev_scl_rht_curve)));
  onParam(55,static_cast<char>(*p(p_op4_kbd_rate_scaling)));
  onParam(56,static_cast<char>(*p(p_op4_amp_mod_sensitivity)));
  onParam(57,static_cast<char>(*p(p_op4_key_vel_sensitivity)));
  onParam(58,static_cast<char>(*p(p_op4_operator_output_level)));
  onParam(59,static_cast<char>(*p(p_op4_osc_mode)));
  onParam(60,static_cast<char>(*p(p_op4_osc_freq_coarse)));
  onParam(61,static_cast<char>(*p(p_op4_osc_freq_fine)));
  onParam(62,static_cast<char>(*p(p_op4_osc_detune)+7));
  // OP3
  onParam(63,static_cast<char>(*p(p_op3_eg_rate_1)));
  onParam(64,static_cast<char>(*p(p_op3_eg_rate_2)));
  onParam(65,static_cast<char>(*p(p_op3_eg_rate_3)));
  onParam(66,static_cast<char>(*p(p_op3_eg_rate_4)));
  onParam(67,static_cast<char>(*p(p_op3_eg_level_1)));
  onParam(68,static_cast<char>(*p(p_op3_eg_level_2)));
  onParam(69,static_cast<char>(*p(p_op3_eg_level_3)));
  onParam(70,static_cast<char>(*p(p_op3_eg_level_4)));
  onParam(71,static_cast<char>(*p(p_op3_kbd_lev_scl_brk_pt)));
  onParam(72,static_cast<char>(*p(p_op3_kbd_lev_scl_lft_depth)));
  onParam(73,static_cast<char>(*p(p_op3_kbd_lev_scl_rht_depth)));
  onParam(74,static_cast<char>(*p(p_op3_kbd_lev_scl_lft_curve)));
  onParam(75,static_cast<char>(*p(p_op3_kbd_lev_scl_rht_curve)));
  onParam(76,static_cast<char>(*p(p_op3_kbd_rate_scaling)));
  onParam(77,static_cast<char>(*p(p_op3_amp_mod_sensitivity)));
  onParam(78,static_cast<char>(*p(p_op3_key_vel_sensitivity)));
  onParam(79,static_cast<char>(*p(p_op3_operator_output_level)));
  onParam(80,static_cast<char>(*p(p_op3_osc_mode)));
  onParam(81,static_cast<char>(*p(p_op3_osc_freq_coarse)));
  onParam(82,static_cast<char>(*p(p_op3_osc_freq_fine)));
  onParam(83,static_cast<char>(*p(p_op3_osc_detune)+7));
  // OP2
  onParam(84,static_cast<char>(*p(p_op2_eg_rate_1)));
  onParam(85,static_cast<char>(*p(p_op2_eg_rate_2)));
  onParam(86,static_cast<char>(*p(p_op2_eg_rate_3)));
  onParam(87,static_cast<char>(*p(p_op2_eg_rate_4)));
  onParam(88,static_cast<char>(*p(p_op2_eg_level_1)));
  onParam(89,static_cast<char>(*p(p_op2_eg_level_2)));
  onParam(90,static_cast<char>(*p(p_op2_eg_level_3)));
  onParam(91,static_cast<char>(*p(p_op2_eg_level_4)));
  onParam(92,static_cast<char>(*p(p_op2_kbd_lev_scl_brk_pt)));
  onParam(93,static_cast<char>(*p(p_op2_kbd_lev_scl_lft_depth)));
  onParam(94,static_cast<char>(*p(p_op2_kbd_lev_scl_rht_depth)));
  onParam(95,static_cast<char>(*p(p_op2_kbd_lev_scl_lft_curve)));
  onParam(96,static_cast<char>(*p(p_op2_kbd_lev_scl_rht_curve)));
  onParam(97,static_cast<char>(*p(p_op2_kbd_rate_scaling)));
  onParam(98,static_cast<char>(*p(p_op2_amp_mod_sensitivity)));
  onParam(99,static_cast<char>(*p(p_op2_key_vel_sensitivity)));
  onParam(100,static_cast<char>(*p(p_op2_operator_output_level)));
  onParam(101,static_cast<char>(*p(p_op2_osc_mode)));
  onParam(102,static_cast<char>(*p(p_op2_osc_freq_coarse)));
  onParam(103,static_cast<char>(*p(p_op2_osc_freq_fine)));
  onParam(104,static_cast<char>(*p(p_op2_osc_detune)+7));
  // OP1
  onParam(105,static_cast<char>(*p(p_op1_eg_rate_1)));
  onParam(106,static_cast<char>(*p(p_op1_eg_rate_2)));
  onParam(107,static_cast<char>(*p(p_op1_eg_rate_3)));
  onParam(108,static_cast<char>(*p(p_op1_eg_rate_4)));
  onParam(109,static_cast<char>(*p(p_op1_eg_level_1)));
  onParam(110,static_cast<char>(*p(p_op1_eg_level_2)));
  onParam(111,static_cast<char>(*p(p_op1_eg_level_3)));
  onParam(112,static_cast<char>(*p(p_op1_eg_level_4)));
  onParam(113,static_cast<char>(*p(p_op1_kbd_lev_scl_brk_pt)));
  onParam(114,static_cast<char>(*p(p_op1_kbd_lev_scl_lft_depth)));
  onParam(115,static_cast<char>(*p(p_op1_kbd_lev_scl_rht_depth)));
  onParam(116,static_cast<char>(*p(p_op1_kbd_lev_scl_lft_curve)));
  onParam(117,static_cast<char>(*p(p_op1_kbd_lev_scl_rht_curve)));
  onParam(118,static_cast<char>(*p(p_op1_kbd_rate_scaling)));
  onParam(119,static_cast<char>(*p(p_op1_amp_mod_sensitivity)));
  onParam(120,static_cast<char>(*p(p_op1_key_vel_sensitivity)));
  onParam(121,static_cast<char>(*p(p_op1_operator_output_level)));
  onParam(122,static_cast<char>(*p(p_op1_osc_mode)));
  onParam(123,static_cast<char>(*p(p_op1_osc_freq_coarse)));
  onParam(124,static_cast<char>(*p(p_op1_osc_freq_fine)));
  onParam(125,static_cast<char>(*p(p_op1_osc_detune)+7));
  // Global for all OPs
  onParam(126,static_cast<char>(*p(p_pitch_eg_rate_1)));
  onParam(127,static_cast<char>(*p(p_pitch_eg_rate_2)));
  onParam(128,static_cast<char>(*p(p_pitch_eg_rate_3)));
  onParam(129,static_cast<char>(*p(p_pitch_eg_rate_4)));
  onParam(130,static_cast<char>(*p(p_pitch_eg_level_1)));
  onParam(131,static_cast<char>(*p(p_pitch_eg_level_2)));
  onParam(132,static_cast<char>(*p(p_pitch_eg_level_3)));
  onParam(133,static_cast<char>(*p(p_pitch_eg_level_4)));
  onParam(134,static_cast<char>(*p(p_algorithm_num)-1));
  onParam(135,static_cast<char>(*p(p_feedback)));
  onParam(136,static_cast<char>(*p(p_oscillator_sync)));
  onParam(137,static_cast<char>(*p(p_lfo_speed)));
  onParam(138,static_cast<char>(*p(p_lfo_delay)));
  onParam(139,static_cast<char>(*p(p_lfo_pitch_mod_depth)));
  onParam(140,static_cast<char>(*p(p_lfo_amp_mod_depth)));
  onParam(141,static_cast<char>(*p(p_lfo_sync)));
  onParam(142,static_cast<char>(*p(p_lfo_waveform)));
  onParam(143,static_cast<char>(*p(p_pitch_mod_sensitivity)));
  onParam(144,static_cast<char>(*p(p_transpose)));
  // 10 bytes (145-154) are the name of the patch
  onParam(155,0x3f); // operator on/off => All OPs on

  TRACE("Bye");
}

// override the run() method
void Dexed::run (uint32_t sample_count)
{
    const LV2_Atom_Sequence* seq = p<LV2_Atom_Sequence> (p_midi_in);
    float* output = p(p_audio_out);
    uint32_t last_frame = 0, num_this_time = 0;

    if(++_param_counter==16)
    {
      set_params(); // pre_process: copy actual voice params
      _param_counter=0;
    }

    for (LV2_Atom_Event* ev = lv2_atom_sequence_begin (&seq->body);
         !lv2_atom_sequence_is_end(&seq->body, seq->atom.size, ev);
         ev = lv2_atom_sequence_next (ev))
    {
       num_this_time = ev->time.frames - last_frame;

       // If it's midi, send it to the engine
       if (ev->body.type == m_midi_type)
       {
         for(uint8_t i=0;i<ev->body.size;i++)
         {
           TRACE("midi msg %d: %d\n",i,((uint8_t*)LV2_ATOM_BODY(&ev->body))[i]);
         }
         ProcessMidiMessage((uint8_t*) LV2_ATOM_BODY (&ev->body),ev->body.size);
       }

       // render audio from the last frame until the timestamp of this event
       GetSamples (num_this_time, outbuf_);
      
       // i is the index of the engine's buf, which always starts at 0 (i think)
       // j is the index of the plugin's float output buffer which will be the timestamp
       // of the last processed atom event.
       for (uint32_t i = 0, j = last_frame; i < num_this_time; ++i, ++j)
         output[j]=outbuf_[i];

       last_frame = ev->time.frames;
    }

    // render remaining samples if any left
    if (last_frame < sample_count)
    {
       // do the same thing as above except from last frame until the end of
       // the processing cycles last sample. at this point, all events have
       // already been handled.

       num_this_time = sample_count - last_frame;
       GetSamples (num_this_time, outbuf_);
       for (uint32_t i = 0, j = last_frame; i < num_this_time; ++i, ++j)
         output[j] = outbuf_[i];
    }
    
    fx.process(output, sample_count);
}

void Dexed::GetSamples(uint32_t n_samples, float* buffer)
{
  uint32_t i;

  if(refreshVoice) {
    for(i=0;i < MAX_ACTIVE_NOTES;i++) {
      if ( voices[i].live )
        voices[i].dx7_note->update(data, voices[i].midi_note, feedback_bitdepth);
    }
    lfo.reset(data+137);
    refreshVoice = false;
  }

  // flush first events
  for (i=0; i < n_samples && i < extra_buf_size_; i++) {
    buffer[i] = extra_buf_[i];
  }
    
  // remaining buffer is still to be processed
  if (extra_buf_size_ > n_samples) {
    for (uint32_t j = 0; j < extra_buf_size_ - n_samples; j++) {
      extra_buf_[j] = extra_buf_[j + n_samples];
    }
    extra_buf_size_ -= n_samples;
  }
  else
  {
    for (; i < n_samples; i += N) {
      AlignedBuf<int32_t, N> audiobuf;
      float sumbuf[N];
      
      for (uint32_t j = 0; j < N; ++j) {
        audiobuf.get()[j] = 0;
        sumbuf[j] = 0.0;
      }

      int32_t lfovalue = lfo.getsample();
      int32_t lfodelay = lfo.getdelay();
            
      for (uint8_t note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        if (voices[note].live) {
          voices[note].dx7_note->compute(audiobuf.get(), lfovalue, lfodelay, &controllers);
          for (uint32_t j=0; j < N; ++j) {
            int32_t val = audiobuf.get()[j];
            val = val >> 4;
            int32_t clip_val = val < -(1 << 24) ? 0x8000 : val >= (1 << 24) ? 0x7fff : val >> 9;
            float f = float(clip_val) / float(0x8000);
            if(f>1.0)
              f=1.0;
            if(f<-1.0)
              f=-1.0;
            sumbuf[j]+=f;
            audiobuf.get()[j]=0;
          }
        }
      }

      uint32_t jmax = n_samples - i;
      for (uint32_t j = 0; j < N; ++j) {
        if (j < jmax) {
          buffer[i + j] = sumbuf[j];
        } else {
          extra_buf_[j - jmax] = sumbuf[j];
        }
      }
    }
    extra_buf_size_ = i - n_samples;
  }

  for(i=0;i < MAX_ACTIVE_NOTES;i++) {
    if(voices[i].live==true && voices[i].keydown==false)
    {
      uint8_t op_amp=0;

      voices[i].dx7_note->peekVoiceStatus(voiceStatus);

      for(uint8_t op=0;op<6;op++)
      {
//        TRACE("Voice[%2d] OP [%d] amp=%ld,amp_step=%d,pitch_step=%d",i,op,voiceStatus.amp[op],voiceStatus.ampStep[op],voiceStatus.pitchStep);

        if(voiceStatus.amp[op]<=1069)
          op_amp++;

      }
      if(op_amp==6)
        voices[i].live=false;
    }
//    TRACE("Voice[%2d] live=%d keydown=%d",i,voices[i].live,voices[i].keydown);
  }
}

void Dexed::ProcessMidiMessage(const uint8_t *buf, uint32_t buf_size) {
    TRACE("Hi");

    uint8_t cmd = buf[0];

    switch(cmd & 0xf0) {
        case 0x80 :
            keyup(buf[1]);
            break;
        case 0x90 :
            keydown(buf[1], buf[2]);
            break;
        case 0xb0 : {
            uint8_t ctrl = buf[1];
            uint8_t value = buf[2];
            
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
                case 64:
                    sustain = value > 63;
                    if (!sustain) {
                        for (uint8_t note = 0; note < MAX_ACTIVE_NOTES; note++) {
                            if (voices[note].sustained && !voices[note].keydown) {
                                voices[note].dx7_note->keyup();
                                voices[note].sustained = false;
                            }
                        }
                    }
                    break;
            }
            break;
        }

        case 0xc0 :
            //setCurrentProgram(buf[1]);
            break;
        // aftertouch
        case 0xd0 :
            controllers.aftertouch_cc = buf[1];
            controllers.refresh();
            break;
    }

    switch (cmd) {
        case 0xe0 :
            controllers.values_[kControllerPitch] = buf[1] | (buf[2] << 7);
        break;
    }

    TRACE("MIDI event unknown: cmd=%d, val1=%d, val2=%d",buf[0],buf[1],buf[2]);

    TRACE("Bye");
}

void Dexed::keydown(uint8_t pitch, uint8_t velo) {
TRACE("Hi");
TRACE("pitch=%d, velo=%d\n",pitch,velo);
    if ( velo == 0 ) {
        keyup(pitch);
        return;
    }

    pitch += data[144] - 24;
    
    if ( normalizeDxVelocity ) {
        velo = ((float)velo) * 0.7874015; // 100/127
    }
    
    uint8_t note = currentNote;
    for (uint8_t i=0; i<MAX_ACTIVE_NOTES; i++) {
        if (!voices[note].keydown) {
            currentNote = (note + 1) % MAX_ACTIVE_NOTES;
            lfo.keydown();  // TODO: should only do this if # keys down was 0
            voices[note].midi_note = pitch;
            voices[note].sustained = sustain;
            voices[note].keydown = true;
            voices[note].dx7_note->init(data, pitch, velo, feedback_bitdepth);
            if ( data[136] )
                voices[note].dx7_note->oscSync();
            break;
        }
        note = (note + 1) % MAX_ACTIVE_NOTES;
    }
    
    if ( monoMode ) {
        for(uint8_t i=0; i<MAX_ACTIVE_NOTES; i++) {            
            if ( voices[i].live ) {
                // all keys are up, only transfert signal
                if ( ! voices[i].keydown ) {
                    voices[i].live = false;
                    voices[note].dx7_note->transferSignal(*voices[i].dx7_note);
                    break;
                }
                if ( voices[i].midi_note < pitch ) {
                    voices[i].live = false;
                    voices[note].dx7_note->transferState(*voices[i].dx7_note);
                    break;
                }
                return;
            }
        }
    }
 
    voices[note].live = true;
TRACE("Bye");
}

void Dexed::keyup(uint8_t pitch) {
TRACE("Hi");
TRACE("pitch=%d\n",pitch);

    pitch += data[144] - 24;

    uint8_t note;
    for (note=0; note<MAX_ACTIVE_NOTES; ++note) {
        if ( voices[note].midi_note == pitch && voices[note].keydown ) {
            voices[note].keydown = false;
            break;
        }
    }
    
    // note not found ?
    if ( note >= MAX_ACTIVE_NOTES ) {
        TRACE("note-off not found???");
        return;
    }
    
    if ( monoMode ) {
        uint8_t highNote = -1;
        int target = 0;
        for (int i=0; i<MAX_ACTIVE_NOTES;i++) {
            if ( voices[i].keydown && voices[i].midi_note > highNote ) {
                target = i;
                highNote = voices[i].midi_note;
            }
        }
        
        if ( highNote != -1 ) {
            voices[note].live = false;
            voices[target].live = true;
            voices[target].dx7_note->transferState(*voices[note].dx7_note);
        }
    }
    
    if ( sustain ) {
        voices[note].sustained = true;
    } else {
        voices[note].dx7_note->keyup();
    }
TRACE("Bye");
}

void Dexed::onParam(uint8_t param_num,uint8_t param_val)
{
	if(param_val!=data[param_num])
	{
	  if(param_num==144)
            panic();
	  refreshVoice=true;
	  data[param_num]=param_val;
	}
}

uint8_t Dexed::getEngineType() {
    return engineType;
}

void Dexed::setEngineType(uint8_t tp) {
    TRACE("settings engine %d", tp);

    if(engineType==tp)
      return;

    panic();
    switch (tp)  {
        case DEXED_ENGINE_MARKI:
            controllers.core = &engineMkI;
            feedback_bitdepth = 11;
            break;
        case DEXED_ENGINE_OPL:
            controllers.core = &engineOpl;
            feedback_bitdepth = 11;
            break;
        default:
            controllers.core = &engineMsfa;
            feedback_bitdepth = 8;
            break;
    }
    engineType = tp;
}

bool Dexed::isMonoMode(void) {
    return monoMode;
}

void Dexed::setMonoMode(bool mode) {
    if(monoMode==mode)
      return;

    monoMode = mode;
}

void Dexed::panic(void) {
  for(uint8_t i=0;i<MAX_ACTIVE_NOTES;i++) {
    voices[i].keydown = false;
    voices[i].live = false;
    if ( voices[i].dx7_note != NULL ) {
      voices[i].dx7_note->oscSync();
    }
  }
}

void Dexed::init(double rate)
{
  TRACE("Hi");

  bufsize_=256;
  outbuf_=new float[bufsize_];

  Exp2::init();
  Tanh::init();
  Sin::init();

  normalizeDxVelocity = false;

  memset(&voiceStatus, 0, sizeof(VoiceStatus));

  Freqlut::init(rate);
  Lfo::init(rate);
  PitchEnv::init(rate);
  Env::init_sr(rate);
  fx.init(rate);

  for (uint8_t note = 0; note < MAX_ACTIVE_NOTES; ++note) {
    voices[note].dx7_note = new Dx7Note;
    voices[note].keydown = false;
    voices[note].sustained = false;
    voices[note].live = false;
  }

  refreshVoice=true;

  currentNote = 0;
  controllers.values_[kControllerPitch] = 0x2000;
  controllers.modwheel_cc = 0;
  controllers.foot_cc = 0;
  controllers.breath_cc = 0;
  controllers.aftertouch_cc = 0;

  //setEngineType(DEXED_ENGINE_MARKI);
  setMonoMode(false);

  sustain = false;

  lfo.reset(data+137);

  extra_buf_size_ = 0;

}

//==============================================================================

DexedVoice::DexedVoice(double rate) : m_key(lvtk::INVALID_KEY), m_rate(rate)
{
  TRACE("Hi");

  TRACE("Bye");
}

DexedVoice::~DexedVoice()
{
  TRACE("Hi");

  TRACE("Bye");
}

void DexedVoice::on(unsigned char key, unsigned char velocity)
{
  TRACE("Hi");

  m_key = key;

  TRACE("Bye");
}

void DexedVoice::off(unsigned char velocity)
{
  TRACE("Hi");

  m_key = lvtk::INVALID_KEY;

  TRACE("Bye");
}

unsigned char DexedVoice::get_key(void) const
{
  TRACE("Hi");

  return m_key;

  TRACE("Bye");
}

static int _ = Dexed::register_class(p_uri);
