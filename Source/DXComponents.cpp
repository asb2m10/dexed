/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
 *
 * DX7 Envelope Tables from legasynth-0.4.1 / DX7 Patch Editor
 * Copyright (C) 2002 Juan Linietsky <coding@reduz.com.ar>
 * Copyright (C) 2006 Mark-André Hopf <mhopf@mark13.org>
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

static float EG_rate_rise_duration[128] = {
    38.00000 ,34.96000 ,31.92000 ,28.88000 ,25.84000 ,
    22.80000 ,20.64000 ,18.48000 ,16.32000 ,14.16000 ,
    12.00000 ,11.10000 ,10.20000 ,9.30000 ,8.40000 ,
    7.50000 ,6.96000 ,6.42000 ,5.88000 ,5.34000 ,
    4.80000 ,4.38000 ,3.96000 ,3.54000 ,3.12000 ,
    2.70000 ,2.52000 ,2.34000 ,2.16000 ,1.98000 ,
    1.80000 ,1.70000 ,1.60000 ,1.50000 ,1.40000 ,
    1.30000 ,1.22962 ,1.15925 ,1.08887 ,1.01850 ,
    0.94813 ,0.87775 ,0.80737 ,0.73700 ,0.69633 ,
    0.65567 ,0.61500 ,0.57833 ,0.54167 ,0.50500 ,
    0.47300 ,0.44100 ,0.40900 ,0.37967 ,0.35033 ,
    0.32100 ,0.28083 ,0.24067 ,0.20050 ,0.16033 ,
    0.12017 ,0.08000 ,0.07583 ,0.07167 ,0.06750 ,
    0.06333 ,0.05917 ,0.05500 ,0.04350 ,0.03200 ,
    0.02933 ,0.02667 ,0.02400 ,0.02200 ,0.02000 ,
    0.01800 ,0.01667 ,0.01533 ,0.01400 ,0.01300 ,
    0.01200 ,0.01100 ,0.01000 ,0.00900 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00767 ,0.00733 ,
    0.00700 ,0.00633 ,0.00567 ,0.00500 ,0.00433 ,
    0.00367 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300
};

static float EG_rate_decay_duration[128] = {
    318.00000 ,283.75000 ,249.50000 ,215.25000 ,181.00000 ,
    167.80000 ,154.60001 ,141.39999 ,128.20000 ,115.00000 ,
    104.60000 ,94.20000 ,83.80000 ,73.40000 ,63.00000 ,
    58.34000 ,53.68000 ,49.02000 ,44.36000 ,39.70000 ,
    35.76000 ,31.82000 ,27.88000 ,23.94000 ,20.00000 ,
    18.24000 ,16.48000 ,14.72000 ,12.96000 ,11.20000 ,
    10.36000 ,9.52000 ,8.68000 ,7.84000 ,7.00000 ,
    6.83250 ,6.66500 ,6.49750 ,6.33000 ,6.16250 ,
    5.99500 ,5.82750 ,5.66000 ,5.10000 ,4.54000 ,
    3.98000 ,3.64833 ,3.31667 ,2.98500 ,2.65333 ,
    2.32167 ,1.99000 ,1.77333 ,1.55667 ,1.34000 ,
    1.22333 ,1.10667 ,0.99000 ,0.89667 ,0.80333 ,
    0.71000 ,0.65000 ,0.59000 ,0.53000 ,0.47000 ,
    0.41000 ,0.32333 ,0.23667 ,0.15000 ,0.12700 ,
    0.10400 ,0.08100 ,0.07667 ,0.07233 ,0.06800 ,
    0.06100 ,0.05400 ,0.04700 ,0.04367 ,0.04033 ,
    0.03700 ,0.03300 ,0.02900 ,0.02500 ,0.02333 ,
    0.02167 ,0.02000 ,0.01767 ,0.01533 ,0.01300 ,
    0.01133 ,0.00967 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800
};

static float EG_rate_decay_percent[128] = {
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00501 ,0.01001 ,0.01500 ,
    0.02000 ,0.02800 ,0.03600 ,0.04400 ,0.05200 ,
    0.06000 ,0.06800 ,0.07600 ,0.08400 ,0.09200 ,
    0.10000 ,0.10800 ,0.11600 ,0.12400 ,0.13200 ,
    0.14000 ,0.15000 ,0.16000 ,0.17000 ,0.18000 ,
    0.19000 ,0.20000 ,0.21000 ,0.22000 ,0.23000 ,
    0.24000 ,0.25100 ,0.26200 ,0.27300 ,0.28400 ,
    0.29500 ,0.30600 ,0.31700 ,0.32800 ,0.33900 ,
    0.35000 ,0.36500 ,0.38000 ,0.39500 ,0.41000 ,
    0.42500 ,0.44000 ,0.45500 ,0.47000 ,0.48500 ,
    0.50000 ,0.52000 ,0.54000 ,0.56000 ,0.58000 ,
    0.60000 ,0.62000 ,0.64000 ,0.66000 ,0.68000 ,
    0.70000 ,0.73200 ,0.76400 ,0.79600 ,0.82800 ,
    0.86000 ,0.89500 ,0.93000 ,0.96500 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000
    
};

static float EG_rate_rise_percent[128] = {
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00501 ,0.01001 ,0.01500 ,
    0.02000 ,0.02800 ,0.03600 ,0.04400 ,0.05200 ,
    0.06000 ,0.06800 ,0.07600 ,0.08400 ,0.09200 ,
    0.10000 ,0.10800 ,0.11600 ,0.12400 ,0.13200 ,
    0.14000 ,0.15000 ,0.16000 ,0.17000 ,0.18000 ,
    0.19000 ,0.20000 ,0.21000 ,0.22000 ,0.23000 ,
    0.24000 ,0.25100 ,0.26200 ,0.27300 ,0.28400 ,
    0.29500 ,0.30600 ,0.31700 ,0.32800 ,0.33900 ,
    0.35000 ,0.36500 ,0.38000 ,0.39500 ,0.41000 ,
    0.42500 ,0.44000 ,0.45500 ,0.47000 ,0.48500 ,
    0.50000 ,0.52000 ,0.54000 ,0.56000 ,0.58000 ,
    0.60000 ,0.62000 ,0.64000 ,0.66000 ,0.68000 ,
    0.70000 ,0.73200 ,0.76400 ,0.79600 ,0.82800 ,
    0.86000 ,0.89500 ,0.93000 ,0.96500 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000
};

static double getDuration(int p_rate, int p_level_l, int p_level_r) {
    float *duration_table=(p_level_r>p_level_l) ? EG_rate_rise_duration : EG_rate_decay_duration;
    double duration=duration_table[p_rate];
    
    float* percent_table =(p_level_r>p_level_l) ? EG_rate_rise_percent  : EG_rate_decay_percent;
    duration *= fabs(percent_table[p_level_r]-percent_table[p_level_l]);
    return duration;
}

EnvDisplay::EnvDisplay() {
    pvalues = (char *) &TMP_LEVEL_PTR;
}

void EnvDisplay::paint(Graphics &g) {
    g.setColour(Colours::black.withAlpha(0.5f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);
    
    int h = getHeight();
    char *rates = pvalues;
    char *levels = pvalues + 4;
    
    double d[4];
    double keyoff = 0.0;
    double release = 0.0;
    d[0] = getDuration(rates[0], levels[3], levels[0]);
    d[1] = getDuration(rates[1], levels[0], levels[1]);
    d[2] = getDuration(rates[2], levels[1], levels[2]);
    
    double ko = 0.0;
    for(int j=0; j<3; ++j)
        ko += d[j];
    if (ko>keyoff)
        keyoff=ko;
    
    d[3] = getDuration(rates[3], levels[2], levels[3]);
    if ( d[3]>release ) {
        release = d[3];
    }
    keyoff += 10.0;
    double w = getWidth() / (keyoff + release);

    g.setColour(Colour(0xF0000000));
    g.fillRoundedRectangle (keyoff*w, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);
    g.setColour(Colours::white);

    int x, y;
    
    Path p;
    p.startNewSubPath(0, 32);
    
    // 0
    x = 0;
    y = h - h / 99.0 * levels[3];
    p.lineTo(x, y);
    if ( vPos == 0 || vPos == 1 ) {
        g.fillEllipse(x-2, y-2, 4, 4);
    }
    
    // 1
    x = d[0]*w;
    y = h - h / 99.0 * levels[0];
    p.lineTo( x, y );
    if ( vPos == 1 || vPos == 2 ) {
        g.fillEllipse(x-2, y-2, 4, 4);
    }
    
    // 2
    x = (d[0]+d[1])*w;
    y = h - h / 99.0 * levels[1];
    p.lineTo( x, y );
    if ( vPos == 2 || vPos == 3 ) {
        g.fillEllipse(x-2, y-2, 4, 4);
    }
    
    // 3
    x = (d[0]+d[1]+d[2])*w;
    y = h - h / 99.0 * levels[2];
    p.lineTo( x, y );
    if ( vPos == 3 || vPos == 4) {
        g.fillEllipse(x-2, y-2, 4, 4);
    }
    
    // 4
    x = keyoff*w;
    y = h - h / 99.0 * levels[2];
    p.lineTo( x, y );
    if ( vPos == 4 ) {
        g.fillEllipse(x-2, y-2, 4, 4);
    }
    
    // 5
    p.lineTo( (d[0]+d[1]+d[2]+keyoff+d[3])*w, h - h / 99.0 * levels[3] );
    
    p.lineTo(96,32);
    p.lineTo(0, 32);

    g.setColour(Colour(0xFF0FC00F).withAlpha(0.3f));
    g.fillPath(p);
    
    g.setColour(Colour(0xFFFFFFFF));
    String len;
    len << ((int) vPos);
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
    float total = 0;
    
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

VuMeter::VuMeter() {
    totalBlocks = 16;
}

void VuMeter::paint(Graphics &g) {
    // taken from the drawLevelMeter ;
    float width = getWidth();
    float height = getHeight();
    
    g.setColour (Colours::black);
    g.fillRoundedRectangle (0.0f, 0.0f, (float)  width, (float) height, 0);

    const int numBlocks = roundToInt (totalBlocks * v);
    const float h = (height - 6.0f) / (float) totalBlocks;
    
    for (int i = 0; i < totalBlocks; ++i) {
        if (i >= numBlocks)
            g.setColour (Colours::red.withAlpha (0.2f));
        else
            g.setColour (Colours::red);
        g.fillRoundedRectangle (3.0f, (height-4.0f) - (3.0f + i * h + h * 0.1f) , width - 6.0f, h * 0.8f, 0);
    }
}

LcdDisplay::LcdDisplay() {
    systemMsg << "*** DEXED FM synthesizer ***";
}

void LcdDisplay::timerCallback() {
    systemMsg = "*** DEXED FM synthesizer ***";
    repaint();
    stopTimer();
}

void LcdDisplay::setSystemMsg(String msg) {
    systemMsg = msg;
    triggerAsyncUpdate();
}

void LcdDisplay::handleAsyncUpdate() {
    repaint();
    startTimer(5000);
}

#ifdef _WIN32 
    const float LCD_FONTSIZE = 13.0f;
#else 
    const float LCD_FONTSIZE = 15.0f;
#endif

void LcdDisplay::paint(Graphics &g) {

    g.setColour(Colours::black.withAlpha(0.4f));
    g.fillRoundedRectangle (0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 1.0f);
    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultMonospacedFontName(), LCD_FONTSIZE, Font::plain));
    g.drawText (systemMsg,
                7, 4, 300, 8,
                Justification::centredLeft, true);
    g.drawText (paramMsg,
                7, 20, 300, 8,
                Justification::centredLeft, true);
}
