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
    setSize (765, 380);

    processor = ownerFilter;
    
    addAndMakeVisible (textButton = new TextButton ("importFile"));
    textButton->setButtonText ("Import ...");
    textButton->addListener (this);
    textButton->setBounds (5, 5, 150, 24);

    addAndMakeVisible (&presets);
    presets.setEditableText (false);
    presets.setJustificationType (Justification::centredLeft);
    presets.setTextWhenNothingSelected (String::empty);
    presets.setBounds(200, 5, 150, 24);

    for(int i=0;i<processor->getNumPrograms();i++) {
        presets.addItem( processor->getProgramName(i), i+1);
    }
    presets.setSelectedId(processor->getCurrentProgram()+1, NotificationType::dontSendNotification);
    presets.addListener(this);

    // OPERATORS
    addAndMakeVisible(&(ops[0]));
    ops[0].setBounds(5, 40, 250, 80);
    ops[0].bind(processor, 0);
    
    addAndMakeVisible(&(ops[1]));
    ops[1].setBounds(255, 40, 250, 80);
    ops[1].bind(processor, 1);
    
    addAndMakeVisible(&(ops[2]));
    ops[2].setBounds(505, 40, 250, 80);
    ops[2].bind(processor, 2);
    
    addAndMakeVisible(&(ops[3]));
    ops[3].setBounds(5, 120, 250, 80);
    ops[3].bind(processor, 3);
    
    addAndMakeVisible(&(ops[4]));
    ops[4].setBounds(255, 120, 250, 80);
    ops[4].bind(processor, 4);
    
    addAndMakeVisible(&(ops[5]));
    ops[5].setBounds(505, 120, 250, 80);
    ops[5].bind(processor, 5);

    // add the midi keyboard component..
    addAndMakeVisible (&midiKeyboard);
    
    // The DX7 is a badass on the bass, keep it that way
    midiKeyboard.setLowestVisibleKey(24);

    const int keyboardHeight = 90;
    midiKeyboard.setBounds (4, getHeight() - keyboardHeight - 4, getWidth() - 8, keyboardHeight);

    addAndMakeVisible(&global);
    global.setBounds(505,200,250,80);
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
    g.fillAll (Colours::white);
    g.setColour (Colours::black);
    g.setColour (Colour (0xb3898989));
    g.fillRect (0, 0, getWidth(), 35);

    g.setColour (Colours::black);
    g.drawRect (0, 0, getWidth(), 35, 1);
}

void DexedAudioProcessorEditor::buttonClicked(Button *buttonThatWasClicked) {
    FileChooser fc ("Import original DX sysex...",
                    File::nonexistent,
                    "*.syx;*.SYX",
                    1);
    
    if ( fc.browseForFileToOpen()) {
        String f = fc.getResults().getReference(0).getFullPathName();
        uint8_t syx_data[4104];
        ifstream fp_in;
        fp_in.open(f.toRawUTF8(), ifstream::in);
        if (fp_in.fail()) {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                              "Error",
                                              "Unable to open: " + f);
            return;
        }
        fp_in.read((char *)syx_data, 4104);
        processor->importSysex((char *) &syx_data);

        presets.clear(NotificationType::dontSendNotification);
        for(int i=0;i<processor->getNumPrograms();i++) {
            presets.addItem(processor->getProgramName(i), i+1);
        }
        TRACE("selecting id %d", processor->getCurrentProgram());
        presets.setSelectedId(processor->getCurrentProgram()+1, NotificationType::dontSendNotification);
        processor->updateHostDisplay();
    }
}


// This is our Slider::Listener callback, when the user drags a slider.
void DexedAudioProcessorEditor::sliderValueChanged (Slider* slider) {
}

void DexedAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged) {
    processor->setCurrentProgram(comboBoxThatHasChanged->getSelectedId()-1);
    processor->updateHostDisplay();    
}

void DexedAudioProcessorEditor::timerCallback() {
    int32_t env[6];

    if ( processor->peekEnvStatus(env) == false ) 
        return;

    for(int i=0;i<6;i++) {
        ops[i].updateGain(sqrt(env[i]) / 8000);
    }
}   

void DexedAudioProcessorEditor::updateUI() {
    TRACE("update UI called");
    for(int i=0;i<processor->ctrl.size();i++) {
        processor->ctrl[i]->updateComponent();
    }
    for(int i=0;i<6;i++) {
        ops[i].updateFreqDisplay();
    }
}
