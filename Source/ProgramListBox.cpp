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
    dragCandidate = -1;
    readOnly = false;
    programNames.clear();
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
            if ( selectedPgm == pgm && dragCandidate == -1 ) {
                g.setColour(DXLookNFeel::fillColour);
                g.fillRoundedRectangle(cellWidth*i+2, cellHeight*j + 2, cellWidth - 4, cellHeight - 4, 0);
            }

            if ( hasContent == true ) {
                if ( dragCandidate != pgm ) {
                    g.setColour(Colours::white);
                    g.drawFittedText(programNames[pgm], cellWidth * i , cellHeight * j, cellWidth, cellHeight, Justification::centred, true);
                } else {
                    g.setColour(DXLookNFeel::fillColour);
                    g.fillRoundedRectangle(cellWidth*i+2, cellHeight*j + 2, cellWidth - 4, cellHeight - 4, 0);
                }
                
            }
            pgm++;
        }
    }
}

void ProgramListBox::resized() {
    cellWidth = getWidth() / cols;
    cellHeight = getHeight() / rows;
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

int ProgramListBox::programPosition(int x, int y) {
    return (y / cellHeight) + ((x / cellWidth) * rows);
}

void ProgramListBox::mouseDown(const MouseEvent &event) {
    if ( ! hasContent )
        return;
    
    int pos = programPosition(event.getMouseDownX(), event.getMouseDownY());

    if ( event.mods.isRightButtonDown() || event.mods.isAnyModifierKeyDown() ) {
        listener->programRightClicked(this, pos);
        return;
    }
    
    listener->programSelected(this, pos);
}

void ProgramListBox::mouseUp(const MouseEvent &event) {
}

void ProgramListBox::mouseDrag(const MouseEvent &event) {
    if ( ! hasContent )
        return;
    if ( dragCandidate != -1 )
        return;
    if ( event.getDistanceFromDragStart() < 7 )
        return;
    
    if (DragAndDropContainer* const dragContainer = DragAndDropContainer::findParentDragContainerFor(this)) {
        Image snapshot (Image::ARGB, cellWidth, cellHeight, true);
        int position = programPosition(event.getMouseDownX(), event.getMouseDownY());
        Graphics g(snapshot);
        g.setColour(DXLookNFeel::lightBackground);
        g.fillRect(0,0,cellWidth, cellHeight);
        g.setColour(Colours::white);
        g.drawFittedText(programNames[position], 0, 0, cellWidth, cellHeight, Justification::centred, true);
        void *src = cartContent.getRawVoice() + (position*128);
        var description = var(src, 128);
        dragContainer->startDragging(description, this, snapshot, false);
    }
}

void ProgramListBox::setSelected(int idx) {
    selectedPgm = idx;
}

Cartridge &ProgramListBox::getCurrentCart() {
    return cartContent;
}

bool ProgramListBox::isInterestedInDragSource (const SourceDetails& dragSourceDetail)  {
    if ( readOnly )
        return false;
    if ( ! hasContent )
        return false;
    
    Component *comp = dragSourceDetail.sourceComponent.get();
    if ( comp == nullptr )
        return false;
    if ( dynamic_cast<ProgramListBox*>(comp) == nullptr )
        return false;
    
    return true;
}

void ProgramListBox::itemDropped(const SourceDetails& dragSourceDetails) {
    dragCandidate = programPosition(dragSourceDetails.localPosition.x, dragSourceDetails.localPosition.y);
    
    MemoryBlock* block = dragSourceDetails.description.getBinaryData();
    if ( listener != nullptr )
        listener->programDragged(this, dragCandidate, (char *)block->getData());
    dragCandidate = -1;
    repaint();
}

void ProgramListBox::itemDragEnter(const SourceDetails &dragSourceDetails) {
    dragCandidate = programPosition(dragSourceDetails.localPosition.x, dragSourceDetails.localPosition.y);
    repaint();
}

void ProgramListBox::itemDragMove(const SourceDetails &dragSourceDetails) {
    dragCandidate = programPosition(dragSourceDetails.localPosition.x, dragSourceDetails.localPosition.y);
    repaint();
}

void ProgramListBox::itemDragExit(const SourceDetails &dragSourceDetails) {
    dragCandidate = -1;
    repaint();
}

