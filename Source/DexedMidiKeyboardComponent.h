/**
 *
 * Copyright (c) 2013-2017 Pascal Gauthier.
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

#ifndef DEXEDMIDIKEYBOARDCOMPONENT_H_INCLUDED
#define DEXEDMIDIKEYBOARDCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class DexedMidiKeyboardComponent : public MidiKeyboardComponent
{
public:
    DexedMidiKeyboardComponent(MidiKeyboardState& state, 
                               Orientation orientation, 
                               bool showKeyboard, 
                               bool preferMidiKeyboardFocus);

    ~DexedMidiKeyboardComponent();
    
    void setShowKeyboard(bool showKeyboard);
    void setPreferMidiKeyboardFocus(bool preferMidiKeyboardFocus);
    bool getShowKeyboard();
    bool getPreferMidiKeyboardFocus();
    void focusLost(FocusChangeType) override;

private:
    bool showKeyboard;
    bool preferMidiKeyboardFocus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DexedMidiKeyboardComponent)
};

#endif  // DEXEDMIDIKEYBOARDCOMPONENT_H_INCLUDED
