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

#ifndef PROGRAMLISTBOX_H_INCLUDED
#define PROGRAMLISTBOX_H_INCLUDED

#include "JuceHeader.h"

class ProgramListBox;
class ProgramListBoxListener {
public:
    virtual ~ProgramListBoxListener() {}
    virtual void programSelected(ProgramListBox *source, int pos) = 0;
};

class ProgramListBox : public Component {
    ProgramListBoxListener *listener;
    bool hasContent;
    bool showPgmNumber;
    int cols, rows;
    int cellWidth, cellHeight;

    StringArray programNames;
    int programPosition(const MouseEvent &event);
    int selectedPgm;
    
public:
    ProgramListBox(const String name, int numCols);
    void addListener(ProgramListBoxListener *listener);
    void paint(Graphics &g);
    void setCartridge(char *sysex);
    void resized();
    void mouseDoubleClick(const MouseEvent &event);
    void mouseDown(const MouseEvent &event);
    void setSelected(int idx);
};


#endif  // PROGRAMLISTBOX_H_INCLUDED
