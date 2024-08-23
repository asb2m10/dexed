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

class ProgramListBox;
class ProgramListBoxListener {
public:
    virtual ~ProgramListBoxListener() = default;
    virtual void programSelected(ProgramListBox *source, int pos) = 0;
    virtual void programRightClicked(ProgramListBox *source, int pos) = 0;
    virtual void programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) = 0;
};

class ProgramListBox : public Component, public KeyListener {
    ProgramListBoxListener *listener;    
    Cartridge cartContent;
    std::unique_ptr<Component> labels[32];

    bool hasContent;
    int cols, rows;
    int cellWidth, cellHeight;
    int activePgm;
    
    friend class ProgramLabel;
public:
    StringArray programNames;
    bool readOnly;
    
    ProgramListBox(const String name, int numCols);
    void addListener(ProgramListBoxListener *listener);
    void resized() override;
    void setActive(int idx);
    
    Cartridge &getCurrentCart();
    void setCartridge(Cartridge &cart);
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;
};


#endif  // PROGRAMLISTBOX_H_INCLUDED
