/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#ifndef DXCOMPONENTS_H_INCLUDED
#define DXCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class EnvDisplay : public Component {
public:
    EnvDisplay();
    char *pvalues;
    char vPos;    
    void paint(Graphics &g);
};

class PitchEnvDisplay : public Component {
    char rvalues[8];
public:
    PitchEnvDisplay();
    char *pvalues;
    char vPos;
    void paint(Graphics &g);
};

class AlgoDisplay : public Component {
    void drawOp(Graphics &g, int x, int y, int num);
public:
	AlgoDisplay();
    char *algo;
	void paint(Graphics &g);
};

class VuMeter: public Component {
    void paint(Graphics &g);
public :
    VuMeter();    
    float v;
    int totalBlocks;
};

class LcdDisplay : public Component, public Timer, public AsyncUpdater {
    void timerCallback();
    void handleAsyncUpdate();
public:
    LcdDisplay();
    void setSystemMsg(String msg);
    String systemMsg;
    String paramMsg;
    void paint(Graphics &g);    
};

#endif  // DXCOMPONENTS_H_INCLUDED
