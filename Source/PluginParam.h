/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#ifndef PLUGINPARAM_H_INCLUDED
#define PLUGINPARAM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DexedAudioProcessor;

class Ctrl : public SliderListener, public ButtonListener, public ComboBoxListener {
protected:
    /**
     * Binded components of the UI
     */
    Slider *slider;
    Button *button;
    ComboBox *comboBox;

public:
    String label;

    Ctrl(String name);
    void bind(Slider *s);
    void bind(Button *b);
    void bind(ComboBox *c);
    void unbind();
    
    virtual void setValuePlugin(float f) = 0;
    virtual float getValuePlugin() = 0;
    virtual String getValueDisplay() = 0;
    virtual void updateComponent() = 0;

    /**
     * Index of this parameter
     */
    int idx;
    DexedAudioProcessor *parent;
};

// CtrlDX is a controller that is related to DX parameters
class CtrlDX : public Ctrl {
    int value;
    int steps;
    int add1;
    int dxOffset;

public:
    CtrlDX(String name, int steps, int offset = -1, bool starts1 = false);
    void setValuePlugin(float f);
    float getValuePlugin();
    void publishValue(int value);
    
    void setValue(int value);
    int getValue();
    String getValueDisplay();
    void sliderValueChanged (Slider* moved);
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* combo);
    void updateComponent();
};


struct OperatorCtrl {
    ScopedPointer<CtrlDX> egRate[4];
    ScopedPointer<CtrlDX> egLevel[4];
    ScopedPointer<CtrlDX> level;
    ScopedPointer<CtrlDX> opMode;
    ScopedPointer<CtrlDX> coarse;
    ScopedPointer<CtrlDX> fine;
    ScopedPointer<CtrlDX> detune;
    ScopedPointer<CtrlDX> sclBrkPt;
    ScopedPointer<CtrlDX> sclLeftDepth;
    ScopedPointer<CtrlDX> sclRightDepth;
    ScopedPointer<CtrlDX> sclLeftCurve;
    ScopedPointer<CtrlDX> sclRightCurve;
    ScopedPointer<CtrlDX> sclRate;
    ScopedPointer<CtrlDX> ampModSens;
    ScopedPointer<CtrlDX> velModSens;
};

#endif  // PLUGINPARAM_H_INCLUDED
