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
 */

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "OperatorEditor.h"
#include "GlobalEditor.h"
#include "DXComponents.h"
#include "DXLookNFeel.h"
#include "CartManager.h"

//==============================================================================
/**
*/
class DexedAudioProcessorEditor  : public AudioProcessorEditor, public ComboBox::Listener, public Timer,
                                   public FileDragAndDropTarget {
    MidiKeyboardComponent midiKeyboard;
    OperatorEditor operators[6];
    Colour background;
    CartManager cartManager;

    SharedResourcePointer<DXLookNFeel> lookAndFeel;
public:
    DexedAudioProcessor *processor;
    GlobalEditor global;
    
    DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter);
    ~DexedAudioProcessorEditor();
    virtual void timerCallback() override;

    virtual void paint (Graphics& g) override;
    virtual void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void updateUI();
    void rebuildProgramCombobox();
    void loadCart(File file);
    void saveCart();
    void initProgram();
    void storeProgram();
    void cartShow();
    void parmShow();
    void tuningShow();
    void discoverMidiCC(Ctrl *ctrl);

    virtual bool isInterestedInFileDrag (const StringArray &files) override;
    virtual void filesDropped (const StringArray &files, int x, int y ) override;
};


#endif  // PLUGINEDITOR_H_INCLUDED
