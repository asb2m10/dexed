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
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#ifndef PLUGINPARAM_H_INCLUDED
#define PLUGINPARAM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DexedAudioProcessor;

class Ctrl : public Slider::Listener, public Button::Listener, public ComboBox::Listener, public MouseListener {
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

    // use this to signal a parameter change to the host
    void publishValue(float value);
    
    // use this to publish a new value to be computed on the event thread
    void publishValueAsync(float value);
    
    /**
     * Host value is related 0.0 to 1.0 values
     */
    virtual void setValueHost(float f) = 0;
    virtual float getValueHost() = 0;
    virtual String getValueDisplay() = 0;
    virtual void updateComponent() = 0;

    void comboBoxChanged (ComboBox* combo);
    void sliderValueChanged (Slider* moved);
    void buttonClicked (Button* buttonThatWasClicked);
    void mouseEnter(const MouseEvent &event);
    void mouseDown(const MouseEvent &event);
    virtual void updateDisplayName();
    
    /**
     * Index of this parameter
     */
    int idx;
    DexedAudioProcessor *parent;
};

class CtrlFloat : public Ctrl {
	float *vPointer;
public:

	CtrlFloat(String name, float *storageValue);
	void setValueHost(float f);
	float getValueHost();
	String getValueDisplay();
    void updateComponent();
};

// CtrlDX is a controller that is related to DX parameters
class CtrlDX : public Ctrl {
    int dxValue;
    int steps;
    int dxOffset;
    int displayValue;
    
public:
    CtrlDX(String name, int steps, int offset = -1, int displayValue = 0);
    void setValueHost(float f);
    float getValueHost();
    void publishValue(float value);
    
    void setValue(int value);
    int getValue();
    int getOffset();
    String getValueDisplay();
    
    void sliderValueChanged (Slider* moved);
    void comboBoxChanged (ComboBox* combo);
    void buttonClicked (Button* buttonThatWasClicked);
    void updateComponent();
    
    void updateDisplayName();
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
    ScopedPointer<Ctrl> opSwitch;
};

#endif  // PLUGINPARAM_H_INCLUDED
