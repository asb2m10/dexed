/**
 *
 * Copyright (c) 2013-2015 Pascal Gauthier.
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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GlobalEditor.h"
#include "ParamDialog.h"
#include "SysexComm.h"
#include "Dexed.h"
#include "math.h"
#include "DXLookNFeel.h"
#include <fstream>

#include "msfa/fm_op_kernel.h"

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
    midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    cartManager(this)
{
    lookAndFeel = DXLookNFeel::getLookAndFeel();
    LookAndFeel::setDefaultLookAndFeel(lookAndFeel);

    setSize(866, ownerFilter->showKeyboard ? 674 : 581);

    processor = ownerFilter;
    
    background = DXLookNFeel::getLookAndFeel()->background;

    // OPERATORS
    addAndMakeVisible(&(operators[0]));
    operators[0].setBounds(2, 1, 287, 218);
    operators[0].bind(processor, 0);
    
    addAndMakeVisible(&(operators[1]));
    operators[1].setBounds(290, 1, 287, 218);
    operators[1].bind(processor, 1);
    
    addAndMakeVisible(&(operators[2]));
    operators[2].setBounds(578, 1, 287, 218);
    operators[2].bind(processor, 2);
    
    addAndMakeVisible(&(operators[3]));
    operators[3].setBounds(2, 219, 287, 218);
    operators[3].bind(processor, 3);
    
    addAndMakeVisible(&(operators[4]));
    operators[4].setBounds(290, 219, 287, 218);
    operators[4].bind(processor, 4);
    
    addAndMakeVisible(&(operators[5]));
    operators[5].setBounds(578, 219, 287, 218);
    operators[5].bind(processor, 5);

    // add the midi keyboard component..
    addAndMakeVisible (&midiKeyboard);
    
    // The DX7 is a badass on the bass, keep it that way
    midiKeyboard.setLowestVisibleKey(24);

    midiKeyboard.setBounds(4, 581, getWidth() - 8, 90);

    addAndMakeVisible(&global);
    global.setBounds(2,436,864,144);
    global.bind(this);
    
    global.setMonoState(processor->isMonoMode());
    
    rebuildProgramCombobox();
    global.programs->addListener(this);
    
    addChildComponent(&cartManager);
    
    updateUI();
    startTimer(100);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    processor->unbindUI();
    setLookAndFeel(nullptr);
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {    
    g.setColour(background);
    g.fillRoundedRectangle(0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 0);
}

void DexedAudioProcessorEditor::cartShow() {
    stopTimer();    
    cartManager.resetActiveSysex();
    cartManager.setBounds(4, 2, 859, 576);
    cartManager.setVisible(true);
    cartManager.initialFocus();
}


void DexedAudioProcessorEditor::loadCart(File file) {
    Cartridge cart;

    int rc = cart.load(file);
    
    if ( rc < 0 ) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Unable to open: " + file.getFullPathName());
        return;
    }
    
    if ( rc != 0 ) {
        rc = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Unable to find DX7 sysex cartridge in file",
                                          "This sysex file is not for the DX7 or it is corrupted. "
                                          "Do you still want to load this file as random data ?");
        if ( rc == 0 )
            return;
    }
    
    processor->loadCartridge(cart);
    rebuildProgramCombobox();
    processor->setCurrentProgram(0);
    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
    processor->updateHostDisplay();
    
    processor->activeFileCartridge = file;
}

void DexedAudioProcessorEditor::saveCart() {
    File startFileName = processor->activeFileCartridge.exists() ? processor->activeFileCartridge : processor->dexedCartDir;

    FileChooser fc ("Export DX sysex...", processor->dexedCartDir, "*.syx;*.SYX", 1);
    if ( fc.browseForFileToSave(true) ) {
        if ( ! processor->currentCart.saveVoice(fc.getResults().getReference(0)) ) {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              "Error",
                                              "Unable to write: " + fc.getResults().getReference(0).getFullPathName());
        }
    }
}

void DexedAudioProcessorEditor::parmShow() {
    int tp = processor->getEngineType();
    
    AlertWindow window("","", AlertWindow::NoIcon, this);
    ParamDialog param;
    param.setColour(AlertWindow::backgroundColourId, Colour(0x32FFFFFF));
    param.setDialogValues(processor->controllers, processor->sysexComm, tp, processor->showKeyboard);
    
    window.addCustomComponent(&param);
    window.addButton("OK", 0);
    window.addButton("Cancel" ,1);
    if ( window.runModalLoop() != 0 )
        return;
    
    bool ret = param.getDialogValues(processor->controllers, processor->sysexComm, &tp, &processor->showKeyboard);
    processor->setEngineType(tp);
    processor->savePreference();
    
    setSize(866, processor->showKeyboard ? 674 : 581);
    midiKeyboard.repaint();
    
    if ( ret == false ) {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Midi Interface", "Error opening midi ports");
    }
}

void DexedAudioProcessorEditor::initProgram() {
    processor->resetToInitVoice();
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(global.programs->getSelectedId()-1);
    processor->updateHostDisplay();
}

void DexedAudioProcessorEditor::timerCallback() {
    if ( processor->forceRefreshUI ) {
        processor->forceRefreshUI = false;
        updateUI();
    }
    
    if ( ! processor->peekVoiceStatus() )
        return;

    for(int i=0;i<6;i++) {
        operators[i].updateGain(sqrt(processor->voiceStatus.amp[5 - i]) / 8196);        // TODO: FUGLY !!!! change this sqrt nonsens
        operators[i].updateEnvPos(processor->voiceStatus.ampStep[5 - i]);
    }
    global.updatePitchPos(processor->voiceStatus.pitchStep);
    global.updateVu(processor->vuSignal);
}   

void DexedAudioProcessorEditor::updateUI() {
    for(int i=0;i<processor->ctrl.size();i++) {
        processor->ctrl[i]->updateComponent();
    }
    for(int i=0;i<6;i++) {
        operators[i].updateDisplay();
    }
    rebuildProgramCombobox();
    global.updateDisplay();
}

void DexedAudioProcessorEditor::rebuildProgramCombobox() {
    global.programs->clear(dontSendNotification);
    
    processor->currentCart.getProgramNames(processor->programNames);
    
    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        global.programs->addItem(id, i+1);
    }
    
    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
    
    String name = Cartridge::normalizePgmName((const char *) processor->data+145);
    cartManager.setActiveProgram(processor->getCurrentProgram(), name);
    if ( name != processor->getProgramName(processor->getCurrentProgram()) )
        global.programs->setText("**. " + name, dontSendNotification);
    
    cartManager.resetActiveSysex();
}

void DexedAudioProcessorEditor::storeProgram() {
    String currentName = Cartridge::normalizePgmName((const char *) processor->data+145);
    Cartridge destSysex = processor->currentCart;
    File *externalFile = NULL;

    bool activeCartridgeFound = processor->activeFileCartridge.exists();
    
    while (true) {
        String msg;
        
        if ( externalFile == NULL ) {
            if ( activeCartridgeFound )
                msg = "Store program to current (" + processor->activeFileCartridge.getFileName() + ") / new cartridge";
            else
                msg = "Store program to current / new cartridge";
        } else {
            msg = "Store program to " + externalFile->getFileName();
        }
        
        AlertWindow dialog("Store Program", msg, AlertWindow::NoIcon, this);
        dialog.addTextEditor("Name", currentName, String("Name"), false);
        // TODO: fix the name length to 10

        StringArray programs;
        destSysex.getProgramNames(programs);
        dialog.addComboBox("Dest", programs, "Program Destination");


        if ( externalFile == NULL ) {
            StringArray saveAction;
            saveAction.add("Store program to DAW plugin song state");
            saveAction.add("Store program and create a new copy of the .syx cartridge");
            if ( activeCartridgeFound )
                saveAction.add("Store program and overwrite current .syx cartridge");
        
            dialog.addComboBox("SaveAction", saveAction, "Store Action");
        }
                
        dialog.addButton("OK", 0, KeyPress(KeyPress::returnKey));
        dialog.addButton("CANCEL", 1, KeyPress(KeyPress::escapeKey));
        dialog.addButton("EXTERNAL FILE", 2, KeyPress());
        int response = dialog.runModalLoop();

        if ( response == 2 ) {
            FileChooser fc("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX;*.*", 1);

            if ( fc.browseForFileToOpen() ) {
                if ( externalFile != NULL ) 
                    delete externalFile;

                externalFile = new File(fc.getResults().getReference(0));
                if ( destSysex.load(*externalFile) == 0 )
                    continue;
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Read error", "Unable to read file");
            }
        }

        if ( response == 0 ) {
            TextEditor *name = dialog.getTextEditor("Name");
            ComboBox *dest = dialog.getComboBoxComponent("Dest");
            
            int programNum = dest->getSelectedItemIndex();
            String programName(name->getText());
            if ( programName.length() > 10 ) {
                int toStrip = programName.length() - 10;
                programName = programName.dropLastCharacters(toStrip);
            }

            if ( externalFile == NULL ) {
                processor->currentCart.packProgram((uint8_t *) processor->data, programNum, programName, processor->controllers.opSwitch);
                rebuildProgramCombobox();
                processor->setCurrentProgram(programNum);
                processor->updateHostDisplay();
                
                int action = dialog.getComboBoxComponent("SaveAction")->getSelectedItemIndex();
                if ( action > 0 ) {                  
                    File destination = processor->activeFileCartridge;
                    if ( action == 1 ) {
                        FileChooser fc("Destination Sysex", processor->dexedCartDir, "*.syx;*.SYX", 1);
                        if ( ! fc.browseForFileToSave(true) )
                            break;
                        destination = fc.getResult();
                    }
                    
                    processor->currentCart.saveVoice(destination);
                    processor->activeFileCartridge = destination;
                }
            } else {
                destSysex.packProgram((uint8_t *) processor->data, programNum, programName, processor->controllers.opSwitch);
                if ( ! destSysex.saveVoice(*externalFile)) {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Write error", "Unable to write file");
                }
            }
        }
        break;
    }

    if ( externalFile != NULL )
        delete externalFile;
    cartManager.resetActiveSysex();
}
