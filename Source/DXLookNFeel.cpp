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
 
#include "DXLookNFeel.h"
#include "PluginProcessor.h"

/**
 * Algorithm arrangements, based on the DX1 display.
 * Feedop 7 == custom draw
 */
const char algoArr[][14] = {
    // 1  2  3  4  5  6  7  8  9  A  B  C  D  F
    {  0, 0, 1, 3, 0, 0, 0, 2, 4, 0, 0, 5, 6, 6 },  // 1
    {  0, 0, 1, 3, 0, 0, 0, 2, 4, 0, 0, 5, 6, 2 },  // 2
    {  0, 0, 1, 4, 0, 0, 0, 2, 5, 0, 3, 6, 0, 6 },  // 3
    {  0, 0, 1, 4, 0, 0, 0, 2, 5, 0, 3, 6, 0, 7 },  // 4
    {  0, 1, 3, 5, 0, 0, 0, 2, 4, 6, 0, 0, 0, 6 },  // 5
    {  0, 1, 3, 5, 0, 0, 0, 2, 4, 6, 0, 0, 0, 7 },  // 6
    {  0, 1, 3, 0, 0, 0, 2, 4, 5, 0, 0, 6, 0, 2 },  // 7
    {  0, 1, 3, 0, 0, 0, 2, 4, 5, 0, 0, 6, 0, 4 },  // 8
    {  0, 1, 3, 0, 0, 0, 2, 4, 5, 0, 0, 0, 0, 2 },  // 9
    {  0, 0, 0, 4, 1, 0, 5, 6, 2, 0, 0, 3, 0, 3 },  // 10
    {  0, 0, 0, 4, 1, 0, 5, 6, 2, 0, 0, 3, 0, 6 },  // 11
    {  0, 0, 3, 0, 1, 0, 4, 5, 6, 2, 0, 0, 0, 2 },  // 12
    {  0, 0, 3, 0, 1, 0, 4, 5, 6, 2, 0, 0, 0, 6 },  // 13
    // 1  2  3  4  5  6  7  8  9  A  B  C  D  F
    {  0, 0, 1, 3, 0, 0, 0, 2, 4, 0, 5, 6, 0, 6 },  // 14
    {  0, 0, 1, 3, 0, 0, 0, 2, 4, 0, 5, 6, 0, 2 },  // 15
    {  0, 0, 1, 0, 0, 0, 2, 3, 5, 0, 4, 6, 0, 6 },  // 16
    {  0, 0, 1, 0, 0, 0, 2, 3, 5, 0, 4, 6, 0, 2 },  // 17
    {  0, 0, 1, 0, 0, 0, 2, 3, 4, 0, 0, 5, 6, 3 },  // 18
    {  0, 0, 1, 4, 5, 0, 0, 2, 6, 0, 3, 0, 0, 6 },  // 19
    {  0, 1, 2, 0, 4, 0, 3, 0, 5, 6, 0, 0, 0, 3 },  // 20
    {  0, 1, 2, 4, 5, 0, 3, 0, 6, 0, 0, 0, 0, 3 },  // 21
    {  0, 1, 3, 4, 5, 0, 2, 0, 6, 0, 0, 0, 0, 6 },  // 22
    {  0, 1, 2, 4, 5, 0, 0, 3, 6, 0, 0, 0, 0, 6 },  // 23
    {  1, 2, 3, 4, 5, 0, 0, 0, 6, 0, 0, 0, 0, 6 },  // 24
    {  1, 2, 3, 4, 5, 0, 0, 0, 6, 0, 0, 0, 0, 6 },  // 25
    {  0, 1, 2, 0, 4, 0, 0, 3, 5, 6, 0, 0, 0, 6 },  // 26
    // 1  2  3  4  5  6  7  8  9  A  B  C  D   F
    {  0, 1, 2, 0, 4, 0, 0, 3, 5, 6, 0, 0, 0, 3 },  // 27
    {  0, 1, 3, 6, 0, 0, 2, 4, 0, 0, 5, 0, 0, 5 },  // 28
    {  0, 1, 2, 3, 5, 0, 0, 0, 4, 6, 0, 0, 0, 6 },  // 29
    {  0, 1, 2, 3, 6, 0, 0, 0, 4, 0, 0, 5, 0, 5 },  // 30
    {  1, 2, 3, 4, 5, 0, 0, 0, 0, 6, 0, 0, 0, 6 },  // 31
    {  1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 6 }   // 32
};

void EnvDisplay::paint(Graphics &g) {
    int rate[4];
    int level[4];

    for (int i = 0; i < 4; i++) {
        rate[i] = s_rate[i]->getValue();
        level[i] = s_level[i]->getValue();
    }

    env.init(rate, level, 99 << 5, 0);
    env.keydown(true);
    for (int i = 0; i < 72; i++) {
        int32_t pos = env.getsample();
        for (int j = 0; j < 16; j++) {
            env.getsample();
        }
        g.setPixel(i, 32 - (sqrt(pos) / 512));
    }
    env.keydown(false);
    for (int i = 0; i < 24; i++) {
        int32_t pos = env.getsample();
        for (int j = 0; j < 16; j++) {
            env.getsample();
        }
        g.setPixel(i + 72, 32 - (sqrt(pos) / 512));
    }
}

AlgoDisplay::AlgoDisplay() {
    algo = NULL;
}

/**
 * For now, this is hardcoded 126x56 (21x14 each)
 */
void AlgoDisplay::paint(Graphics &g) {
    int alg;

    if ( algo == NULL ) {
        return;
    }
    
    if ( *algo <= 31 ) { 
        alg = *algo;
    } else {
        alg = 31;
    }
    const char *arr = algoArr[alg];
    
    for(int i=0;i<13;i++) {
        char target = arr[i];
        bool feedback = target == arr[13];
        if ( target == 0 )
            continue;
        
        if ( i < 6 ) {
            drawOp(g, i, 3, target, feedback);
            continue;
        }
        
        if ( i < 10 ){
            drawOp(g, (i-6)+1, 2, target, feedback);
            continue;
        }
        
        if ( i < 12 ) {
            drawOp(g, (i-10)+2, 1, target, feedback);
            continue;
        }
    
        // last one
        drawOp(g, (i-12)+3, 0, target, feedback);
    }

}

void AlgoDisplay::drawOp(Graphics &g, int x, int y, int num, bool feedback) {
    String txt;
    txt << num;

    g.setColour(Colour(0xFF0FC00F));
    g.fillRect(x*21+1, y*14+1, 19, 13);
    g.setColour(Colour(0xFFFFFFFF));
    g.drawText(txt, x*21, y*14, 21, 14, Justification::centred, true);
    if ( feedback ) {
        g.setColour(Colour(0xFFFFFFFF));
        int x1 = (x*21) + 19;
        g.drawLine(x1, y*14+1, x1, y*14+14, 3);
    }
}

void VuMeter::paint(Graphics &g) {
    
    // taken from the drawLevelMeter ; 
    float width = getWidth();
    float height = getHeight();
    
    g.setColour (Colours::black);
    g.fillRoundedRectangle (0.0f, 0.0f, (float)  width, (float) height, 0);
    /*g.setColour (Colours::black.withAlpha (0.2f));
    g.drawRoundedRectangle (1.0f, 1.0f, width - 2.0f, height - 2.0f, 3.0f, 1.0f);*/
    
    const int totalBlocks = 16;
    const int numBlocks = roundToInt (totalBlocks * v);
    const float h = (height - 6.0f) / (float) totalBlocks;
    
    for (int i = 0; i < totalBlocks; ++i) {
        g.setColour (Colours::red);
        if (i >= numBlocks)
            g.setColour (Colours::red.withAlpha (0.2f));
        else
            g.setColour (Colours::red);
        //g.fillRoundedRectangle (3.0f + i * w + w * 0.1f, 3.0f, w * 0.8f, height - 6.0f, w * 0.4f);
        
        g.fillRoundedRectangle (3.0f, (height-3.0f) - (3.0f + i * h + h * 0.1f) , width - 6.0f, h * 0.8f, 0);
    }
}

DXLookNFeel::DXLookNFeel() {
    setColour(TextButton::buttonColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
    setColour(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
}
