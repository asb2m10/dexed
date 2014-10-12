/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
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
        String ver("Version " DEXED_VERSION " ; built date " __DATE__ );
        g.drawSingleLineText(ver, 9, 118);
    }
};

/**
 * Ugly but usefull midi monitor to know if you are really sending/receiving something from the DX7
 * If the midi is not configured this component wont show up
 */
class MidiMonitor : public Component {
    SysexComm *midi;
public:
    MidiMonitor(SysexComm *sysexComm) {
        midi = sysexComm;
    }

    void paint(Graphics &g) {
        if ( ! (midi->isInputActive() || midi->isOutputActive() ) ) 
            return;

        //g.setColour(DXLookNFeel::dxDarkBrown);
        g.fillRect(0, 0, getWidth(), getHeight());
        g.setColour(Colours::black);
        g.drawSingleLineText("DX7 ", 0, 13);

        if ( midi->isInputActive() ) {
            g.drawSingleLineText("IN", 27,13);
            if ( midi->inActivity ) {
                g.setColour(Colours::red);
            } else {
                g.setColour(Colours::darkgrey);
            }
            g.fillRect(44, 4, 7, 9);

            midi->inActivity = false;
        }

        if ( midi->isOutputActive() ) {
            g.setColour(Colours::black);
            g.drawSingleLineText("OUT", 55, 13);
            if ( midi->outActivity ) {
                g.setColour(Colours::red);
            } else {
                g.setColour(Colours::darkgrey);
            }
            g.fillRect(83, 4, 7, 9);

            midi->outActivity = false;
        }
    }
};

//==============================================================================
DexedAudioProcessorEditor::DexedAudioProcessorEditor (DexedAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
    midiKeyboard (ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    LookAndFeel::setDefaultLookAndFeel(&dx_lnf);

    setSize (866, 677);

    processor = ownerFilter;
    
    addAndMakeVisible(midiMonitor = new MidiMonitor(&processor->sysexComm));
    midiMonitor->setBounds(645, 6, 110, 18);

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
    operators[3].setBounds(2, 220, 287, 218);
    operators[3].bind(processor, 3);
    
    addAndMakeVisible(&(operators[4]));
    operators[4].setBounds(290, 220, 287, 218);
    operators[4].bind(processor, 4);
    
    addAndMakeVisible(&(operators[5]));
    operators[5].setBounds(578, 220, 287, 218);
    operators[5].bind(processor, 5);

    // add the midi keyboard component..
    addAndMakeVisible (&midiKeyboard);
    
    // The DX7 is a badass on the bass, keep it that way
    midiKeyboard.setLowestVisibleKey(24);

    const int keyboardHeight = 90;
    midiKeyboard.setBounds(4, getHeight() - keyboardHeight - 4, getWidth() - 8, keyboardHeight);

    global.editor = this;
    addAndMakeVisible(&global);
    global.setBounds(2,437,864,144);
    global.bind(processor);
    
    sendPopup.addItem(1, "Send program to DX7");
    sendPopup.addItem(2, "Send cartridge to DX7");
    
    global.setMonoState(processor->isMonoMode());
    
    rebuildProgramCombobox();
    global.programs->addListener(this);
    
    updateUI();
    startTimer(100);
}

DexedAudioProcessorEditor::~DexedAudioProcessorEditor() {
    stopTimer();
    processor->unbindUI();
}

//==============================================================================
void DexedAudioProcessorEditor::paint (Graphics& g) {    
    g.setColour(DXLookNFeel::background);
    g.fillRoundedRectangle(0.0f, 0.0f, (float) getWidth(), (float) getHeight(), 0);
}

void DexedAudioProcessorEditor::cartShow() {
    int result = processor->cartManager.getCarts()->show();
    if ( result < 1 )
        return;
    
    processor->loadBuiltin(result-1);
    processor->setCurrentProgram(0);
    rebuildProgramCombobox();
    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
    processor->updateHostDisplay();
    return;
}

void DexedAudioProcessorEditor::loadCart() {
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
        global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
        processor->updateHostDisplay();
    }
    
    return;
}

void DexedAudioProcessorEditor::saveCart() {
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
        }
    }
}

void DexedAudioProcessorEditor::sendToDx7() {
    int result = sendPopup.show();
    
    if ( result == 1 ) {
        uint8_t raw[167];
        
        exportSysexPgm((char *) raw, processor->data, processor->sysexComm.getChl());
        if ( processor->sysexComm.isOutputActive() ) {
            processor->sysexComm.send(MidiMessage(raw, 163));
        }
        global.setSystemMessage(String("Done sending program"));
        return;
    }
    
    if ( result == 2 ) {
        uint8_t raw[4104];
        
        exportSysexCart((char *) raw, (char *) &processor->sysex, processor->sysexComm.getChl());
        if ( processor->sysexComm.isOutputActive() ) {
            processor->sysexComm.send(MidiMessage(raw, 4104));
        }
        global.setSystemMessage(String("Done sending cartridge"));
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
    
    if ( ret == false ) {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Midi Interface", "Error opening midi ports");
    }
    
    //sendButton->setVisible(processor->sysexComm.isOutputActive());
}

void DexedAudioProcessorEditor::initProgram() {
    processor->resetToInitVoice();
}

/*
    if (buttonThatWasClicked == monoButton ) {
        processor->setMonoMode(monoButton->getToggleState());
        monoButton->setState(processor->isMonoMode() ? Button::ButtonState::buttonDown : Button::ButtonState::buttonNormal);
        return;
    }
    
    if (buttonThatWasClicked == aboutButton) {
        AboutBox about(this);
        about.runModalLoop();
        return;
    }

    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Sorry", "Soon !");
}*/

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
    midiMonitor->repaint();
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
    for(int i=0;i<processor->getNumPrograms();i++) {
        String id;
        id << (i+1) << ". " << processor->getProgramName(i);
        global.programs->addItem(id, i+1);
    }
    global.programs->setSelectedId(processor->getCurrentProgram()+1, dontSendNotification);
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
        dialog.addButton("CANCEL", 1, KeyPress(KeyPress::escapeKey));
        dialog.addButton("EXTERNAL FILE", 2, KeyPress());
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
