#ifndef __SYNTH_TUNING_H
#define __SYNTH_TUNING_H

#include "synth.h"
#include <memory>
#include <string>
#include "Tunings.h"

class TuningState {
public:
    virtual ~TuningState() { }

    virtual int32_t midinote_to_logfreq(int midinote) = 0;
    virtual bool is_standard_tuning() { return true; }
    virtual int scale_length() { return 12; }
    virtual std::string display_tuning_str() { return "Standard Tuning"; }

    virtual Tunings::Tuning &getTuning() {
        static Tunings::Tuning t;
        return t;
    }
};

std::shared_ptr<TuningState> createStandardTuning();

std::shared_ptr<TuningState> createTuningFromSCLData( const std::string &sclData );
std::shared_ptr<TuningState> createTuningFromKBMData( const std::string &kbmData );
std::shared_ptr<TuningState> createTuningFromSCLAndKBMData( const std::string &sclData, const std::string &kbmData );

#endif
