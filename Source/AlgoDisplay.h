/*
  ==============================================================================
  ==============================================================================
*/

#ifndef ALGODISPLAY_H_INCLUDED
#define ALGODISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class AlgoDisplay : public Component {
    void drawOp(Graphics &g, int x, int y, int num);
public:
    AlgoDisplay();
    char *algo;
    void paint(Graphics &g);
};

#endif  // ALGODISPLAY_H_INCLUDED
