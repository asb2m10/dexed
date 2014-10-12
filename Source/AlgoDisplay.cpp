/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#include "AlgoDisplay.h"
#include "DXLookNFeel.h"

AlgoDisplay::AlgoDisplay() {
    static char tmpAlgo = 0;
    algo = &tmpAlgo;
}

inline void displayOp(Graphics &g, char id, char x, char y, char link, char fb) {
    const int LINE_SZ = 3;
    String t(id);

    x *= 25;
    y *= 21;
    y += 5;

    g.setColour(Colours::white);
    g.drawText(t, x, y, 16, 12, Justification::centred, true);

    g.setColour(DXLookNFeel::fillColour);
    switch(link) {
    case 0 : // LINE DOWN
        g.drawLine(x+8, y+12, x+8, y+21, LINE_SZ);
        break;
    case 1: // ARROW TO RIGHT
        g.drawLine(x+8, y+12, x+8,  y+18, LINE_SZ);
        g.drawLine(x+7, y+18, x+32, y+18, LINE_SZ);
        break;
    case 2: // ARROW TO LEFT
        g.drawLine(x+8,  y+12, x+8, y+18, LINE_SZ);
        g.drawLine(x-16, y+18, x+10, y+18, LINE_SZ);
        break;
    case 3: // LINE
        g.drawLine(x+8, y+12, x+8, y+18, LINE_SZ);
        g.drawLine(x,   y+18, x+26, y+18, LINE_SZ);
        break;
    case 4: // LEFT * 2
        g.drawLine(x+8,  y+12, x+8, y+18, LINE_SZ);
        g.drawLine(x-16, y+18, x+25, y+18, LINE_SZ);
    default :
        break;
    }

    if ( fb ) {
        g.drawLine(x+7, y, x+8, y-4, LINE_SZ);
        g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
        g.drawLine(x+19, y-4, x+19, y+15, LINE_SZ);
        g.drawLine(x+18, y+15, x+19, y+15, LINE_SZ);
        g.drawLine(x+8, y+15, x+19, y+15, LINE_SZ);
    }

}

void AlgoDisplay::paint(Graphics &g) {

    switch(*algo) {
    case 0:
        displayOp(g, 6, 3, 0, 0, 1);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 1:
        displayOp(g, 6, 3, 0, 0, 0);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 1);
        displayOp(g, 1, 2, 3, 1, 0);
        break;

    case 2:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 0, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0); 
        break;
    case 3: //
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 0, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);    
        break;

    case 4:
        displayOp(g, 6, 3, 2, 0, 1);
        displayOp(g, 5, 3, 3, 2, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 3, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0); 
        break;
    case 5: //
        displayOp(g, 6, 3, 2, 0, 0);
        displayOp(g, 5, 3, 3, 2, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 3, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0); 
        break;

    case 6:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 2, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 2, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 7: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 2, 0);
        displayOp(g, 4, 2, 2, 0, 1);
        displayOp(g, 3, 2, 3, 2, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 8: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 2, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 2, 0);
        displayOp(g, 2, 1, 2, 0, 1);
        displayOp(g, 1, 1, 3, 1, 0);
        break;

    case 9: 
        displayOp(g, 6, 2, 2, 0, 0);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 3, 1, 0, 1);
        displayOp(g, 2, 3, 2, 2, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;
    case 10: 
        displayOp(g, 6, 2, 2, 0, 1);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 3, 1, 0, 0);
        displayOp(g, 2, 3, 2, 2, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;

    case 11: 
        displayOp(g, 6, 2, 2, 0, 1);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 0, 2, 1, 0);
        displayOp(g, 3, 1, 3, 1, 0);
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;

    case 12: 
        displayOp(g, 6, 2, 2, 0, 0);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 0, 2, 1, 0);
        displayOp(g, 3, 1, 3, 1, 0);
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;

    case 13:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0); 
        break;
    case 14: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);    
        break;

    case 30:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 3, 0);
        displayOp(g, 3, 2, 3, 3, 0);
        displayOp(g, 2, 1, 3, 3, 0);
        displayOp(g, 1, 0, 3, 1, 0);     
        break;
    case 31:
        displayOp(g, 6, 5, 3, 2, 1);
        displayOp(g, 5, 4, 3, 3, 0);
        displayOp(g, 4, 3, 3, 3, 0);
        displayOp(g, 3, 2, 3, 3, 0);
        displayOp(g, 2, 1, 3, 3, 0);
        displayOp(g, 1, 0, 3, 1, 0);       
        break;
    default:
        break;
    }
}

