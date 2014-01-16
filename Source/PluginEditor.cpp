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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GlobalEditor.h"
#include "math.h"
#include <fstream>

using namespace ::std;

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
    midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{

    LookAndFeel::setDefaultLookAndFeel(&dx_lnf);

    // This is where our plugin's editor size is set.
    setSize (865, 420);

    processor = ownerFilter;

    cachedImage_background_png = ImageCache::getFromMemory (BinaryData::background_png, BinaryData::background_pngSize);
        
    addAndMakeVisible (loadButton = new TextButton("LOAD"));
    loadButton->setButtonText ("LOAD");
    loadButton->addListener (this);
    loadButton->setBounds(5, 5, 50, 18);

    addAndMakeVisible( saveButton = new TextButton("SAVE"));
    saveButton->setButtonText ("SAVE");
    saveButton->addListener (this);
    saveButton->setBounds (57, 5, 50, 18);

    addAndMakeVisible( storeButton = new TextButton("STORE"));
    storeButton->setButtonText ("STORE");
    storeButton->addListener (this);
    storeButton->setBounds (300, 5, 50, 18);

    addAndMakeVisible( aboutButton = new TextButton("ABOUT"));
    aboutButton->setButtonText ("ABOUT");
    aboutButton->addListener (this);
    aboutButton->setBounds (805, 5, 50, 18);

    addAndMakeVisible (&presets);
    presets.setEditableText (false);
    presets.setJustificationType (Justification::centredLeft);
    presets.setTextWhenNothingSelected (String::empty);
    presets.setBounds(115, 5, 180, 18);

    rebuildPresetCombobox();
    presets.addListener(this);

    // OPERATORS
    addAndMakeVisible(&(operators[0]));
    operators[0].setBounds(5, 40, 280, 90);
    operators[0].bind(processor, 0);
    
    addAndMakeVisible(&(operators[1]));
    operators[1].setBounds(290, 40, 280, 90);
    operators[1].bind(processor, 1);
    
    addAndMakeVisible(&(operators[2]));
    operators[2].setBounds(575, 40, 280, 90);
    operators[2].bind(processor, 2);
    
    addAndMakeVisible(&(operators[3]));
    operators[3].setBounds(5, 130, 280, 90);
    operators[3].bind(processor, 3);
    
    addAndMakeVisible(&(operators[4]));
    operators[4].setBounds(290, 130, 280, 90);
    operators[4].bind(processor, 4);
    
    addAndMakeVisible(&(operators[5]));
    operators[5].setBounds(575, 130, 280, 90);
    operators[5].bind(processor, 5);

    // add the midi keyboard component..
    addAndMakeVisible (&midiKeyboard);
    
    // The DX7 is a badass on the bass, keep it that way
    midiKeyboard.setLowestVisibleKey(24);

    const int keyboardHeight = 90;
    midiKeyboard.setBounds (4, getHeight() - keyboardHeight - 4, getWidth() - 8, keyboardHeight);

    addAndMakeVisible(&global);
    global.setBounds(5,235,855,90);
    global.bind(processor);

   updateUI();
   startTimer(100);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    processor->unbindUI();
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {
    g.drawImage (cachedImage_background_png, 0, 0, 865, 420,
                 0, 0, cachedImage_background_png.getWidth(), cachedImage_background_png.getHeight());
}

void DexedAudioProcessorEditor::buttonClicked(Button *buttonThatWasClicked) {

    if (buttonThatWasClicked == loadButton) {
        FileChooser fc ("Import original DX sysex...", File::nonexistent, "*.syx;*.SYX;*.*", 1);

        if ( fc.browseForFileToOpen()) {
            String f = fc.getResults().getReference(0).getFullPathName();
            uint8_t syx_data[4104];
            ifstream fp_in(f.toRawUTF8(), ios::binary);
            if (fp_in.fail()) {
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  "Error",
                                                  "Unable to open: " + f);
                return;
            }
            fp_in.read((char *)syx_data, 4104);
            fp_in.close();
            processor->importSysex((char *) &syx_data);

            rebuildPresetCombobox();

            presets.setSelectedId(processor->getCurrentProgram()+1, NotificationType::dontSendNotification);
            processor->setCurrentProgram(0);
            processor->updateHostDisplay();
        }

        return;
    }

    if (buttonThatWasClicked == saveButton) {
        FileChooser fc ("Export DX sysex...", File::nonexistent, "*.syx", 1);
        if ( fc.browseForFileToSave(true) ) {
            String f = fc.getResults().getReference(0).getFullPathName();
            uint8_t syx_data[4104];

            processor->exportSysex((char *) syx_data);

            ofstream fp_out(f.toRawUTF8(), ios::binary);
            fp_out.write((char *)syx_data, 4104);
            fp_out.close();

            if (fp_out.fail()) {
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  "Error",
                                                  "Unable to write: " + f);
                return;
            }
        }
        return;
    }
    
    if (buttonThatWasClicked == storeButton) {
        AlertWindow dialog(String("Store Program Destination"), "", AlertWindow::NoIcon, this);
        dialog.addTextEditor(String("Name"), processor->getProgramName(processor->getCurrentProgram()), String("Name"), false);
        
        StringArray programs;
        
        for(int i=0;i<32;i++) {
            programs.add(presets.getItemText(i));
        }
        
        dialog.addComboBox(String("Dest"), programs);
        dialog.addButton("OK", 0, KeyPress(KeyPress::returnKey));
        dialog.addButton("Cancel", 1, KeyPress(KeyPress::escapeKey));
        if ( dialog.runModalLoop() == 0 ) {
            TextEditor *name = dialog.getTextEditor(String("Name"));
            ComboBox *dest = dialog.getComboBoxComponent(String("Dest"));
            
            int programNum = dest->getSelectedItemIndex();
            const char *programName = name->getText().toRawUTF8();
            
            processor->packProgram(programNum, programName);
            
            rebuildPresetCombobox();
            
            processor->setCurrentProgram(programNum);
            processor->updateHostDisplay();
        }
        return;
    }

    if (buttonThatWasClicked == aboutButton) {
        AlertWindow::showMessageBoxAsync(AlertWindow::NoIcon, "DEXED - DX Emulator 0.3", "https://github.com/asb2m10/dexed\n"
                "(c) 2013 Pascal Gauthier\nUnder the GPL v2\n\n"
                "Based on Music Synthesizer for Android\nhttps://code.google.com/p/music-synthesizer-for-android");
        return;
    }

    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Sorry", "Soon !");
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(comboBoxThatHasChanged->getSelectedId()-1);
    processor->updateHostDisplay();    
}

void DexedAudioProcessorEditor::timerCallback() {
    int32_t env[6];
    
    if ( processor->refreshUI ) {
        if ( processor->refreshUI & DexedAudioProcessor::REFRESH_COMP )
            updateUI();
        if ( processor->refreshUI & DexedAudioProcessor::REFRESH_MSG )
            global.repaint();
        processor->refreshUI = 0;
    }
    
    if ( processor->peekEnvStatus(env) == false )
        return;
    
    for(int i=0;i<6;i++) {
        operators[i].updateGain(sqrt(env[5 - i]) / 8196);
    } 
}   

void DexedAudioProcessorEditor::updateUI() {
    for(int i=0;i<processor->ctrl.size();i++) {
        processor->ctrl[i]->updateComponent();
    }
    for(int i=0;i<6;i++) {
        operators[i].updateDisplay();
    }
    
    int id = processor->getCurrentProgram() + 1;
    presets.setSelectedId(id, NotificationType::dontSendNotification);
    
    global.repaint();
}

void DexedAudioProcessorEditor::rebuildPresetCombobox() {
    presets.clear(NotificationType::dontSendNotification);
    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        presets.addItem(id, i+1);
    }
    presets.setSelectedId(processor->getCurrentProgram()+1, NotificationType::dontSendNotification);
}

