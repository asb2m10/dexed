/**
 *
 * Copyright (c) 2014-2017 Pascal Gauthier.
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

#ifndef ALGODISPLAY_H_INCLUDED
#define ALGODISPLAY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DexedAudioProcessorEditor;

class AlgoDisplay : public Component {
    static const int MAX_OPERATORS = 6;
    static const int OP_NUMBER_WIDTH = 16;
    static const int OP_NUMBER_HEIGHT = 12;
    
    struct OperatorPosition {
        char id;
        int x, y;
        int width, height;
    };
    
    OperatorPosition opPositions[MAX_OPERATORS];
    int numOpPositions;
    DexedAudioProcessorEditor *editor;
    
    void displayOp(Graphics &g, char id, int x, int y, char link, char fb);
    void storeOpPosition(char id, int x, int y);
public:
    const char *opStatus;
    AlgoDisplay();
    char *algo;
    void paint(Graphics &g);
    void mouseDown(const MouseEvent &event) override;
    void bind(DexedAudioProcessorEditor *edit);
};

#endif  // ALGODISPLAY_H_INCLUDED
