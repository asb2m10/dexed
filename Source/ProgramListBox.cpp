/**
 *
 * Copyright (c) 2015-2024 Pascal Gauthier.
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
    activePgm = -1;
    hasContent = false;
    readOnly = false;
    programNames.clear();
    
    for(int i=0;i<32;i++) {
        labels[i].reset(new ProgramLabel(this, i));
        addAndMakeVisible(labels[i].get());
    }

    setTitle(name);
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    setFocusContainerType(FocusContainerType::focusContainer);
}

void ProgramListBox::resized() {
    cellWidth = (float) getWidth() / cols;
    cellHeight = (float) getHeight() / rows;

    for(int i=0;i<32;i++) {
        int targetCols = i / rows;
        int targetRow = i % rows;
        labels[i].get()->setBounds(cellWidth*targetCols+1, cellHeight*targetRow+1, cellWidth-2, cellHeight-2);
    }
}

void ProgramListBox::setCartridge(Cartridge &cart) {
    cartContent = cart;
    cartContent.getProgramNames(programNames);
    hasContent = true;
    repaint();
}

void ProgramListBox::addListener(ProgramListBoxListener *listener) {
    this->listener = listener;
}

void ProgramListBox::setActive(int idx) {
    activePgm = idx;
    repaint();
}

Cartridge &ProgramListBox::getCurrentCart() {
    return cartContent;
}

bool ProgramListBox::keyPressed(const KeyPress &key, Component *originatingComponent) {
    ProgramLabel *programLabel = dynamic_cast<ProgramLabel*>(getCurrentlyFocusedComponent());
    if ( programLabel == nullptr ) 
        return false;

    if ( key.isKeyCode(KeyPress::returnKey) ) {
        activePgm = programLabel->idx;
        if ( activePgm != -1 ) {
            listener->programSelected(this, activePgm);
        }
        return true;
    }

    int currentIdx = programLabel->idx;

    if ( key.isKeyCode(KeyPress::upKey) ) {
        currentIdx--;
        if ( currentIdx < 0 )
            currentIdx += rows;
    } else if ( key.isKeyCode(KeyPress::downKey) ) {
        currentIdx++;
        if ( currentIdx >= 32 )
            currentIdx -= rows;
    } else if ( key.isKeyCode(KeyPress::leftKey) ) {
        currentIdx -= rows;
        if ( currentIdx < 0 )
            currentIdx += 32;
    } else if ( key.isKeyCode(KeyPress::rightKey) ) {
        currentIdx += rows;
        if ( currentIdx >= 32 )
            currentIdx -= 32;
    } else {
        return false;
    }

    labels[currentIdx]->grabKeyboardFocus();

    repaint();
    return true;
}

void ProgramListBox::paint(Graphics &g) {
    g.fillAll(DXLookNFeel::background);
}
