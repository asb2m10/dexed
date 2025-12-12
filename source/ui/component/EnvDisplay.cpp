#include "EnvDisplay.h"
#include "../DXLookNFeel.h"
#include "msfa/pitchenv.h"

void PitchEnvDisplay::paint(Graphics &g) {
    g.setColour(juce::Colours::white);

    float dist[4];
    float total = 0;

    int old = pitchenv_tab[levels[3]] + 128;

    // find the scale
    for(int i=0;i<4;i++) {
        int nw = pitchenv_tab[levels[i].get()] + 128;
        dist[i] = ((float)abs(nw - old)) / pitchenv_rate[rates[i]];
        total += dist[i];
        old = nw;
    }

    if ( total < 0.00001 ) {
        dist[0] = dist[1] = dist[2] = dist[3] = 1;
        total = 4;
    }

    float ratio =  96 / total;

    juce::Path p;
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
