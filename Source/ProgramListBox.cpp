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

class ProgramLabel : public Component, public DragAndDropTarget {
    ProgramListBox *pgmListBox;
    bool inDrag = false;

public:
    int idx;

    ProgramLabel(ProgramListBox *pgmListBox, int idx) {
        this->pgmListBox = pgmListBox;
        this->idx = idx;
        setWantsKeyboardFocus(true);
        setExplicitFocusOrder(idx+1);
    }

    void paint(Graphics &g) override {
        if ( inDrag ) {
            g.setColour(Colours::black);
            g.fillRect(0,0,getWidth(), getHeight());
            return;
        }

        if ( pgmListBox->hasContent == false )
            return;
        if ( getCurrentlyFocusedComponent() == this )
            g.fillAll(DXLookNFeel::fillColour);
        else {
            if ( idx % 2 == 0 ) {
                auto alternateColour = DXLookNFeel::lightBackground.interpolatedWith (getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
                g.fillAll(alternateColour);
            } else {
                g.fillAll(DXLookNFeel::lightBackground);
            }
        }

        if ( idx == pgmListBox->activePgm ) {
            g.setColour(Colours::white);
        } else {
            g.setColour(Colours::black);
        }

        g.drawFittedText(getProgramName(), 0, 0, getWidth(), getHeight(), Justification::centred, true);
    }

    void focusGained(FocusChangeType cause) override {
        repaint();
    }

    void focusLost(FocusChangeType cause) override {
        repaint();
    }

    String getProgramName() {
        return pgmListBox->cartContent.getProgramName(idx);
    }

    void mouseDown(const MouseEvent &event) override {
        if ( ! pgmListBox->hasContent )
            return;

        if ( event.mods.isPopupMenu()) {
            pgmListBox->listener->programRightClicked(pgmListBox, idx);
            return;
        }
        if ( event.getNumberOfClicks() == 2 )
            pgmListBox->listener->programSelected(pgmListBox, idx);
    }

    void mouseDrag(const MouseEvent &event) override {
        if ( ! pgmListBox->hasContent )
            return;
        if ( event.getDistanceFromDragStart() < 7 )
            return;
    
        if (DragAndDropContainer* const dragContainer = DragAndDropContainer::findParentDragContainerFor(this)) {
            Image snapshot (Image::ARGB, getWidth(), getHeight(), true);
            Graphics g(snapshot);
            paint(g);
            void *src = pgmListBox->cartContent.getRawVoice() + (idx*128);
            var description = var(src, 128);
            dragContainer->startDragging(description, this, snapshot, false);
        }        
    }

    bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override {
        if ( pgmListBox->readOnly )
            return false;
        if ( ! pgmListBox->hasContent )
            return false;

        Component *comp = dragSourceDetails.sourceComponent.get();

        if ( comp == this )
            return false;
        if ( dynamic_cast<ProgramLabel*>(comp) == nullptr )
            return false;

        return true;
    }

    void itemDragEnter(const SourceDetails &dragSourceDetails) override {
        inDrag = true;
        repaint();
    }

    void itemDragMove(const SourceDetails &dragSourceDetails) override {

    }

    void itemDragExit(const SourceDetails &dragSourceDetails) override {
        inDrag = false;
        repaint();
    }

    void itemDropped(const SourceDetails& dragSourceDetails) override {
        inDrag = false;

        Component *comp = dragSourceDetails.sourceComponent.get();
        ProgramLabel *dest = dynamic_cast<ProgramLabel*>(comp);
        jassert(dest);

        MemoryBlock* block = dragSourceDetails.description.getBinaryData();
        if ( pgmListBox->listener != nullptr )
            pgmListBox->listener->programDragged(pgmListBox, dest->idx, (char *)block->getData());

        repaint();
    }

    struct ProgramLabelAH : public juce::AccessibilityHandler {
        explicit ProgramLabelAH(ProgramLabel *s): program(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::listItem) {
        }

        virtual String getTitle() const override {
            return String(program->idx + 1) + " " + program->getProgramName();
        }

        ProgramLabel *program;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProgramLabelAH);
    };

    std::unique_ptr< AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<ProgramLabelAH>(this);
    }
};

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
    cellWidth = getWidth() / cols;
    cellHeight = getHeight() / rows;

    for(int i=0;i<32;i++) {
        int targetCols = i / rows;
        int targetRow = i % rows;
        labels[i].get()->setBounds(targetCols*cellWidth, targetRow*cellHeight, cellWidth, cellHeight);
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

/*    

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
    */
    /*
    for(int i=0;i<cols;i++) {
        for(int j=0;j<rows;j++) {
            if ( activePgm == pgm && dragCandidate == -1 ) {
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
*/
/*int ProgramListBox::programPosition(int x, int y) {
    return (y / cellHeight) + ((x / cellWidth) * rows);
}*/

/*
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
}*/
/*
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
*/
