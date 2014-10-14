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

    g.setGradientFill(ColourGradient());
    g.setColour(DXLookNFeel::fillColour);
    switch(link) {
    case 0 : // LINE DOWN
        g.drawLine(x+8, y+12, x+8, y+21, LINE_SZ);
        break;
    case 1: // ARROW TO RIGHT
        g.drawLine(x+8, y+12, x+8,  y+18, LINE_SZ);
        g.drawLine(x+7, y+18, x+34, y+18, LINE_SZ);
        break;
    case 2: // ARROW TO RIGHT JOIN
        g.drawLine(x+8,  y+12, x+8, y+19, LINE_SZ);
        //g.drawLine(x-17, y+18, x+9, y+18, LINE_SZ);
        break;
    case 3: // LINE
        g.drawLine(x+8, y+12, x+8, y+18, LINE_SZ);
        g.drawLine(x,   y+18, x+26, y+18, LINE_SZ);
        break;
    case 4: // LEFT * 2
        g.drawLine(x+8,  y+12, x+8, y+18, LINE_SZ);
        g.drawLine(x-16, y+18, x+25, y+18, LINE_SZ);
        break;
    case 5: // RIGHT DIAG
        g.drawLine(x+8,  y+12, x+25, y+18, LINE_SZ);
        break;
    case 6:
        g.drawLine(x+8, y+12, x+8,  y+18, LINE_SZ);
        g.drawLine(x+7, y+18, x+58, y+18, LINE_SZ);
        break;
    case 7: // ARROW TO LEFT
        g.drawLine(x+8,  y+12, x+8, y+19, LINE_SZ);
        g.drawLine(x-17, y+18, x+9, y+18, LINE_SZ);
        break;
    }

    switch(fb) {
    case 0:
        break;
    case 1:
        g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
        g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
        g.drawLine(x+19, y-4, x+19, y+15, LINE_SZ);
        g.drawLine(x+18, y+15, x+19, y+15, LINE_SZ);
        g.drawLine(x+8, y+15, x+20, y+15, LINE_SZ);
        break;
    case 2: // ALGO 4
        g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
        g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
        g.drawLine(x+19, y-4, x+19, y+59, LINE_SZ);
//        g.drawLine(x+19, y+57, x+19, y+59, LINE_SZ);
        g.drawLine(x+8, y+58, x+19, y+58, LINE_SZ);
        break;
    case 3: // ALGO 6
        g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
        g.drawLine(x+8, y-4, x+20, y-4, LINE_SZ);
        g.drawLine(x+19, y-4, x+19, y+37, LINE_SZ);
        //g.drawLine(x+18, y+36, x+19, y+36, LINE_SZ);
        g.drawLine(x+8, y+36, x+19, y+36, LINE_SZ);
        break;
    case 4:
        g.drawLine(x+7, y, x+8, y-5, LINE_SZ);
        g.drawLine(x+8, y-4, x-4, y-4, LINE_SZ);
        g.drawLine(x-3, y-4, x-3, y+15, LINE_SZ);
        g.drawLine(x-3, y+15, x+8, y+15, LINE_SZ);
        g.drawLine(x+8, y+15, x+8, y+12, LINE_SZ);
        break;
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
        displayOp(g, 6, 3, 1, 0, 2);
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
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0); 
        break;
    case 5: //
        displayOp(g, 6, 3, 2, 0, 3);
        displayOp(g, 5, 3, 3, 2, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0); 
        break;
    case 6:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 7, 0);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 2, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 7: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 7, 0);
        displayOp(g, 4, 2, 2, 0, 4);
        displayOp(g, 3, 2, 3, 2, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 8: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 7, 0);
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
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;
    case 10: 
        displayOp(g, 6, 2, 2, 0, 1);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 3, 1, 0, 0);
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;
    case 11: 
        displayOp(g, 6, 3, 2, 7, 0);
        displayOp(g, 5, 2, 2, 0, 0);
        displayOp(g, 4, 1, 2, 1, 0);
        displayOp(g, 3, 2, 3, 6, 0);
        displayOp(g, 2, 4, 2, 0, 1);
        displayOp(g, 1, 4, 3, 2, 0);
        break;
    case 12: 
        displayOp(g, 6, 3, 2, 7, 1);
        displayOp(g, 5, 2, 2, 0, 0);
        displayOp(g, 4, 1, 2, 1, 0);
        displayOp(g, 3, 2, 3, 6, 0);
        displayOp(g, 2, 4, 2, 0, 0);
        displayOp(g, 1, 4, 3, 2, 0);
        break;
    case 13:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 2, 1, 1, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 14: 
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 2, 1, 1, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 4);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 15:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 7, 0);
        displayOp(g, 4, 2, 1, 0, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 1, 2, 1, 0);
        displayOp(g, 1, 2, 3, 0, 0);
        break;
    case 16:
        displayOp(g, 6, 3, 1, 0, 0);
        displayOp(g, 5, 3, 2, 7, 0);
        displayOp(g, 4, 2, 1, 0, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 1, 2, 1, 4);
        displayOp(g, 1, 2, 3, 0, 0);
        break;
    case 17:
        displayOp(g, 6, 3, 0, 0, 0);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 7, 0);
        displayOp(g, 3, 2, 2, 0, 4);
        displayOp(g, 2, 1, 2, 1, 0);
        displayOp(g, 1, 2, 3, 0, 0);
        break;
    case 18:
        displayOp(g, 6, 2, 2, 0, 1);
        displayOp(g, 5, 3, 3, 2, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 1, 1, 0, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 19:
       displayOp(g, 6, 4, 2, 0, 0);
       displayOp(g, 5, 3, 2, 1, 0);
       displayOp(g, 4, 4, 3, 2, 0);
       displayOp(g, 3, 1, 2, 0, 1);
       displayOp(g, 2, 2, 3, 6, 0);
       displayOp(g, 1, 1, 3, 1, 0);
       break;
    case 20:
        displayOp(g, 6, 3, 2, 0, 0);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 1, 2, 0, 1);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 21:
        displayOp(g, 6, 3, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 22: // CC
        displayOp(g, 6, 3, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 23: // CC
        displayOp(g, 6, 3, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    case 24: // CC
        displayOp(g, 6, 3, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    case 25:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 4, 3, 2, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 2, 3, 6, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 26:
        displayOp(g, 6, 4, 2, 0, 0);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 4, 3, 2, 0);
        displayOp(g, 3, 2, 2, 0, 1);
        displayOp(g, 2, 2, 3, 6, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 27:
        displayOp(g, 6, 3, 3, 2, 0);
        displayOp(g, 5, 2, 1, 0, 1);
        displayOp(g, 4, 2, 2, 0, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 28:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 29:
        displayOp(g, 6, 4, 3, 2, 0);
        displayOp(g, 5, 3, 1, 0, 1);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 30:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);     
        break;
    case 31:
        displayOp(g, 6, 5, 3, 2, 1);
        displayOp(g, 5, 4, 3, 1, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);       
        break;
    default:
        break;
    }
}

