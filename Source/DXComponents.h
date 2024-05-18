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

// Red-strip VU Meter for Operators' outputs
class VuMeter: public Component {
    void paint(Graphics &g);
public : 
    /// Total number of blocks of the strips,
    /// according to content of ''Meter3C_140x8_png''.
    static const int totalBlocks = 46; 

    static constexpr float one_per_totalBlocks = 1.0F / totalBlocks;

    // amplitude value corresponding to -(totalBlocks) dB
    static constexpr float amp_46dB = 2.51189E-05;

    float v;
};

// Green-yellow-red-colored VU Meter for Dexed's main output
class VuMeterMain : public VuMeter {
public:
    VuMeterMain();
    void paint(Graphics& g);

    /// Number of red blocks on the three-colored strip,
    /// according to the content of ''Meter3C_140x8_png''.
    static const int numRedBlocks = 6; 

    /// Number of yellow blocks on the three-colored strip,
    /// according to the content of ''Meter3C_140x8_png''.
    static const int numYellowBlocks = 6;

    static const int numNonRedBlocks = totalBlocks - numRedBlocks;

    /// Ratio of length of the green+yellow area to total length
    /// of the three-colored strip.
    static constexpr float VU_0dB = ((float)numNonRedBlocks) / ((float)totalBlocks);

    /// Image of a three-colored (green-yellow-red) strip,
    /// loaded from ''Meter3C_140x8.png''.
    Image strip3c;
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
