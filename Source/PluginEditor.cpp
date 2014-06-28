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
#include "ParamDialog.h"
#include "math.h"
#include <fstream>

using namespace ::std;

class AboutBox : public DialogWindow {
public:
    Image about_png;
    
    AboutBox(Component *parent) : DialogWindow("About", Colour(0xFF000000), true) {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        about_png = ImageCache::getFromMemory(BinaryData::about_png, BinaryData::about_pngSize);
        setSize( about_png.getWidth(), about_png.getHeight());
        centreAroundComponent (parent, getWidth(), getHeight());
    }
    
    void closeButtonPressed() {
        setVisible (false);
    }
    
    void paint(Graphics &g) {
        g.drawImage (about_png, 0, 0, about_png.getWidth(), about_png.getHeight(),
                     0, 0, about_png.getWidth(), about_png.getHeight());
        g.setColour(Colour(0xFF000000));
        String ver("Version 0.5 ; built date " __DATE__ );
        g.drawSingleLineText(ver, 9, 118);
    }
};

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
    midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    LookAndFeel::setDefaultLookAndFeel(&dx_lnf);

    setSize (866, 420);

    processor = ownerFilter;

    addAndMakeVisible (cartButton = new TextButton("CART"));
    cartButton->setButtonText ("CART");
    cartButton->addListener (this);
    cartButton->setBounds(5, 6, 50, 18);
    for(int i=0;i<processor->cartManager.cartNames.size();i++) {
        cartPopup.addItem(i+1, processor->cartManager.cartNames[i]);
    }
    
    addAndMakeVisible (loadButton = new TextButton("LOAD"));
    loadButton->setButtonText ("LOAD");
    loadButton->addListener(this);
    loadButton->setBounds(59, 6, 50, 18);

    addAndMakeVisible(saveButton = new TextButton("SAVE"));
    saveButton->setButtonText ("SAVE");
    saveButton->addListener (this);
    saveButton->setBounds(113, 6, 50, 18);
    
    addAndMakeVisible (&programs);
    programs.setEditableText(false);
    programs.setJustificationType (Justification::centredLeft);
    programs.setTextWhenNothingSelected (String::empty);
    programs.setBounds(167, 6, 160, 18);
    rebuildProgramCombobox();
    programs.addListener(this);
    
    addAndMakeVisible(storeButton = new TextButton("STORE"));
    storeButton->setButtonText("STORE");
    storeButton->addListener(this);
    storeButton->setBounds(331, 6, 50, 18);

    addAndMakeVisible(sendButton = new TextButton("SEND"));
    sendButton->setButtonText("SEND");
    sendButton->addListener(this);
    sendButton->setBounds(385, 6, 50, 18);
    
    addAndMakeVisible(settingsButton = new TextButton("PARMS"));
    settingsButton->setButtonText("PARMS");
    settingsButton->addListener(this);
    settingsButton->setBounds(755, 6, 50, 18);
    
    addAndMakeVisible(aboutButton = new TextButton("ABOUT"));
    aboutButton->setButtonText("ABOUT");
    aboutButton->addListener(this);
    aboutButton->setBounds(807, 6, 50, 18);

    // OPERATORS
    addAndMakeVisible(&(operators[0]));
    operators[0].setBounds(7, 38, 280, 90);
    operators[0].bind(processor, 0);
    
    addAndMakeVisible(&(operators[1]));
    operators[1].setBounds(294, 38, 280, 90);
    operators[1].bind(processor, 1);
    
    addAndMakeVisible(&(operators[2]));
    operators[2].setBounds(579, 38, 280, 90);
    operators[2].bind(processor, 2);
    
    addAndMakeVisible(&(operators[3]));
    operators[3].setBounds(7, 133, 280, 90);
    operators[3].bind(processor, 3);
    
    addAndMakeVisible(&(operators[4]));
    operators[4].setBounds(294, 133, 280, 90);
    operators[4].bind(processor, 4);
    
    addAndMakeVisible(&(operators[5]));
    operators[5].setBounds(579, 133, 280, 90);
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
    
    sendPopup.addItem(1, "Send current program to DX7 via sysex");
    sendPopup.addItem(2, "Send current cartridge to DX7 via sysex");
    
    updateUI();
    startTimer(100);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    processor->unbindUI();
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {    
    g.setColour(Colour(0xFF47260D));
    g.fillRoundedRectangle(0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 0);
    
    g.setColour(Colour(0xFF4D3828));
    g.fillRoundedRectangle(0.0f, 30, (float) getWidth(), 200, 0);
}

void DexedAudioProcessorEditor::buttonClicked(Button *buttonThatWasClicked) {
    if (buttonThatWasClicked == cartButton) {
        int result = cartPopup.show();
        if ( result < 1 )
            return;
        
        processor->loadBuiltin(result-1);
        processor->setCurrentProgram(0);
        rebuildProgramCombobox();
        programs.setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
        processor->updateHostDisplay();
        return;
    }
    
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
            if ( processor->importSysex((char *) &syx_data) ) {
                global.setSystemMessage(String("Unkown sysex format !?"));
            }
            processor->setCurrentProgram(0);
            rebuildProgramCombobox();
            programs.setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
            processor->updateHostDisplay();
        }

        return;
    }

    if (buttonThatWasClicked == saveButton) {
        FileChooser fc ("Export DX sysex...", File::nonexistent, "*.syx", 1);
        if ( fc.browseForFileToSave(true) ) {
            String f = fc.getResults().getReference(0).getFullPathName();
            char syx_data[4104];

            exportSysexCart((char *) syx_data, (char *) &processor->sysex, 0);

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
        storeProgram();
        return;
    }
    
    if (buttonThatWasClicked == sendButton) {
        int result = sendPopup.show();
        
        if ( result == 1 ) {
            uint8_t raw[165];
            
            exportSysexPgm((char *) raw, processor->data, processor->sysexComm.getChl());
            if ( processor->sysexComm.isOutputActive() ) {
                processor->sysexComm.send(MidiMessage(raw, 165));
            } else {
                processor->midiOut.addEvent(raw, 165, 0);
            }
            return;
        }
        
        if ( result == 2 ) {
            uint8_t raw[4104];
            
            exportSysexCart((char *) raw, processor->sysex, processor->sysexComm.getChl());
            if ( processor->sysexComm.isOutputActive() ) {
                processor->sysexComm.send(MidiMessage(raw, 4104));
            } else {
                processor->midiOut.addEvent(raw, 4104, 0);
            }
            return;
        }
        
        return;
    }

    if (buttonThatWasClicked == settingsButton) {
        
        AlertWindow window("","", AlertWindow::NoIcon, this);
        ParamDialog param;
        param.setColour(AlertWindow::backgroundColourId, Colour(0x32FFFFFF));
        param.setDialogValues(processor->controllers, processor->sysexComm);
        
        window.addCustomComponent(&param);
        window.addButton("OK", 0);
        window.addButton("Cancel" ,1);
        if ( window.runModalLoop() != 0 )
            return;
        
        param.getDialogValues(processor->controllers, processor->sysexComm);
        processor->savePreference();
        
        return;
    }
    
    if (buttonThatWasClicked == aboutButton) {
        AboutBox about(this);
        about.runModalLoop();
        return;
    }

    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Sorry", "Soon !");
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(programs.getSelectedId()-1);
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
        operators[i].updateGain(sqrt(processor->voiceStatus.amp[5 - i]) / 8196);
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
    
    int id = processor->getCurrentProgram() + 1;
    programs.setSelectedId(id, dontSendNotification);
    
    global.updateDisplay();
}

void DexedAudioProcessorEditor::rebuildProgramCombobox() {
    programs.clear(dontSendNotification);
    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        programs.addItem(id, i+1);
    }
    programs.setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
}

void DexedAudioProcessorEditor::storeProgram() {
    String currentName(processor->getProgramName(processor->getCurrentProgram()));
    char destSysex[4096];
    File *externalFile = NULL;

    memcpy(&destSysex, processor->sysex, 4096);

    while (true) {
        String msg;
        if ( externalFile == NULL ) {
            msg = "Store program to current cartridge";
        } else {
            msg = "Store program to " + externalFile->getFileName();
        }
        
        AlertWindow dialog(String("Store Program"), msg, AlertWindow::NoIcon, this);
        dialog.addTextEditor(String("Name"), currentName, String("Name"), false);
        // TODO: fix the name length to 10

        StringArray programs;
        extractProgramNames((char *) &destSysex, programs);

        dialog.addComboBox(String("Dest"), programs);
        dialog.addButton("OK", 0, KeyPress(KeyPress::returnKey));
        dialog.addButton("Cancel", 1, KeyPress(KeyPress::escapeKey));
        dialog.addButton("External File", 2, KeyPress());
        int response = dialog.runModalLoop();

        if ( response == 2 ) {
            FileChooser fc("Destination Sysex", File::nonexistent, "*.syx;*.SYX;*.*", 1);

            if ( fc.browseForFileToOpen() ) {
                if ( externalFile != NULL ) 
                    delete externalFile;

                MemoryBlock block;
                externalFile = new File(fc.getResults().getReference(0));
                if ( externalFile->loadFileAsData(block) ) {
                    block.copyTo(destSysex, 6, 4096);
                    continue;
                }
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Read error", "Unable to read file");
            }
        }

        if ( response == 0 ) {
            TextEditor *name = dialog.getTextEditor(String("Name"));
            ComboBox *dest = dialog.getComboBoxComponent(String("Dest"));
            
            int programNum = dest->getSelectedItemIndex();
            String programName(name->getText());
            if ( programName.length() > 10 ) {
                int toStrip = programName.length() - 10;
                programName = programName.dropLastCharacters(toStrip);
            }

            if ( externalFile == NULL ) {
                packProgram((uint8_t *) processor->sysex, (uint8_t *) processor->data, programNum, programName);
                processor->programNames.set(programNum, programName);
                rebuildProgramCombobox();
                processor->setCurrentProgram(programNum);
                processor->updateHostDisplay();
            } else {
                packProgram((uint8_t *) &destSysex, (uint8_t *) processor->data, programNum, programName);
                char sysexFile[4104];
                exportSysexCart((char *) &sysexFile, (char *) &destSysex, 0);
                if ( ! externalFile->replaceWithData(sysexFile, 4104) ) {
                    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Write error", "Unable to write file");
                }
            }
        }

        break;
    }

    if ( externalFile != NULL ) 
        delete externalFile;
}
