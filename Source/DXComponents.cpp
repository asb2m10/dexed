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

#include "DXComponents.h"
#include "PluginProcessor.h"
#include "msfa/pitchenv.h"

/**
 * Algorithm arrangements, based on the DX1 display.
 */
static const char algoArr[][13] = {
    // 1  2  3  4  5   6   7   8   9   A   B   C   D
    {  0, 0, 1, 3, 0,  0,  0,  2,  4,  0,  0,  5, -6 },  // 1
    {  0, 0, 1, 3, 0,  0,  0, -2,  4,  0,  0,  5,  6 },  // 2
    {  0, 0, 1, 4, 0,  0,  0,  2,  5,  0,  3, -6,  0 },  // 3
    {  0, 0, 1, -4, 0,  0,  0,  2,  -5, 0, 3, -6,  0 },  // 4
    {  0, 1, 3, 5, 0,  0,  2,   4,  -6, 0, 0,  0,  0 },  // 5
    {  0, 1, 3, -5, 0,  0, 2,   4,  -6, 0, 0,  0,  0 },  // 6
    {  0, 1, 3, 0, 0,  0,  2,  4,  5,  0,  0, -6,  0 },  // 7
    {  0, 1, 3, 0, 0,  0,  2, -4,  5,  0,  0,  6,  0 },  // 8
    {  0, 1, 3, 0, 0,  0, -2,  4,  5,  0,  0,  6,  0 },  // 9
    {  0, 0, 4, 1, 0,  0,  5,  6,  2,  0,  0, -3,  0 },  // 10
    {  0, 0, 4, 1, 0,  0,  5, -6,  2,  0,  0,  3,  0 },  // 11
    {  0, 0, 3, 0, 1,  0,  4,  5,  6, -2,  0,  0,  0 },  // 12
    {  0, 0, 3, 0, 1,  0,  4,  5, -6,  2,  0,  0,  0 },  // 13
    // 1  2  3  4  5   6   7   8   9   A   B   C   D
    {  0, 0, 1, 3, 0,  0,  0,  2,  4,  0,  5, -6,  0 },  // 14
    {  0, 0, 1, 3, 0,  0,  0, -2,  4,  0,  5,  6,  0 },  // 15
    {  0, 0, 1, 0, 0,  0,  2,  3,  5,  0,  4, -6,  0 },  // 16
    {  0, 0, 1, 0, 0,  0, -2,  3,  5,  0,  4,  6,  0 },  // 17
    {  0, 0, 1, 0, 0,  0,  2, -3,  4,  0,  0,  5,  6 },  // 18
    {  0, 0, 1, 4, 5,  0,  0,  2, -6,  0,  3,  0,  0 },  // 19
    {  0, 1, 2, 0, 4,  0, -3,  0,  5,  6,  0,  0,  0 },  // 20
    {  0, 1, 2, 4, 5,  0, -3,  0,  6,  0,  0,  0,  0 },  // 21
    {  0, 1, 3, 4, 5,  0,  2,  0, -6,  0,  0,  0,  0 },  // 22
    {  0, 1, 2, 4, 5,  0,  0,  3, -6,  0,  0,  0,  0 },  // 23
    {  1, 2, 3, 4, 5,  0,  0,  0, -6,  0,  0,  0,  0 },  // 24
    {  1, 2, 3, 4, 5,  0,  0,  0, -6,  0,  0,  0,  0 },  // 25
    {  0, 1, 2, 0, 4,  0,  0,  3,  5, -6,  0,  0,  0 },  // 26
    // 1  2  3  4  5   6   7   8   9   A   B   C   D
    {  0, 1, 2, 0, 4,  0,  0, -3,  5,  6,  0,  0,  0 },  // 27
    {  0, 1, 3, 6, 0,  0,  2,  4,  0,  0, -5,  0,  0 },  // 28
    {  0, 1, 2, 3, 5,  0,  0,  0,  4, -6,  0,  0,  0 },  // 29
    {  0, 1, 2, 3, 6,  0,  0,  0,  4,  0,  0, -5,  0 },  // 30
    {  1, 2, 3, 4, 5,  0,  0,  0,  0, -6,  0,  0,  0 },  // 31
    {  1, 2, 3, 4, 5, -6,  0,  0,  0,  0,  0,  0,  0 }   // 32
};

static char TMP_LEVEL_PTR[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

AlgoDisplay::AlgoDisplay() {
    static char tmpAlgo = 0;
    algo = &tmpAlgo;
}

void AlgoDisplay::paint(Graphics &g) {
    int alg;
    
    if ( *algo <= 31 ) {
        alg = *algo;
    } else {
        alg = 31;
    }
    const char *arr = algoArr[alg];
    
    g.setColour (Colours::black.withAlpha(0.1f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);
    
    for(int i=0;i<13;i++) {
        int target = arr[i];
        if ( target == 0 )
            continue;
        
        if ( i < 6 ) {
            drawOp(g, i, 3, target);
            continue;
        }
        
        if ( i < 10 ){
            drawOp(g, (i-6)+1, 2, target);
            continue;
        }
        
        if ( i < 12 ) {
            drawOp(g, (i-10)+2, 1, target);
            continue;
        }
        
        // last one
        drawOp(g, (i-12)+3, 0, target);
    }
    
    String algoTxt;
    algoTxt << (alg+1);
    g.drawText(algoTxt, 5, 1, 21, 14, Justification::left, true);
}

void AlgoDisplay::drawOp(Graphics &g, int x, int y, int num) {
    String txt;
    txt << abs(num);
    
    int offx = 24;
    int offy = 17;
    
    g.setColour(Colour(0xFF0FC00F));
    g.fillRect(x*offx+4, y*offy+3, offx-2, offy-1);
    g.setColour(Colour(0xFFFFFFFF));
    g.drawText(txt, x*offx+3, y*offy+2, offx+2, offy+2, Justification::centred, true);
    if ( num < 0 ) {
        g.setColour(Colour(0xFFFFFFFF));
        int x1 = (x*offx) + 24;
        g.drawLine(x1+1, y*offy+3, x1+1, y*offy+offy+2, 3);
    }
}

EnvDisplay::EnvDisplay() {
    pvalues = (char *) &TMP_LEVEL_PTR;    
}

void EnvDisplay::paint(Graphics &g) {
    g.setColour(Colours::black.withAlpha(0.5f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);

    
    char *levels = pvalues + 4;
    char *rates = pvalues;
    
    float dist[4];
    float total;
    
    int old = levels[3];
    
    for(int i=0;i<4;i++) {
        int nw = levels[i];
        dist[i] = ((float)abs(nw - old)) / rates[i] == 0 ? 1 : rates[i];
        total += dist[i];
        old = nw;
    }
    
    if ( total < 1 ) {
        dist[0] = dist[1] = dist[2] = dist[3] = 1;
        total = 4;
    }
    
    // TODO : this is WIP
    float ratio =  96 / total;
    
    int oldx = 0;
    int oldy = 32 - ((float)levels[3] / 3.125);
    Path p;
    
    p.startNewSubPath(0, 32);
    g.setColour(Colours::white);
    
    int i;
    for(i=0;i<4;i++) {
        int newx = dist[i] * ratio + oldx;
        int newy = 32 - ((float)levels[i] / 3.125);
        
        p.lineTo(newx, newy);
        if ( vPos == i ) {
            g.fillEllipse(oldx-2, oldy-2, 4, 4);
        }
        
        oldx = newx;
        oldy = newy;
    }
    p.lineTo(96,32);
    p.lineTo(0, 32);
    
    if ( vPos == i ) {
        g.fillEllipse(oldx-2, oldy-2, 4, 4);
    }

    g.setColour(Colour(0xFF0FC00F).withAlpha(0.3f));
    g.fillPath(p);
    
    g.setColour(Colour(0xFFFFFFFF));
    String len;
    len << ((int) total);
    g.drawText(len, 5, 1, 72, 14, Justification::left, true);
}


PitchEnvDisplay::PitchEnvDisplay() {
    pvalues = (char *) &TMP_LEVEL_PTR;
    vPos = 0;
}

void PitchEnvDisplay::paint(Graphics &g) {
    g.setColour(Colours::black.withAlpha(0.1f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);
    g.setColour(Colours::white);
    
    char *levels = pvalues + 4;
    char *rates = pvalues;
    
    float dist[4];
    float total;
    
    int old = pitchenv_tab[levels[3]] + 128;
    // find the scale
    
    for(int i=0;i<4;i++) {
        int nw = pitchenv_tab[levels[i]] + 128;
        dist[i] = ((float)abs(nw - old)) / pitchenv_rate[rates[i]];
        total += dist[i];
        old = nw;
    }
    
    if ( total < 0.00001 ) {
        dist[0] = dist[1] = dist[2] = dist[3] = 1;
        total = 4;
    }
        
    // TODO : this is WIP
    float ratio =  96 / total;
    
    int oldx = 0;
    int oldy = 25 - (pitchenv_tab[levels[3]] + 128) / 10;
    
    int i;
    for(i=0;i<4;i++) {
        int newx = dist[i] * ratio + oldx;
        int newy = 25 - (pitchenv_tab[levels[i]] + 128) / 10;
        
        g.drawLine(oldx, oldy, newx, newy, 2);
        
        if ( vPos == i ) {
            g.fillEllipse(oldx-2, oldy-2, 4, 4);
        }
        
        oldx = newx;
        oldy = newy;
    }
    
    if ( vPos == i ) {
        g.fillEllipse(oldx-2, oldy-2, 4, 4);
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