/*
  ==============================================================================

    ProgramSelector.cpp
    Created: 29 Sep 2014 4:45:42pm
    Author:  Pascal Gauthier

  ==============================================================================
*/
#include "AlgoDisplay.h"

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