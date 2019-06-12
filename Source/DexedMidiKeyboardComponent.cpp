/**
 *
 * Copyright (c) 2013-2018 Pascal Gauthier.
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

#include "DexedMidiKeyboardComponent.h"

//==============================================================================
DexedMidiKeyboardComponent::DexedMidiKeyboardComponent(MidiKeyboardState& s, Orientation o, bool showKey, bool preferKeyFocus)
    : MidiKeyboardComponent(s, o), showKeyboard(showKey), preferMidiKeyboardFocus(preferKeyFocus) {
}

DexedMidiKeyboardComponent::~DexedMidiKeyboardComponent() {
}

void DexedMidiKeyboardComponent::setShowKeyboard(bool showKey) {
    showKeyboard = showKey;
}

void DexedMidiKeyboardComponent::setPreferMidiKeyboardFocus(bool preferKeyFocus) {
    preferMidiKeyboardFocus = preferKeyFocus;
}

bool DexedMidiKeyboardComponent::getShowKeyboard() {
    return showKeyboard;
}

bool DexedMidiKeyboardComponent::getPreferMidiKeyboardFocus() {
    return preferMidiKeyboardFocus;
}

void DexedMidiKeyboardComponent::focusLost(FocusChangeType focusChangeType) {
    MidiKeyboardComponent::focusLost(focusChangeType);
    
    /* Get the component that grabbed focus away from the keyboard.
     * If the method returns null, it is not a Dexed component.
     */
    Component *currentlyFocusedComponent = getCurrentlyFocusedComponent();
    bool currentlyFocusedComponentIsADexedComponent = currentlyFocusedComponent != NULL;
    
    // Only take the keyboard focus back if focus was lost to a Dexed component and if it is visible.
    if (currentlyFocusedComponentIsADexedComponent == true 
        && showKeyboard == true 
        && preferMidiKeyboardFocus == true) {
        grabKeyboardFocus();
    }
}
