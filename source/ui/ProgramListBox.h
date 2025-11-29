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

#ifndef PROGRAMLISTBOX_H_INCLUDED
#define PROGRAMLISTBOX_H_INCLUDED

#include "JuceHeader.h"
#include "PluginData.h"
#include "DXLookNFeel.h"

class ProgramListBox;
class ProgramListBoxListener {
public:
    virtual ~ProgramListBoxListener() = default;
    virtual void programSelected(ProgramListBox *source, int pos) = 0;
    virtual void programRightClicked(ProgramListBox *source, int pos) = 0;
    virtual void programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) = 0;
};

class ProgramLabel;
class ProgramListBox : public Component, public KeyListener {
    ProgramListBoxListener *listener;    
    Cartridge cartContent;
    std::unique_ptr<ProgramLabel> labels[32];

    bool hasContent;
    int cols, rows;
    float cellWidth, cellHeight;
    int activePgm;
    
    friend class ProgramLabel;
public:
    StringArray programNames;
    bool readOnly;
    
    ProgramListBox(const String name, int numCols);
    void addListener(ProgramListBoxListener *listener);
    void resized() override;
    void paint(Graphics &g) override;
    void setActive(int idx);
    
    Cartridge &getCurrentCart();
    void setCartridge(Cartridge &cart);
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;

    ProgramLabel *getProgramComponent(int idx) {
        return labels[idx].get();
    }
};

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
            g.fillAll(DXLookNFeel::background);
            return;
        }

        if ( pgmListBox->hasContent == false )
            return;

        if ( idx == pgmListBox->activePgm ) {
            g.setGradientFill(ColourGradient(DXLookNFeel::fillColour.brighter(3.0f), 0.0f, 0.0f, DXLookNFeel::fillColour.darker(0.5f), 0.0f, getHeight(), false));
        } else if ( getCurrentlyFocusedComponent() == this ) {
            g.setColour(DXLookNFeel::fillColour);
        } else {
            if ( idx % 2 == 0 ) {
                auto alternateColour = DXLookNFeel::lightBackground.interpolatedWith (getLookAndFeel().findColour(ListBox::textColourId), 0.75f);
                g.setColour(alternateColour);
            } else {
                auto alternateColour = DXLookNFeel::lightBackground.interpolatedWith (getLookAndFeel().findColour(ListBox::textColourId), 0.15f);
                g.setColour(alternateColour);
            }
        }
        g.fillAll();
        g.setColour(Colours::black);
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

    void loadProgram() {
        if ( ! pgmListBox->hasContent )
            return;        
        pgmListBox->listener->programSelected(pgmListBox, idx);
    }

    void mouseDown(const MouseEvent &event) override {
        if ( ! pgmListBox->hasContent )
            return;

        if ( event.mods.isPopupMenu()) {
            pgmListBox->listener->programRightClicked(pgmListBox, idx);
            return;
        }
        if ( event.getNumberOfClicks() == 2 )
            loadProgram();
    }

    void mouseDrag(const MouseEvent &event) override {
        if ( ! pgmListBox->hasContent )
            return;
        if ( event.getDistanceFromDragStart() < 7 )
            return;
    
        if (DragAndDropContainer* const dragContainer = DragAndDropContainer::findParentDragContainerFor(this)) {
            ScaledImage snapshot;
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
            pgmListBox->listener->programDragged(pgmListBox, idx, (char *)block->getData());

        repaint();
    }

    bool isActive() {
        return pgmListBox->hasContent;
    }

    struct ProgramLabelAH : public juce::AccessibilityHandler {
        explicit ProgramLabelAH(ProgramLabel *s): program(s), juce::AccessibilityHandler(*s, juce::AccessibilityRole::cell, getAccessibilityActions(s)) {
        }

        virtual String getTitle() const override {
            return String(program->idx + 1) + " " + program->getProgramName();
        }

        ProgramLabel *program;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProgramLabelAH);

        static AccessibilityActions getAccessibilityActions (ProgramLabel* label) {
            return AccessibilityActions().addAction(AccessibilityActionType::press, [label] { label->loadProgram(); });
        }        
    };

    std::unique_ptr< AccessibilityHandler> createAccessibilityHandler() override {
        return std::make_unique<ProgramLabelAH>(this);
    }
};

#endif  // PROGRAMLISTBOX_H_INCLUDED
