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
    
    for(int i=0;i<cols;i++) {
        for(int j=0;j<rows;j++) {
            if ( selectedPgm == pgm ) {
                g.setColour(DXLookNFeel::fillColour);
                g.fillRoundedRectangle(cellWidth*i+2, cellHeight*j + 2, cellWidth - 4, cellHeight - 4, 2);
            } else {
                g.setColour(Colours::dimgrey);
                g.drawRoundedRectangle(cellWidth*i+2, cellHeight*j + 2, cellWidth - 4, cellHeight - 4, 2, 0.5);
            }

            if ( hasContent == true ) {
                g.setColour(Colours::white);
                g.drawText(String(pgm+1) + ". ", cellWidth * i + 5, cellHeight * j, cellWidth, cellHeight, Justification::left, true);
                g.drawFittedText(programNames[pgm], cellWidth * i - 5, cellHeight * j, cellWidth, cellHeight, Justification::right, true);
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
    
    int pos = programPosition(event);
    if ( listener != nullptr ) {
        listener->programSelected(this, pos);
    }
    repaint();
}

void ProgramListBox::mouseDown(const MouseEvent &event) {
    
}

void ProgramListBox::setSelected(int idx) {
    selectedPgm = idx;
}
