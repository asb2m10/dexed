#include "tuning.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <vector>


struct StandardTuning : public TuningState {
    StandardTuning() {
        const int base = 50857777;  // (1 << 24) * (log(440) / log(2) - 69/12) 
        const int step = (1 << 24) / 12;
        for( int mn = 0; mn < 128; ++mn )
        {
            auto res = base + step * mn;
            current_logfreq_table_[mn] = res;
        }
    }
    
    virtual int32_t midinote_to_logfreq(int midinote) override {
        return current_logfreq_table_[midinote];
    }
    
    int current_logfreq_table_[128];
};


struct SCLAndKBMTuningState : public TuningState {
    virtual bool is_standard_tuning() override {
        return false;
    }
    
    virtual int32_t midinote_to_logfreq(int midinote) override {
        const int base = 50857777;
        const int step = ( 1 << 24 );
        return tuning.logScaledFrequencyForMidiNote( midinote ) * step + base;
    }

    virtual int scale_length() { return tuning.scale.count; }
    virtual std::string display_tuning_str() { return "SCL KBM Tuning"; }

    virtual Tunings::Tuning &getTuning() override { return tuning; }

    Tunings::Tuning tuning;

};

std::shared_ptr<TuningState> createStandardTuning()
{
    return std::make_shared<StandardTuning>();
}

std::shared_ptr<TuningState> createTuningFromSCLData( const std::string &scl )
{
    auto s = Tunings::parseSCLData(scl);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->tuning = Tunings::Tuning( s );
    return res;
}

std::shared_ptr<TuningState> createTuningFromKBMData( const std::string &kbm )
{
    auto k = Tunings::parseKBMData(kbm);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->tuning = Tunings::Tuning( k );
    return res;
}

std::shared_ptr<TuningState> createTuningFromSCLAndKBMData( const std::string &sclData, const std::string &kbmData )
{
    auto s = Tunings::parseSCLData(sclData);
    auto k = Tunings::parseKBMData(kbmData);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->tuning = Tunings::Tuning( s, k );
    return res;
}
