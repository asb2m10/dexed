/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class ParameterListener   : private AudioProcessorParameter::Listener,
                            private AudioProcessorListener,
                            private Timer
{
public:
    ParameterListener (AudioProcessor& proc, AudioProcessorParameter& param)
        : processor (proc), parameter (param), isLegacyParam (LegacyAudioParameter::isLegacy (&param))
    {
        if (isLegacyParam)
            processor.addListener (this);
        else
            parameter.addListener (this);

        startTimer (100);
    }

    ~ParameterListener() override
    {
        if (isLegacyParam)
            processor.removeListener (this);
        else
            parameter.removeListener (this);
    }

    AudioProcessorParameter& getParameter() const noexcept
    {
        return parameter;
    }

    virtual void handleNewParameterValue() = 0;

private:
    //==============================================================================
    void parameterValueChanged (int, float) override
    {
        parameterValueHasChanged = 1;
    }

    void parameterGestureChanged (int, bool) override {}

    //==============================================================================
    void audioProcessorParameterChanged (AudioProcessor*, int index, float) override
    {
        if (index == parameter.getParameterIndex())
            parameterValueHasChanged = 1;
    }

    void audioProcessorChanged (AudioProcessor*) override {}

    //==============================================================================
    void timerCallback() override
    {
        if (parameterValueHasChanged.compareAndSetBool (0, 1))
        {
            handleNewParameterValue();
            startTimerHz (50);
        }
        else
        {
            startTimer (jmin (250, getTimerInterval() + 10));
        }
    }

    AudioProcessor& processor;
    AudioProcessorParameter& parameter;
    Atomic<int> parameterValueHasChanged { 0 };
    const bool isLegacyParam;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterListener)
};

//==============================================================================
class BooleanParameterComponent final   : public Component,
                                          private ParameterListener
{
public:
    BooleanParameterComponent (AudioProcessor& proc, AudioProcessorParameter& param)
        : ParameterListener (proc, param)
    {
        // Set the initial value.
        handleNewParameterValue();

        button.onClick = [this] { buttonClicked(); };

        addAndMakeVisible (button);
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto area = getLocalBounds();
        area.removeFromLeft (8);
        button.setBounds (area.reduced (0, 10));
    }

private:
    void handleNewParameterValue() override
    {
        button.setToggleState (isParameterOn(), dontSendNotification);
    }

    void buttonClicked()
    {
        if (isParameterOn() != button.getToggleState())
        {
            getParameter().beginChangeGesture();
            getParameter().setValueNotifyingHost (button.getToggleState() ? 1.0f : 0.0f);
            getParameter().endChangeGesture();
        }
    }

    bool isParameterOn() const    { return getParameter().getValue() >= 0.5f; }

    ToggleButton button;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BooleanParameterComponent)
};

//==============================================================================
class SwitchParameterComponent final   : public Component,
                                         private ParameterListener
{
public:
    SwitchParameterComponent (AudioProcessor& proc, AudioProcessorParameter& param)
        : ParameterListener (proc, param)
    {
        for (auto& button : buttons)
        {
            button.setRadioGroupId (293847);
            button.setClickingTogglesState (true);
        }

        buttons[0].setButtonText (getParameter().getText (0.0f, 16));
        buttons[1].setButtonText (getParameter().getText (1.0f, 16));

        buttons[0].setConnectedEdges (Button::ConnectedOnRight);
        buttons[1].setConnectedEdges (Button::ConnectedOnLeft);

        // Set the initial value.
        buttons[0].setToggleState (true, dontSendNotification);
        handleNewParameterValue();

        buttons[1].onStateChange = [this] { rightButtonChanged(); };

        for (auto& button : buttons)
            addAndMakeVisible (button);
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto area = getLocalBounds().reduced (0, 8);
        area.removeFromLeft (8);

        for (auto& button : buttons)
            button.setBounds (area.removeFromLeft (80));
    }

private:
    void handleNewParameterValue() override
    {
        bool newState = isParameterOn();

        if (buttons[1].getToggleState() != newState)
        {
            buttons[1].setToggleState (newState,   dontSendNotification);
            buttons[0].setToggleState (! newState, dontSendNotification);
        }
    }

    void rightButtonChanged()
    {
        auto buttonState = buttons[1].getToggleState();

        if (isParameterOn() != buttonState)
        {
            getParameter().beginChangeGesture();

            if (getParameter().getAllValueStrings().isEmpty())
            {
                getParameter().setValueNotifyingHost (buttonState ? 1.0f : 0.0f);
            }
            else
            {
                // When a parameter provides a list of strings we must set its
                // value using those strings, rather than a float, because VSTs can
                // have uneven spacing between the different allowed values and we
                // want the snapping behaviour to be consistent with what we do with
                // a combo box.
                auto selectedText = buttons[buttonState ? 1 : 0].getButtonText();
                getParameter().setValueNotifyingHost (getParameter().getValueForText (selectedText));
            }

            getParameter().endChangeGesture();
        }
    }

    bool isParameterOn() const
    {
        if (getParameter().getAllValueStrings().isEmpty())
            return getParameter().getValue() > 0.5f;

        auto index = getParameter().getAllValueStrings()
                                   .indexOf (getParameter().getCurrentValueAsText());

        if (index < 0)
        {
            // The parameter is producing some unexpected text, so we'll do
            // some linear interpolation.
            index = roundToInt (getParameter().getValue());
        }

        return index == 1;
    }

    TextButton buttons[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwitchParameterComponent)
};

//==============================================================================
class ChoiceParameterComponent final   : public Component,
                                         private ParameterListener
{
public:
    ChoiceParameterComponent (AudioProcessor& proc, AudioProcessorParameter& param)
        : ParameterListener (proc, param),
          parameterValues (getParameter().getAllValueStrings())
    {
        box.addItemList (parameterValues, 1);

        // Set the initial value.
        handleNewParameterValue();

        box.onChange = [this] { boxChanged(); };
        addAndMakeVisible (box);
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto area = getLocalBounds();
        area.removeFromLeft (8);
        box.setBounds (area.reduced (0, 10));
    }

private:
    void handleNewParameterValue() override
    {
        auto index = parameterValues.indexOf (getParameter().getCurrentValueAsText());

        if (index < 0)
        {
            // The parameter is producing some unexpected text, so we'll do
            // some linear interpolation.
            index = roundToInt (getParameter().getValue() * (parameterValues.size() - 1));
        }

        box.setSelectedItemIndex (index);
    }

    void boxChanged()
    {
        if (getParameter().getCurrentValueAsText() != box.getText())
        {
            getParameter().beginChangeGesture();

            // When a parameter provides a list of strings we must set its
            // value using those strings, rather than a float, because VSTs can
            // have uneven spacing between the different allowed values.
            getParameter().setValueNotifyingHost (getParameter().getValueForText (box.getText()));

            getParameter().endChangeGesture();
        }
    }

    ComboBox box;
    const StringArray parameterValues;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoiceParameterComponent)
};

//==============================================================================
class SliderParameterComponent final   : public Component,
                                         private ParameterListener
{
public:
    SliderParameterComponent (AudioProcessor& proc, AudioProcessorParameter& param)
        : ParameterListener (proc, param)
    {
        if (getParameter().getNumSteps() != AudioProcessor::getDefaultNumParameterSteps())
            slider.setRange (0.0, 1.0, 1.0 / (getParameter().getNumSteps() - 1.0));
        else
            slider.setRange (0.0, 1.0);

        slider.setScrollWheelEnabled (false);
        addAndMakeVisible (slider);

        valueLabel.setColour (Label::outlineColourId, slider.findColour (Slider::textBoxOutlineColourId));
        valueLabel.setBorderSize ({ 1, 1, 1, 1 });
        valueLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (valueLabel);

        // Set the initial value.
        handleNewParameterValue();

        slider.onValueChange = [this] { sliderValueChanged(); };
        slider.onDragStart   = [this] { sliderStartedDragging(); };
        slider.onDragEnd     = [this] { sliderStoppedDragging(); };
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto area = getLocalBounds().reduced (0, 10);

        valueLabel.setBounds (area.removeFromRight (80));

        area.removeFromLeft (6);
        slider.setBounds (area);
    }

private:
    void updateTextDisplay()
    {
        valueLabel.setText (getParameter().getCurrentValueAsText(), dontSendNotification);
    }

    void handleNewParameterValue() override
    {
        if (! isDragging)
        {
            slider.setValue (getParameter().getValue(), dontSendNotification);
            updateTextDisplay();
        }
    }

    void sliderValueChanged()
    {
        auto newVal = (float) slider.getValue();

        if (getParameter().getValue() != newVal)
        {
            if (! isDragging)
                getParameter().beginChangeGesture();

            getParameter().setValueNotifyingHost ((float) slider.getValue());
            updateTextDisplay();

            if (! isDragging)
                getParameter().endChangeGesture();
        }
    }

    void sliderStartedDragging()
    {
        isDragging = true;
        getParameter().beginChangeGesture();
    }

    void sliderStoppedDragging()
    {
        isDragging = false;
        getParameter().endChangeGesture();
    }

    Slider slider { Slider::LinearHorizontal, Slider::TextEntryBoxPosition::NoTextBox };
    Label valueLabel;
    bool isDragging = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderParameterComponent)
};

//==============================================================================
class ParameterDisplayComponent   : public Component
{
public:
    ParameterDisplayComponent (AudioProcessor& processor, AudioProcessorParameter& param)
        : parameter (param)
    {
        parameterName.setText (parameter.getName (128), dontSendNotification);
        parameterName.setJustificationType (Justification::centredRight);
        addAndMakeVisible (parameterName);

        parameterLabel.setText (parameter.getLabel(), dontSendNotification);
        addAndMakeVisible (parameterLabel);

        addAndMakeVisible (*(parameterComp = createParameterComp (processor)));

        setSize (400, 40);
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto area = getLocalBounds();

        parameterName.setBounds (area.removeFromLeft (100));
        parameterLabel.setBounds (area.removeFromRight (50));
        parameterComp->setBounds (area);
    }

private:
    AudioProcessorParameter& parameter;
    Label parameterName, parameterLabel;
    std::unique_ptr<Component> parameterComp;

    std::unique_ptr<Component> createParameterComp (AudioProcessor& processor) const
    {
        // The AU, AUv3 and VST (only via a .vstxml file) SDKs support
        // marking a parameter as boolean. If you want consistency across
        // all  formats then it might be best to use a
        // SwitchParameterComponent instead.
        if (parameter.isBoolean())
            return std::make_unique<BooleanParameterComponent> (processor, parameter);

        // Most hosts display any parameter with just two steps as a switch.
        if (parameter.getNumSteps() == 2)
            return std::make_unique<SwitchParameterComponent> (processor, parameter);

        // If we have a list of strings to represent the different states a
        // parameter can be in then we should present a dropdown allowing a
        // user to pick one of them.
        if (! parameter.getAllValueStrings().isEmpty()
             && std::abs (parameter.getNumSteps() - parameter.getAllValueStrings().size()) <= 1)
            return std::make_unique<ChoiceParameterComponent> (processor, parameter);

        // Everything else can be represented as a slider.
        return std::make_unique<SliderParameterComponent> (processor, parameter);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterDisplayComponent)
};

//==============================================================================
class ParametersPanel   : public Component
{
public:
    ParametersPanel (AudioProcessor& processor, const Array<AudioProcessorParameter*>& parameters)
    {
        for (auto* param : parameters)
            if (param->isAutomatable())
                addAndMakeVisible (paramComponents.add (new ParameterDisplayComponent (processor, *param)));

        int maxWidth = 400;
        int height = 0;

        for (auto& comp : paramComponents)
        {
            maxWidth = jmax (maxWidth, comp->getWidth());
            height += comp->getHeight();
        }

        setSize (maxWidth, jmax (height, 125));
    }

    ~ParametersPanel() override
    {
        paramComponents.clear();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto area = getLocalBounds();

        for (auto* comp : paramComponents)
            comp->setBounds (area.removeFromTop (comp->getHeight()));
    }

private:
    OwnedArray<ParameterDisplayComponent> paramComponents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametersPanel)
};

//==============================================================================
struct GenericAudioProcessorEditor::Pimpl
{
    Pimpl (GenericAudioProcessorEditor& parent)  : owner (parent)
    {
        auto* p = parent.getAudioProcessor();
        jassert (p != nullptr);

        legacyParameters.update (*p, false);

        owner.setOpaque (true);

        view.setViewedComponent (new ParametersPanel (*p, legacyParameters.params));
        owner.addAndMakeVisible (view);

        view.setScrollBarsShown (true, false);
    }

    ~Pimpl()
    {
        view.setViewedComponent (nullptr, false);
    }

    void resize (Rectangle<int> size)
    {
        view.setBounds (size);
        auto content = view.getViewedComponent();
        content->setSize (view.getMaximumVisibleWidth(), content->getHeight());
    }

    //==============================================================================
    GenericAudioProcessorEditor& owner;
    LegacyAudioParametersWrapper legacyParameters;
    Viewport view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl)
};


//==============================================================================
GenericAudioProcessorEditor::GenericAudioProcessorEditor (AudioProcessor& p)
    : AudioProcessorEditor (p), pimpl (new Pimpl (*this))
{
    setSize (pimpl->view.getViewedComponent()->getWidth() + pimpl->view.getVerticalScrollBar().getWidth(),
             jmin (pimpl->view.getViewedComponent()->getHeight(), 400));
}

GenericAudioProcessorEditor::~GenericAudioProcessorEditor() {}

void GenericAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void GenericAudioProcessorEditor::resized()
{
    pimpl->resize (getLocalBounds());
}

} // namespace juce
