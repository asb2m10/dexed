/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
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
 
#include "DXLookNFeel.h"
#include "DXComponents.h"
#include "Dexed.h"

Colour DXLookNFeel::background = Colour(60,50,47);
Colour DXLookNFeel::fillColour = Colour(77,159,151);
Colour DXLookNFeel::lightBackground = Colour(78,72,63);
Colour DXLookNFeel::ctrlBackground = Colour(20,18,18);
Colour DXLookNFeel::comboBoxBackground = Colour(20,18,18);

DXLookNFeel::DXLookNFeel() {
    setColour(TextButton::buttonColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
    setColour(AlertWindow::backgroundColourId,lightBackground);
    setColour(TextEditor::backgroundColourId,ctrlBackground);
    setColour(TextEditor::textColourId, Colours::white);
    setColour(TextEditor::outlineColourId, Colours::transparentBlack);
    setColour(ComboBox::backgroundColourId,ctrlBackground);
    setColour(ComboBox::textColourId, Colours::white);
    setColour(ComboBox::buttonColourId, Colours::white);
    setColour(TextButton::textColourOnId, Colours::white);
    setColour(TextButton::textColourOffId, Colours::white);
    setColour(PopupMenu::backgroundColourId, background);
    setColour(PopupMenu::textColourId, Colours::white);
    setColour(PopupMenu::highlightedTextColourId, Colours::white);
    setColour(PopupMenu::highlightedBackgroundColourId, fillColour);

    defaultFont = Typeface::createSystemTypefaceFor(BinaryData::NotoSansRegular_ttf, BinaryData::NotoSansRegular_ttfSize);
    defaultFontBold =  Typeface::createSystemTypefaceFor(BinaryData::NotoSansBold_ttf, BinaryData::NotoSansBold_ttfSize);
}

Typeface::Ptr DXLookNFeel::getTypefaceForFont(const Font &) {
    return defaultFont;
}

void DXLookNFeel::drawRotarySlider( Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
     float rotaryStartAngle, float rotaryEndAngle,  Slider &slider ) {
     Image myStrip = ImageCache::getFromMemory(BinaryData::Knob_34x34_png, BinaryData::Knob_34x34_pngSize);

     const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
     const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
     const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        
     const float radius = jmin (width / 2.0f, height / 2.0f) ;
     const float centreX = x + width * 0.5f;
     const float centreY = y + height * 0.5f;
     const float rx = centreX - radius - 1.0f;
     const float ry = centreY - radius - 1.0f;
        
     g.drawImage(myStrip, (int)rx, (int)ry, 2*(int)radius, 2*(int)radius, 0, frameIdx*myStrip.getWidth(), myStrip.getWidth(), myStrip.getWidth());
};

void DXLookNFeel::drawToggleButton(Graphics& g, ToggleButton& button, bool isMouseOverButton, bool isButtonDown) {
    Image myStrip = ImageCache::getFromMemory(BinaryData::Switch_48x26_png, BinaryData::Switch_48x26_pngSize);
    g.drawImage(myStrip, 0, 0, 48, 26, 0, button.getToggleState() ? 0 : 26, 48, 26);
}

void DXLookNFeel::drawButtonBackground(Graphics &g, Button &button, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) {
    Image myStrip = ImageCache::getFromMemory(BinaryData::ButtonUnlabeled_50x30_png, BinaryData::ButtonUnlabeled_50x30_pngSize);
    int w = button.getWidth();
    
                //      dx,  dy,  dw,  dl,   sx, sy,                    sw, sl
    g.drawImage(myStrip, 0,   0,   3,  30,    0, isButtonDown ? 30 : 0,  3, 30);
    g.drawImage(myStrip, 3,   0, w-6,  30,    3, isButtonDown ? 30 : 0, 44, 30);
    g.drawImage(myStrip, w-3, 0,   3,  30,   47, isButtonDown ? 30 : 0, 47, 30);
}

void DXLookNFeel::drawLinearSliderBackground (Graphics&, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                              const Slider::SliderStyle, Slider&) {
    // NOP
}

void DXLookNFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                    float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle, Slider&) {
    Image myStrip = ImageCache::getFromMemory(BinaryData::Slider_26x26_png, BinaryData::Slider_26x26_pngSize);
    int p = sliderPos - minSliderPos;
    p -= 6;
    g.drawImage(myStrip, p, 0, 26, 26, 0, 0, 26, 26);
}

Font DXLookNFeel::getTextButtonFont(TextButton& button, int buttonHeight) {
    return Font(defaultFontBold);
}

void DXLookNFeel::positionComboBoxText(ComboBox& box, Label& label) {
    ComboBox *src = &box;

    ComboBoxImage* img = dynamic_cast<ComboBoxImage*>(src);
    if(img != 0) { 
        return;
    }

    LookAndFeel_V3::positionComboBoxText(box, label);
}

