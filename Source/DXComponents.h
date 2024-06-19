/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#ifndef DXCOMPONENTS_H_INCLUDED
#define DXCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdint.h>

class EnvDisplay : public Component {
public:
    EnvDisplay();
    uint8_t *pvalues;
    char vPos;    
    void paint(Graphics &g);
};

class PitchEnvDisplay : public Component {
    char rvalues[8];
public:
    PitchEnvDisplay();
    uint8_t *pvalues;
    char vPos;
    void paint(Graphics &g);
};

/*
class VuMeter: public Component {
    void paint(Graphics &g);
public : 
    float v;
};
*/

class VuMeter : public juce::Component {
public:
    void paint(juce::Graphics& g);

    // image of the stripe
    juce::Image myStrip;

    // width of the image
    int width = 0;

    // height of the original image,
    // containing a dark and a bright stripes
    int height = 0;

    // height of a single stripe (half of the height of the original stripe)
    int height2 = 0;

    // total number of blocks ("LED"s) of the stripe
    static const int totalBlocks = 46;

    // dimensions of a single block in pixels, for the case of the 140x80 sized image
    static const int blockWidth = 2;
    static const int blockGap = 1;
    /*
        // dimension of a single block in pixels, for the case of the 280x16 sized image
        static const int blockWidth = 2*2;
        static const int blockGap = 1*2;
    */

    // dimension of the border
    static const int border = 1;
    // dimension of a block plus the size of a gap between two adjacent blocks
    static const int blockWidthWithGap = blockWidth + blockGap;

    // number of block having different colors 
    static const int numRedBlocks = 6;
    static const int numYellowBlocks = 6;
    static const int numGreenBlocks = totalBlocks - numRedBlocks - numYellowBlocks;

    // array of images of the individual stripes
    static const int numStripes = totalBlocks + 1;
    juce::Image stripes[numStripes];

	VuMeter(bool tricolor, int maxdB2);

    float v = 0.0F;

    // minimum dB associated to leftmost LED, for solid stripe
    int mindB = -39;
    // minimum amplitude equivalent to mindB, for solid stripe
    float minamp = (float)pow(10.0, mindB * 0.1);
    // maximum amplitude, for solid stripe
    float maxamp = 1.0F;

    void recolorize();
    void createStripes();
    float dB_to_amp(float dB);
};

class LcdDisplay : public Component {
public:
    LcdDisplay();
    void setSystemMsg(String msg);
    String paramMsg;
    void paint(Graphics &g);    
};

class ComboBoxImage : public ComboBox {
    Image items;
    int itemHeight;
    PopupMenu popup;
    int itemPos[4];
public:
    ComboBoxImage();

    virtual void paint(Graphics &g) override;
    virtual void showPopup() override;
    void setImage(Image image);
    void setImage(Image image, int pos[]);
};

class ProgramSelector : public ComboBox {
public:
    virtual void mouseDown(const MouseEvent &event) override;
    virtual void mouseEnter(const MouseEvent &event) override { accum_wheel = 0; }
    virtual void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    virtual void paint(Graphics &g) override;

private:
    float accum_wheel;
};

#endif  // DXCOMPONENTS_H_INCLUDED
