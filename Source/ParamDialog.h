/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.11

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "msfa/controllers.h"
#include "SysexComm.h"
#include <functional>
#include "DXLookNFeel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
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

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
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
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::function<void(ParamDialog *, ParamDialog::TuningAction)> tuning_callback_ = [](ParamDialog *, ParamDialog::TuningAction i) {};
    bool is_standard_tuning_;
    std::function<void(ParamDialog *)> general_callback_ = [](ParamDialog *p) {};
    //[/UserVariables]

    //==============================================================================
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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

