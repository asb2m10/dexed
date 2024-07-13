/**
 *
 * Copyright (c) 2013-2018 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 *
 */

#include <inttypes.h>
#include <stdint.h>
 
#include "DXLookNFeel.h"
#include "DXComponents.h"
#include "Dexed.h"
#include "PluginProcessor.h"

#define REG_COLOUR(id, value) setColour(id, value); colourMap.set(#id, id)

Image findImage(String path) {
    Image img;
    if ( path.length() <= 3 ) 
        return img;
    //File imgFile(path);   // it might cause an assertion in juce_File.cpp
    File imgFile = File::getCurrentWorkingDirectory().getChildFile(path);
    img = ImageCache::getFromFile(imgFile);
    if (img.isNull()) {
        TRACE("img.isNull() == true, path=%s", path.toRawUTF8());
    }
    return img;
}

DXLookNFeel::DXLookNFeel() {
    Colour ctrlBackground;
    
    DexedAudioProcessor::dexedAppDir.setAsCurrentWorkingDirectory();
    ctrlBackground = Colour(20,18,18);

    // WARNING! If you modify the colour IDs here, please actualize the file ''DexedTheme.md'' 
    // in the subdirectory ``~/dexed/Documentation/``
    REG_COLOUR(TextButton::buttonColourId,Colour(0xFF0FC00F));
    REG_COLOUR(TextButton::textColourOnId, Colours::white);
    REG_COLOUR(TextButton::textColourOffId, Colours::white);
    REG_COLOUR(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
    REG_COLOUR(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
    REG_COLOUR(AlertWindow::backgroundColourId,lightBackground);
    REG_COLOUR(AlertWindow::textColourId, Colours::white);
    REG_COLOUR(TextEditor::backgroundColourId,ctrlBackground);
    REG_COLOUR(TextEditor::textColourId, Colours::white);
    REG_COLOUR(TextEditor::highlightColourId, fillColour);
    REG_COLOUR(TextEditor::outlineColourId, Colours::transparentBlack);
    REG_COLOUR(ComboBox::backgroundColourId, ctrlBackground);
    REG_COLOUR(ComboBox::textColourId, Colours::white);
    REG_COLOUR(ComboBox::buttonColourId, Colours::white);
    REG_COLOUR(PopupMenu::backgroundColourId, background);
    REG_COLOUR(PopupMenu::textColourId, Colours::white);
    REG_COLOUR(PopupMenu::highlightedTextColourId, Colours::white);
    REG_COLOUR(PopupMenu::highlightedBackgroundColourId, fillColour);
    REG_COLOUR(TreeView::backgroundColourId, background);
    REG_COLOUR(DirectoryContentsDisplayComponent::highlightColourId, fillColour);
    REG_COLOUR(DirectoryContentsDisplayComponent::textColourId, Colours::white);
    
    // Register ``Scrollbar::thumbColourId`` to allow its redefinion in ``DexedTheme.xml``.
    REG_COLOUR(ScrollBar::thumbColourId, background.darker());

    // WARNING! If you modify the images here, please actualize the file ''DexedTheme.md'' 
    // in the subdirectory ``~/dexed/Documentation/``
    imageKnob = ImageCache::getFromMemory(BinaryData::Knob_68x68_png, BinaryData::Knob_68x68_pngSize); // 2x
    imageSwitch = ImageCache::getFromMemory(BinaryData::Switch_96x52_png, BinaryData::Switch_96x52_pngSize); // 2x
    imageSwitchLighted = ImageCache::getFromMemory(BinaryData::SwitchLighted_48x26_png, BinaryData::SwitchLighted_48x26_pngSize);
    imageSwitchOperator = ImageCache::getFromMemory(BinaryData::Switch_64x64_png, BinaryData::Switch_64x64_pngSize); // 2x
    imageButton = ImageCache::getFromMemory(BinaryData::ButtonUnlabeled_50x30_png, BinaryData::ButtonUnlabeled_50x30_pngSize);
    imageSlider = ImageCache::getFromMemory(BinaryData::Slider_52x52_png, BinaryData::Slider_52x52_pngSize); // 2x
    imageScaling = ImageCache::getFromMemory(BinaryData::Scaling_36_26_png, BinaryData::Scaling_36_26_pngSize); // 2x
    imageLight = ImageCache::getFromMemory(BinaryData::Light_28x28_png, BinaryData::Light_28x28_pngSize); // 2x
    imageLFO = ImageCache::getFromMemory(BinaryData::LFO_36_26_png, BinaryData::LFO_36_26_pngSize);
    imageOperator =  ImageCache::getFromMemory(BinaryData::OperatorEditor_574x436_png, BinaryData::OperatorEditor_574x436_pngSize); // 2x
    imageGlobal = ImageCache::getFromMemory (BinaryData::GlobalEditor_1728x288_png, BinaryData::GlobalEditor_1728x288_pngSize); // 2x

    //---
    // load and parse the file ``DexedTheme.xml``
    //---

    File dexedTheme = DexedAudioProcessor::dexedAppDir.getChildFile("DexedTheme.xml");

    if ( ! dexedTheme.existsAsFile() ) {        
        TRACE("no DexedTheme.xml found at %s", dexedTheme.getFullPathName().toRawUTF8());
        return;
    }
    
    std::unique_ptr<XmlElement> root = XmlDocument::parse(dexedTheme);
    if ( root == NULL )
    {
        TRACE("DXLookNFeel(): ERROR: XmlDocument::parse(): failed");
        return;
    }

    //--- 
    // get custom colors from ``DexedTheme.xml``
    // specified by the ``colour id`` / ``value`` pairs
    //---

    forEachXmlChildElementWithTagName(*root, colour, "colour") {
        String name = colour->getStringAttribute("id", "");
        if ( name == "" )
            continue;
        String value = colour->getStringAttribute("value", "");
        if ( value == "" )
            continue;
        if ( value.length() < 8 ) {
            TRACE("ERROR: illegal value=\"%s\" at color id=\"%s\"; skipped", value.toRawUTF8(), name.toRawUTF8());
            continue;
        }
        //int conv = strtol(value.toRawUTF8(), NULL, 16); // as alpha (MSB) could be above 0x7F, hence ``strtol()`` is inappropiate to convert values exceeding ``0x7FFFFFFF`` 
        char* endptr = NULL;
        uint64_t conv = strtoull(value.toRawUTF8(), &endptr, 16);
        TRACE("color id=\"%s\" value=\"%s\": conv=0x%" PRIx64 "", name.toRawUTF8(), value.toRawUTF8(), conv);
        if (endptr != nullptr && *endptr != '\0') {
            TRACE("ERROR: illegal char #%d in value=\"%s\" at color id=\"%s\"; skipped", (int)(*endptr), value.toRawUTF8(), name.toRawUTF8());
            continue;
        }
        if (conv > 0xFFFFFFFFULL) {            
            TRACE("ERROR: value 0x%" PRIx64 " exceeded the limit at color id=\"%s\"; skipped", conv, name.toRawUTF8());
            continue;
        }
        if ( colourMap.contains(name) ) {
            setColour(colourMap[name], Colour((uint32_t)conv));
        } else {
            if ( name == "Dexed::backgroundId" ) {
                background = Colour((uint32_t)conv);
                continue;
            }
            else if ( name == "Dexed::fillColourId" ) {
                fillColour = Colour((uint32_t)conv);
                continue;
            }
            TRACE("ERROR: color id=\"%s\" not found in colourMap; skipped.", name.toRawUTF8());
        }
    }

    //---
    // get custom images from ``DexedTheme.xml``
    // specified by the ``image id`` / ``path`` pairs
    //---

    forEachXmlChildElementWithTagName(*root, image, "image") {
        String name = image->getStringAttribute("id", "");
        String path = image->getStringAttribute("path", "");
        //TRACE("image id=\'%s\' path=\'%s\'", name.toRawUTF8(), path.toRawUTF8());
        if ( name == /*"Knob_34x34.png"*/"Knob_68x68.png" ) { // 2x
            imageKnob = findImage(path);
            continue;
        }
        if ( name == /*"Switch_48x26.png"*/ "Switch_96x52.png" ) { // 2x
            imageSwitch = findImage(path);
            continue;
        }
        if ( name == "SwitchLighted_48x26.png" ) {
            imageSwitchLighted = findImage(path);
            continue;
        }
        if ( name == /*"Switch_32x64.png"*/ "Switch_64x64.png" ) { // 2x
            imageSwitchOperator = findImage(path);
            continue;
        }
        if ( name == "ButtonUnlabeled_50x30.png" ) {
            imageButton = findImage(path);
            continue;
        }
        if ( name == /*"Slider_26x26.png"*/ "Slider_52x52.png" ) { // 2x
            imageSlider = findImage(path);
            continue;
        }
        if ( name == "Scaling_36_26.png" ) {
            imageScaling = findImage(path);
            continue;
        }
        if ( name == /*"Light_14x14.png"*/ "Light_28x28.png" ) { // 2x
            imageLight = findImage(path);
            continue;
        }
        if ( name == "LFO_36_26.png" ) {
            imageLFO = findImage(path);
            continue;
        }
        if ( name == /*"OperatorEditor_287x218.png"*/ "OperatorEditor_574x436_png" ) { // 2x
            imageOperator = findImage(path);
            continue;
        }
        if ( name == /*"GlobalEditor_864x144.png"*/ "GlobalEditor_1728x288_png" ) { // 2x
            imageGlobal = findImage(path);
            continue;
        }
        TRACE("ERROR: unknown image id=\"%s\"; skipped", name.toRawUTF8());
    }
}

Typeface::Ptr DXLookNFeel::getTypefaceForFont(const Font &) {
    return Typeface::createSystemTypefaceFor(BinaryData::NotoSansRegular_ttf, BinaryData::NotoSansRegular_ttfSize);
}

void DXLookNFeel::drawRotarySlider( Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
     float rotaryStartAngle, float rotaryEndAngle,  Slider &slider ) {
     if ( imageKnob.isNull() ) {
         LookAndFeel_V3::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);
         return;
     }

     const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
     const int nFrames = imageKnob.getHeight()/imageKnob.getWidth(); // number of frames for vertical film strip
     const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1

     //const float radius = jmin (width / 2.0f, height / 2.0f) ;
     const float radius = jmin(width * 0.5f, height * 0.5f);
     const float centreX = x + width * 0.5f;
     const float centreY = y + height * 0.5f;
     const float rx = centreX - radius - 1.0f;
     const float ry = centreY - radius - 1.0f;

     g.drawImage(imageKnob, (int)rx, (int)ry, 2*(int)radius, 2*(int)radius, 0, frameIdx*imageKnob.getWidth(), imageKnob.getWidth(), imageKnob.getWidth());
};

void DXLookNFeel::drawToggleButton(Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) {
    if ( imageSwitch.isNull() ) {
        LookAndFeel_V3::drawToggleButton(g, button, isMouseOverButton, isButtonDown);
        return;
    }

    // One would think there is a better way...
    auto lb = dynamic_cast<LightedToggleButton *>( &button );
    if( lb )
    {
        if( imageSwitchLighted.isNull() ) {
            LookAndFeel_V3::drawToggleButton(g, button, isMouseOverButton, isButtonDown);
            return;
        }
        g.drawImage(imageSwitchLighted, 0, 0, 48, 26, 0, button.getToggleState() ? 0 : 26, 48, 26);
    }
    else
        g.drawImage(imageSwitch, 0, 0, 48, 26, 0, button.getToggleState() ? 0 : 52, 96, 52);
    
}

void DXLookNFeel::drawButtonBackground(Graphics &g, Button &button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    if ( imageButton.isNull() ) {
        LookAndFeel_V3::drawButtonBackground(g, button, backgroundColour, isMouseOverButton, isButtonDown);
        return;
    }

    int w = button.getWidth();
    
                    //      dx,  dy,  dw,  dl,   sx, sy,                    sw, sl
    g.drawImage(imageButton, 0,   0,   3,  30,    0, isButtonDown ? 30 : 0,  3, 30);
    g.drawImage(imageButton, 3,   0, w-6,  30,    3, isButtonDown ? 30 : 0, 44, 30);
    g.drawImage(imageButton, w-3, 0,   3,  30,   47, isButtonDown ? 30 : 0, 47, 30);
}

void DXLookNFeel::drawLinearSliderBackground (Graphics&, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                              const Slider::SliderStyle st, Slider& s) {
    // NOP
}

void DXLookNFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                    float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle st, Slider& s) {
    // TODO: find out why the V4 LookNFeel doesn't call this
    // TRACE("draw slider"); 
    if ( imageSlider.isNull() ) {
        LookAndFeel_V3::drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, st, s);
        return;
    }

    int p = sliderPos - minSliderPos;
    p -= 6;
    g.drawImage(imageSlider, p, 0, 26, 26, 0, 0, 52, 52);
}

void DXLookNFeel::positionComboBoxText(ComboBox& box, Label& label) {
    ComboBox *src = &box;

    // I'm not proud of this one, but really... it must be another way to do this....
    ComboBoxImage* img = dynamic_cast<ComboBoxImage*>(src);
    if( img != 0 ) {
        return;
    }

    LookAndFeel_V3::positionComboBoxText(box, label);
}

Colour DXLookNFeel::fillColour = Colour(77,159,151);
Colour DXLookNFeel::lightBackground = Colour(78,72,63);
Colour DXLookNFeel::background = Colour(60,50,47);
Colour DXLookNFeel::roundBackground = Colour(58,52,48);
