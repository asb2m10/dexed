#ifndef __SYNTH_TUNING_H
#define __SYNTH_TUNING_H

#include "synth.h"
#include <memory>
#include <string>

class TuningState {
public:
    virtual ~TuningState() { }
    
    virtual int32_t midinote_to_logfreq(int midinote) = 0;
};

std::shared_ptr<TuningState> createStandardTuning();

std::shared_ptr<TuningState> createTuningFromSCLData( const std::string &sclData );
std::shared_ptr<TuningState> createTuningFromKBMData( const std::string &kbmData );
std::shared_ptr<TuningState> createTuningFromSCLAndKBMData( const std::string &sclData, const std::string &kbmData );

#endif
