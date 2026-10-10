#include "tuning.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

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

    virtual int scale_length() override { return tuning.scale.count; }
    virtual std::string display_tuning_str() override { return "SCL KBM Tuning"; }

    virtual Tunings::Tuning &getTuning() override { return tuning; }

    Tunings::Tuning tuning;

};

std::shared_ptr<TuningState> createStandardTuning()
{
    return std::make_shared<StandardTuning>();
}

std::shared_ptr<TuningState> createTuningFromSCLData( const std::string &scl )
{
    Tunings::Scale s;
    try {
        s = Tunings::parseSCLData(scl);
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error parsing SCL data for SCL tuning",
            e.what(),
            "OK");
        return nullptr;
    }

    auto res = std::make_shared<SCLAndKBMTuningState>();
    try {
        res->tuning = Tunings::Tuning(s);
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error creating tuning for SCL tuning",
            e.what(),
            "OK");
        res = nullptr;
    }

    return res;
}

std::shared_ptr<TuningState> createTuningFromKBMData( const std::string &kbm )
{
    Tunings::KeyboardMapping k;
    try {
        k = Tunings::parseKBMData(kbm);        
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error parsing KBM data for KBM tuning",
            e.what(),
            "OK");
        return nullptr;
    }

    auto res = std::make_shared<SCLAndKBMTuningState>();
    try {
        res->tuning = Tunings::Tuning(k);
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error creating tuning for KBM tuning",
            e.what(),
            "OK");
        res = nullptr;
    }

    return res;
}

std::shared_ptr<TuningState> createTuningFromSCLAndKBMData( const std::string &sclData, const std::string &kbmData )
{
    Tunings::Scale s;
    try {
        s = Tunings::parseSCLData(sclData);
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error parsing SCL data for SCL/KBM tuning",
            e.what(),
            "OK");
        return nullptr;
    }

    Tunings::KeyboardMapping k;
    try {
        k = Tunings::parseKBMData(kbmData);        
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error parsing KBM data for SCL/KBM tuning",
            e.what(),
            "OK");
        return nullptr;
    }
    
    auto res = std::make_shared<SCLAndKBMTuningState>();
    try {
        res->tuning = Tunings::Tuning(s, k);
    } catch (const std::exception& e) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,
            "Error creating tuning for SCL/KBM tuning",
            e.what(),
            "OK");
        res = nullptr;
    }
    return res;
}
