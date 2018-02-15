/**
 *
 * Copyright (c) 2014-2018 Pascal Gauthier.
 *
 * DX7 Envelope Tables from legasynth-0.4.1 / DX7 Patch Editor
 * Copyright (C) 2002 Juan Linietsky <coding@reduz.com.ar>
 * Copyright (C) 2006 Mark-André Hopf <mhopf@mark13.org>
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

#include "Dexed.h"
#include "DXComponents.h"
#include "DXLookNFeel.h"
#include "PluginProcessor.h"
#include "msfa/pitchenv.h"

static char TMP_LEVEL_PTR[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

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
    pvalues = (uint8_t *) &TMP_LEVEL_PTR;
}

void EnvDisplay::paint(Graphics &g) {
    int h = getHeight();
    uint8_t *rates = pvalues;
    uint8_t *levels = pvalues + 4;
    
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

    g.setColour(DXLookNFeel::fillColour);
    g.fillPath(p);
    
    g.setColour(Colour(0xFFFFFFFF));
    String len;
    len << ((int) vPos);
    g.drawText(len, 5, 1, 72, 14, Justification::left, true);
}

PitchEnvDisplay::PitchEnvDisplay() {
    pvalues = (uint8_t *) &TMP_LEVEL_PTR;
    vPos = 0;
}

void PitchEnvDisplay::paint(Graphics &g) {
    g.setColour(Colours::white);
    
    uint8_t *levels = pvalues + 4;
    uint8_t *rates = pvalues;
    
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
        
    float ratio =  96 / total;

    Path p;
    p.startNewSubPath(0, 32);

    int x = 0;
    int y = 25 - (pitchenv_tab[levels[3]] + 128) / 10;
    p.lineTo(0,y);

    int dx = x;
    int dy = y;

    int i;
    for(i=0;i<4;i++) {
        if ( vPos == i ) {
            dx = x;
            dy = y;
        }

        x = dist[i] * ratio + x;
        y = 25 - (pitchenv_tab[levels[i]] + 128) / 10;
        p.lineTo(x, y);
    }
    
    if ( vPos == i ) {
        dx = x;
        dy = y;
    }

    p.lineTo(96,32);
    p.lineTo(0, 32);
    g.setColour(DXLookNFeel::fillColour);
    g.fillPath(p);

    g.setColour(Colours::white);
    g.fillEllipse(dx-2, dy-2, 4, 4);
}

void VuMeter::paint(Graphics &g) {
    Image myStrip = ImageCache::getFromMemory(BinaryData::Meter_140x8_png, BinaryData::Meter_140x8_pngSize);
    
    g.drawImage(myStrip, 0, 0, 140, 8, 0, 0, 140, 8);
        
    if ( v <= 0 )
        return;
    
    const int totalBlocks = 46;
    int numBlocks = roundToInt(totalBlocks * v);

    if ( numBlocks > 46 )
        numBlocks = totalBlocks;
    int brkpoint = numBlocks * 3 + 2;
    
    g.drawImage(myStrip, 0, 0, brkpoint, 8, 0, 8, brkpoint, 8);
}

LcdDisplay::LcdDisplay() {
    paramMsg = "DEXED " DEXED_VERSION;
}

void LcdDisplay::setSystemMsg(String msg) {
    paramMsg = msg;
}

void LcdDisplay::paint(Graphics &g) {
    g.setColour (Colours::white);    
    g.drawText (paramMsg,
                0, 0, 140, 14,
                Justification::centred, false);
}

void ComboBoxImage::paint(Graphics &g) {
    int idx = getSelectedItemIndex();
    if ( itemPos[0] != -1 ) {
        if ( idx < 4 )
            idx = itemPos[idx];
    }
    
    g.drawImage(items, 0, 0, items.getWidth(), itemHeight, 0, idx * itemHeight, items.getWidth(), itemHeight);
}

ComboBoxImage::ComboBoxImage() {
    itemPos[0] = -1;
}

static void comboBoxPopupMenuFinishedCallback (int result, ComboBoxImage* combo) {
    if (combo != nullptr) {
        combo->hidePopup();
        
        if (result != 0)
            combo->setSelectedId (result);
    }
}

void ComboBoxImage::showPopup() {
    popup.showMenuAsync (PopupMenu::Options().withTargetComponent (this)
                         .withItemThatMustBeVisible(getSelectedId())
                         .withMinimumWidth(getWidth())
                         .withMaximumNumColumns(1)
                         .withStandardItemHeight(itemHeight),
                         ModalCallbackFunction::forComponent(comboBoxPopupMenuFinishedCallback, this));
}

void ComboBoxImage::setImage(Image image) {
    items = image;
    
    int numItems = getNumItems();
    itemHeight = image.getHeight() / numItems;

    for(int i=0;i<numItems;i++) {
        Image tmp = image.getClippedImage(Rectangle<int>(0,itemHeight*i, image.getWidth(), itemHeight));
        popup.addItem(i+1, getItemText(i), true, false, tmp);
    }
}

void ComboBoxImage::setImage(Image image, int pos[]) {
    items = image;

    int numItems = getNumItems();
    itemHeight = 26;

    for(int i=0;i<numItems;i++) {
        Image tmp = image.getClippedImage(Rectangle<int>(0,itemHeight*pos[i], image.getWidth(), itemHeight));
        popup.addItem(i+1, getItemText(i), true, false, tmp);
    }

    for(int i=0;i<4;i++)
        itemPos[i] = pos[i];
}

void ProgramSelector::mouseDown(const MouseEvent &event) {
    if ( event.x < getWidth() - 8) {
        ComboBox::mouseDown(event);
        return;
    }
    
    int cur = getSelectedItemIndex();
    if ( event.y < getHeight() / 2 ) {
        if ( cur == 0 )
            cur = 31;
        else
            cur--;
    } else {
        if ( cur == 31 )
            cur = 0;
        else
            cur++;
    }
    setSelectedItemIndex(cur);
}

void ProgramSelector::paint(Graphics &g) {
    int x = getWidth();
    int y = getHeight();
    
    Path path;
    path.addTriangle(x-8, y/2-1, x-4, 2,   x, y/2-1);
    path.addTriangle(x-8, y/2+1, x-4, y-2, x, y/2+1);
    
    g.setColour(Colours::white);
    g.fillPath(path);
}
