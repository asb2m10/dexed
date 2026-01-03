#pragma once

#include "msfa/controllers.h"
#include "midi/SysexComm.h"
#include <functional>
#include "../DXLookNFeel.h"


class ParamDialog  : public Component,
                     public juce::Slider::Listener,
                     public juce::ComboBox::Listener,
                     public juce::Button::Listener,
                     public juce::Timer
{
public:
    //==============================================================================
    ParamDialog ();
    ~ParamDialog() override;

    void setDialogValues(Controllers &c, SysexComm &mgr, int reso, bool showKeyboard, float dpiScaleFactor);
    bool getDialogValues(Controllers &c, SysexComm &mgr, int *reso, bool *showKeyboard, float *dpiScaleFactor);

    typedef enum {
        LOAD_SCL,
        LOAD_KBM,
        RESET_TUNING,
        SHOW_TUNING
    } TuningAction;

    void setTuningCallback(std::function<void(ParamDialog *, ParamDialog::TuningAction)> tc ) { tuning_callback_ = tc; }
    void setGeneralCallback(std::function<void(ParamDialog *)> gc ) { general_callback_ = gc; }

    void setIsStandardTuning(bool s);
    void timerCallback() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    std::function<void(ParamDialog *, ParamDialog::TuningAction)> tuning_callback_ = [](ParamDialog *, ParamDialog::TuningAction i) {};
    bool is_standard_tuning_;
    std::function<void(ParamDialog *)> general_callback_ = [](ParamDialog *p) {};

    std::unique_ptr<juce::Slider> pitchRangeDn;
    std::unique_ptr<juce::Slider> pitchStep;
    std::unique_ptr<juce::ComboBox> sysexIn;
    std::unique_ptr<juce::ComboBox> sysexOut;
    std::unique_ptr<juce::Slider> sysexChl;
    std::unique_ptr<juce::ComboBox> engineReso;
    std::unique_ptr<LightedToggleButton> showKeyboard;
    std::unique_ptr<juce::Slider> whlRange;
    std::unique_ptr<juce::Slider> ftRange;
    std::unique_ptr<juce::Slider> brRange;
    std::unique_ptr<juce::Slider> atRange;
    std::unique_ptr<LightedToggleButton> whlEg;
    std::unique_ptr<LightedToggleButton> ftEg;
    std::unique_ptr<LightedToggleButton> brEg;
    std::unique_ptr<LightedToggleButton> atEg;
    std::unique_ptr<LightedToggleButton> whlAmp;
    std::unique_ptr<LightedToggleButton> ftAmp;
    std::unique_ptr<LightedToggleButton> brAmp;
    std::unique_ptr<LightedToggleButton> atAmp;
    std::unique_ptr<LightedToggleButton> whlPitch;
    std::unique_ptr<LightedToggleButton> ftPitch;
    std::unique_ptr<LightedToggleButton> brPitch;
    std::unique_ptr<LightedToggleButton> atPitch;
    std::unique_ptr<juce::TextButton> sclButton;
    std::unique_ptr<juce::TextButton> kbmButton;
    std::unique_ptr<juce::TextButton> showTunButton;
    std::unique_ptr<juce::TextButton> resetTuningButton;
    std::unique_ptr<LightedToggleButton> transposeScale;
    std::unique_ptr<juce::Slider> mpePBRange;
    std::unique_ptr<LightedToggleButton> mpeEnabled;
    std::unique_ptr<juce::ImageButton> transposeHelp;
    std::unique_ptr<juce::Slider> pitchRangeUp;
    std::unique_ptr<juce::ComboBox> scalingFactor;
    std::unique_ptr<LightedToggleButton> glissando;
    std::unique_ptr<juce::Slider> portamentoTm;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamDialog)
};
