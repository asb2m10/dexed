#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"
#include "Dexed.h"

class AboutBox : public juce::DialogWindow {
public:
    juce::Image logo_png;
    std::unique_ptr<juce::HyperlinkButton> dexed; // changed to std::unique_ptr from juce::ScopedPointer
    std::unique_ptr<juce::HyperlinkButton> surge; // changed to std::unique_ptr from juce::ScopedPointer

    AboutBox() : juce::DialogWindow("About", juce::Colour(0xFF000000), true),
        dexed(std::make_unique<juce::HyperlinkButton>("https://asb2m10.github.io/dexed/", juce::URL("https://asb2m10.github.io/dexed/"))),
        surge(std::make_unique<juce::HyperlinkButton>("https://surge-synthesizer.github.io/", juce::URL("https://surge-synthesizer.github.io/")))
    {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        logo_png = juce::ImageCache::getFromMemory(BinaryData::dexedlogo_png, BinaryData::dexedlogo_pngSize);
        setSize(logo_png.getWidth() + 8, 500);
        //centreAroundComponent(parent, getWidth(), getHeight());

        dexed->setColour(juce::HyperlinkButton::ColourIds::textColourId, juce::Colour(0xFF4ea097));
        dexed->setJustificationType(juce::Justification::left);
        dexed->setBounds(18, 433, getWidth() - 36, 30);

        surge->setColour(juce::HyperlinkButton::ColourIds::textColourId, juce::Colour(0xFF4ea097));
        surge->setJustificationType(juce::Justification::left);
        surge->setBounds(18, 458, getWidth() - 36, 30);

        // create a new Component to hold ''dexed'' and ''surge'' as subcomponents
        // and set this holder Component as the content component of the DialogWindow
        Component* holder = new Component();
        holder->setSize(getWidth(), getHeight());
        holder->addAndMakeVisible((juce::Component*)dexed.get());
        holder->addAndMakeVisible((juce::Component*)surge.get());
        setContentOwned(holder, true);  // TODO: ''setContentComponent(holder, true, true);'' also worked; which is the better?
    }

    void closeButtonPressed() override {
        setVisible(false);
    }

    void paint(juce::Graphics &g) override {
        g.fillAll(juce::Colour(0xFF554F46));
        g.drawImage(logo_png, 0, 10, logo_png.getWidth(), logo_png.getHeight(),
                     0, 0, logo_png.getWidth(), logo_png.getHeight());
        g.setFont(20);
        g.setColour(juce::Colour(0xFFFFFFFF));
        const char *credits = "Version " DEXED_VERSION " build date: " __DATE__ "\n"
                            "This software is released under the GPL V3\n\n"
                            "DSP Engine: orignal project (msfa) Raph Levin, enhancements Pascal Gauthier\n"
                            "UI Programming: Pascal Gauthier\n"
                            "UI Design: AZur Studio\n\n"
                            "Credits to Surge Synthesizer Team for MPE and microtuning support\n"
                            "Credits to GitHub users: tico-tico, Sentinel77, jeremybernstein; filters based on OB-Xd";
        g.drawMultiLineText(credits, 18, 260, logo_png.getWidth()-18);
    }
};