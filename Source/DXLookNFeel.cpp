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
#include "Dexed.h"

Colour DXLookNFeel::dxDarkBrown = Colour(0xFF47260D);
Colour DXLookNFeel::dxLightBrown = Colour(0xFFA87B67);

Colour DXLookNFeel::background = Colour(60,50,47);
Colour DXLookNFeel::fillColour = Colour(77,159,151);

Colour DXLookNFeel::comboBoxBackground = Colour(20, 18, 18);

DXLookNFeel::DXLookNFeel() {
    setColour(TextButton::buttonColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
    
    defaultFont = Typeface::createSystemTypefaceFor(BinaryData::NotoSansRegular_ttf, BinaryData::NotoSansRegular_ttfSize);
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
    
//    g.drawImage(myStrip, 0, 0, button.getWidth(), button.getHeight(), 0, isButtonDown ? 30 : 0, 50, 30);

    int w = button.getWidth();
    
                //      dx,  dy,  dw,  dl,   sx, sy,                               sw, sl
    g.drawImage(myStrip, 0,   0,   3,  30,    0, button.getToggleState() ? 0 : 26,  3, 30);
    g.drawImage(myStrip, 3,   0, w-3,  30,    3, button.getToggleState() ? 0 : 26, 44, 30);
    g.drawImage(myStrip, w-3, 0,   w,  30,   47, button.getToggleState() ? 0 : 26, 47, 30);

}

void DXLookNFeel::drawLinearSliderBackground (Graphics&, int x, int y, int width, int height,
                                         float sliderPos, float minSliderPos, float maxSliderPos,
                                              const Slider::SliderStyle, Slider&) {
}

void DXLookNFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                    float sliderPos, float minSliderPos, float maxSliderPos,
                                         const Slider::SliderStyle, Slider&) {
    Image myStrip = ImageCache::getFromMemory(BinaryData::Slider_26x26_png, BinaryData::Slider_26x26_pngSize);
    
    TRACE("%g %g", sliderPos, minSliderPos);
    int p = sliderPos - minSliderPos;
    
    p -= 7;
    
    g.drawImage(myStrip, p, 0, 26, 26, 0, 0, 26, 26);
}




