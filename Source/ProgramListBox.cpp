/**
 *
 * Copyright (c) 2015 Pascal Gauthier.
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

#include "ProgramListBox.h"
#include "PluginData.h"
#include "DXLookNFeel.h"
#include "Dexed.h"

ProgramListBox::ProgramListBox(const String name, int numCols) : Component(name) {
    cols = numCols;
    rows = 32 / numCols;
    selectedPgm = -1;
    hasContent = false;
}

void ProgramListBox::paint(Graphics &g) {
    int pgm = 0;
    
    g.setColour(Colour(20,18,18));
    g.fillRect(0,0,getWidth(), getHeight());
    g.setColour(Colour(0,0,0));
    g.drawLine(0,0,getWidth(), 0, 2);
    g.setColour(Colour(3,3,1));
    g.drawLine(0,0,0,getHeight(),2);
    g.setColour(Colour(34,32,32));
    g.drawLine(getWidth(), 3, getWidth(), getHeight(), 2);
    g.setColour(Colour(75,73,73));
    g.drawLine(0,getHeight(),getWidth(),getHeight(), 2);


    const float dashLength[] = { 4, 4 };

    g.setColour(Colour(83,76,69));
    for(int i=1;i<cols;i++) {
        Line<float> line(cellWidth*i,0,cellWidth*i,getHeight());
        g.drawDashedLine(line, dashLength, 2);
    }
    for(int i=1;i<rows;i++) {
        Line<float> line(2, cellHeight*i,getWidth(),cellHeight*i);
        g.drawDashedLine(line, dashLength, 2);
    }
    
    for(int i=0;i<cols;i++) {
        for(int j=0;j<rows;j++) {
            if ( selectedPgm == pgm ) {
                g.setColour(DXLookNFeel::fillColour);
                g.fillRoundedRectangle(cellWidth*i+2, cellHeight*j + 2, cellWidth - 4, cellHeight - 4, 0);
            }

            if ( hasContent == true ) {
                g.setColour(Colours::white);
                g.drawFittedText(programNames[pgm], cellWidth * i , cellHeight * j, cellWidth, cellHeight, Justification::centred, true);
            }
            pgm++;
        }
    }
}

void ProgramListBox::resized() {
    cellWidth = getWidth() / cols;
    cellHeight = getHeight() / rows;
}

void ProgramListBox::setCartridge(char *sysex) {
    extractProgramNames((const char *)sysex, programNames);
    memcpy(cartContent, sysex, 4104);
    hasContent = true;
    repaint();
}

void ProgramListBox::addListener(ProgramListBoxListener *listener) {
    this->listener = listener;
}

int ProgramListBox::programPosition(const MouseEvent &event) {
    return (event.getMouseDownY() / cellHeight) + ((event.getMouseDownX() / cellWidth) * rows);
}

void ProgramListBox::mouseDoubleClick(const MouseEvent &event) {
    if ( ! hasContent )
        return;
    if ( ! event.mods.isLeftButtonDown() )
        return;
    
    int pos = programPosition(event);
    if ( listener != nullptr )
        listener->programSelected(this, pos);
}

void ProgramListBox::mouseDown(const MouseEvent &event) {
    if ( ! hasContent )
        return;
    if ( ! event.mods.isRightButtonDown() )
        return;
    
    int pos = programPosition(event);
    if ( listener != nullptr )
        listener->programRightClicked(this, pos);
}

void ProgramListBox::setSelected(int idx) {
    selectedPgm = idx;
}

char* ProgramListBox::getCurrentCart() {
    if ( ! hasContent )
        return nullptr;
    return cartContent;
}
