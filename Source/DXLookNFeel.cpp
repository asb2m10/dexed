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

Colour DXLookNFeel::dxDarkBrown = Colour(0xFF47260D);
Colour DXLookNFeel::dxLightBrown = Colour(0xFFA87B67);

Colour DXLookNFeel::background = Colour(60,50,47);
Colour DXLookNFeel::fillColour = Colour(77,159,151);

DXLookNFeel::DXLookNFeel() {
    setColour(TextButton::buttonColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
}



void DXLookNFeel::drawRotarySlider( Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
     float rotaryStartAngle, float rotaryEndAngle,  Slider &slider ) {
     Image myStrip = ImageCache::getFromMemory (BinaryData::knobstrip_png, BinaryData::knobstrip_pngSize);
        
        
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
