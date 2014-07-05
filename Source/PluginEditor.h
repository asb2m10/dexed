/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OperatorEditor.h"
#include "GlobalEditor.h"
#include "DXComponents.h"
#include "DXLookNFeel.h"

//==============================================================================
/**
*/
class DexedAudioProcessorEditor  : public AudioProcessorEditor,
        public ButtonListener,
        public ComboBoxListener,
        public Timer {
    DexedAudioProcessor *processor;
    ComboBox programs;
    PopupMenu cartPopup;
    PopupMenu sendPopup;
            
    MidiKeyboardComponent midiKeyboard;
    DXLookNFeel dx_lnf;

    ScopedPointer<TextButton> cartButton;
    ScopedPointer<TextButton> loadButton;
    ScopedPointer<TextButton> saveButton;
    ScopedPointer<TextButton> storeButton;
    ScopedPointer<TextButton> aboutButton;
    ScopedPointer<TextButton> settingsButton;
    ScopedPointer<TextButton> sendButton;
    ScopedPointer<Component> midiMonitor;
    void storeProgram();

public:

    DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter);
    ~DexedAudioProcessorEditor();

    void timerCallback();
            
    //==============================================================================
    // This is just a standard Juce paint method...
    void paint (Graphics& g);
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);

    OperatorEditor operators[6];
    GlobalEditor global;

    void updateUI();
    void rebuildProgramCombobox();
};


#endif  // PLUGINEDITOR_H_INCLUDED
