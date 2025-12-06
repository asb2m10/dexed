#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Quick and dirty container to manage audio components and their attachments.
 *
 * It expects the component's name to match the parameter ID in the AudioProcessorValueTreeState.
 */
class AudioComponentContainer {
    using Avts = juce::AudioProcessorValueTreeState;
    Avts &avts;
    juce::Component &parent;
    std::vector<std::unique_ptr<juce::Component>> components;
    std::vector<std::unique_ptr<Avts::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr<Avts::ButtonAttachment>> buttonAttachments;
    std::vector<std::unique_ptr<Avts::ComboBoxAttachment>> comboBoxAttachments;

public:
    AudioComponentContainer(juce::Component &parent, Avts &avts) : avts(avts), parent(parent) {
    }

    void add(std::unique_ptr<juce::Component> comp) {
        parent.addAndMakeVisible(comp.get());
        components.push_back(std::move(comp));
    }

    void addAndAttach(std::unique_ptr<juce::Component> component) {
        parent.addAndMakeVisible(component.get());
        juce::RangedAudioParameter *param = avts.getParameter(component.get()->getName());
        if ( param != nullptr ) {
            if ( auto slider = dynamic_cast<juce::Slider *>(component.get()) ) {
                sliderAttachments.push_back(std::make_unique<Avts::SliderAttachment>(avts, param->getName(), *slider));
            } else if ( auto button = dynamic_cast<juce::Button *>(component) ) {
                buttonAttachments.push_back(std::make_unique<Avts::ButtonAttachment>(avts, param->getName(), *button));
            } else if ( auto comboBox = dynamic_cast<juce::ComboBox *>(component) ) {
                comboBoxAttachments.push_back(std::make_unique<Avts::ComboBoxAttachment>(avts, param->getName(), *comboBox));
            } else {
                // Unhandled attachement
                jassertfalse;
            }
        } else {
            // Parameter not found
            jassertfalse;
        }
        components.push_back(std::move(component));
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