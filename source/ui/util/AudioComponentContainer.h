#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Quick and dirty container to manage audio components and their attachments.
 *
 * It expects the component's name to match the parameter ID in the AudioProcessorValueTreeState.
 */
class AudioComponentContainer : public juce::MouseListener {
    using Apvts = juce::AudioProcessorValueTreeState;
    Apvts &apvts;
    juce::Component &parent;
    std::vector<std::unique_ptr<juce::Component>> components;
    std::vector<std::unique_ptr<Apvts::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<Apvts::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<Apvts::ComboBoxAttachment>> comboBoxAttachments;

public:
    AudioComponentContainer(juce::Component &parent, Apvts &apvts) : apvts(apvts), parent(parent) {
    }

    void add(std::unique_ptr<juce::Component> comp) {
        parent.addAndMakeVisible(comp.get());
        components.push_back(std::move(comp));
    }

    void attach(juce::Component *component) {
        juce::RangedAudioParameter *param = apvts.getParameter(component->getName());
        if ( param == nullptr ) {
            // Parameter not found
            jassertfalse;
            return;
        }
        component->setTitle(param->getLabel());

        if ( auto slider = dynamic_cast<juce::Slider *>(component) ) {
            slider->setRange(param->getNormalisableRange().start, param->getNormalisableRange().end, param->getNormalisableRange().interval);
            sliderAttachments.push_back(std::make_unique<Apvts::SliderAttachment>(apvts, component->getName(), *slider));
            return;
        }

        if ( auto button = dynamic_cast<juce::Button *>(component) ) {
            buttonAttachments.push_back(std::make_unique<Apvts::ButtonAttachment>(apvts, component->getName(), *button));
            return;
        }

        if ( auto comboBox = dynamic_cast<juce::ComboBox *>(component) ) {
            auto *choiceParam = dynamic_cast<juce::AudioParameterChoice *>(param);
            if ( choiceParam != nullptr ) {
                comboBox->clear();
                const auto &choices = choiceParam->choices;
                for ( int i = 0; i < choices.size(); ++i ) {
                    comboBox->addItem(choices[i], i + 1);
                }
            }
            comboBoxAttachments.push_back(std::make_unique<Apvts::ComboBoxAttachment>(apvts, component->getName(), *comboBox));
            return;
        }

        // Unhandled attachment
        jassertfalse;
    }

    void addAndAttach(std::unique_ptr<juce::Component> component) {
        parent.addAndMakeVisible(component.get());
        attach(component.get());
        components.push_back(std::move(component));
    }

    void mouseEnter(const juce::MouseEvent &event) override {
        juce::Component *comp = event.eventComponent;
        if ( comp != nullptr ) {
            juce::RangedAudioParameter *param = apvts.getParameter(comp->getName());
            if ( param != nullptr ) {
                juce::String tooltip = param->getLabel() + juce::String(":") + param->getCurrentValueAsText();
                //comp->setTooltip(tooltip);
            }
        }
    }

    /**
     * To be called on the parent component's destructor to free attachments first.
     */
    void freeAttachments() {
        sliderAttachments.clear();
        buttonAttachments.clear();
        comboBoxAttachments.clear();
    }
};