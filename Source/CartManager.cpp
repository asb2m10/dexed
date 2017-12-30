/**
 *
 * Copyright (c) 2015-2017 Pascal Gauthier.
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
#include "CartManager.h"
#include "DXLookNFeel.h"
#include "PluginData.h"

#include <fstream>
using namespace ::std;

class SyxFileFilter : public FileFilter {
public:
    SyxFileFilter() : FileFilter(".syx") {}
    bool isFileSuitable(const File &file) const {
        return file.getFileExtension().toLowerCase() == ".syx" && file.getSize() >= 4096;
    }
    bool isDirectorySuitable(const File &file) const {
        return true;
    };
};

class FileTreeDrop : public FileTreeComponent {
public :
    FileTreeDrop(DirectoryContentsList &listToShow) : FileTreeComponent(listToShow) {}
    
    bool isInterestedInFileDrag (const StringArray &files) override {
        bool found = false;
        
        for(int i=0; i<files.size(); i++) {
            String filename = files[i].toLowerCase();
            found |= filename.endsWith(".syx");
        }
        return found;
    }
    
    void filesDropped(const StringArray &files, int x, int y) override {
        File targetDir = getSelectedFile();
        
        if ( ! targetDir.exists() )
            targetDir = DexedAudioProcessor::dexedCartDir;
        
        if ( ! targetDir.isDirectory() )
            targetDir = targetDir.getParentDirectory();
        
        for(int i=0; i<files.size(); i++) {
            if ( files[i].toLowerCase().endsWith(".syx") ) {
                File src(files[i]);
                File target = targetDir.getChildFile(src.getFileName());
                src.copyFileTo(target);
            }
        }
        refresh();
    }
};

CartManager::CartManager(DexedAudioProcessorEditor *editor) : Component("CartManager") {
    mainWindow = editor;
    cartDir = DexedAudioProcessor::dexedCartDir;            
            
    addAndMakeVisible(activeCart = new ProgramListBox("activepgm", 8));
    activeCart->setBounds(28, 441, 800, 96);
    activeCart->addListener(this);
            
    addAndMakeVisible(browserCart = new ProgramListBox("browserpgm", 2));
    browserCart->setBounds(635, 18, 200, 384);
    browserCart->addListener(this);
    
    // -------------------------
    syxFileFilter = new SyxFileFilter();
    timeSliceThread = new TimeSliceThread("Cartridge Directory Scanner");
    timeSliceThread->startThread();
    cartBrowserList = new DirectoryContentsList(syxFileFilter, *timeSliceThread);
    cartBrowserList->setDirectory(cartDir, true, true);
    cartBrowser = new FileTreeDrop(*cartBrowserList);
    cartBrowser->addKeyListener(this);
    addAndMakeVisible(cartBrowser);
    
    cartBrowser->setBounds(23, 18, 590, 384);
    cartBrowser->setDragAndDropDescription("Sysex Browser");
    cartBrowser->addListener(this);
            
    addAndMakeVisible(closeButton = new TextButton("CLOSE"));
    closeButton->setBounds(4, 545, 50, 30);
    closeButton->addListener(this);

    addAndMakeVisible(loadButton = new TextButton("LOAD"));
    loadButton->setBounds(52, 545, 50, 30);
    loadButton->addListener(this);
    
    addAndMakeVisible(saveButton = new TextButton("SAVE"));
    saveButton->setBounds(100, 545, 50, 30);
    saveButton->addListener(this);

    addAndMakeVisible(fileMgrButton = new TextButton("SHOW DIR"));
    fileMgrButton->setBounds(148, 545, 70, 30);
    fileMgrButton->addListener(this);
/*
 *
 * I've removed this since it only works on the DX7 II. TBC.
 *

    addAndMakeVisible(getDXPgmButton = new TextButton("GET DX7 PGM"));
    getDXPgmButton->setBounds(656, 545, 100, 30);
    getDXPgmButton->addListener(this);
            
    addAndMakeVisible(getDXCartButton = new TextButton("GET DX7 CART"));
    getDXCartButton->setBounds(755, 545, 100, 30);
    getDXCartButton->addListener(this);
*/
}

CartManager::~CartManager() {
    timeSliceThread->stopThread(500);
    delete cartBrowser;
    delete cartBrowserList;
    delete timeSliceThread;
}

void CartManager::paint(Graphics &g) {
    g.fillAll(DXLookNFeel::lightBackground);
    g.setColour(DXLookNFeel::roundBackground);
    g.fillRoundedRectangle(8, 418, 843, 126, 15);
    g.setColour(Colours::whitesmoke);
    g.drawText("currently loaded cartridge", 38, 410, 150, 40, Justification::left);
}

void CartManager::programSelected(ProgramListBox *source, int pos) {
    if ( source == activeCart ) {
        browserCart->setSelected(-1);
        mainWindow->processor->setCurrentProgram(pos);
        mainWindow->processor->updateHostDisplay();
    } else {
        uint8_t unpackPgm[161];
        source->getCurrentCart().unpackProgram(unpackPgm, pos);
        activeCart->setSelected(-1);
        browserCart->setSelected(pos);
        repaint();
        mainWindow->processor->updateProgramFromSysex((uint8_t *) unpackPgm);
        mainWindow->processor->updateHostDisplay();
    }
}

void CartManager::buttonClicked(juce::Button *buttonThatWasClicked) {
    if ( buttonThatWasClicked == closeButton ) {
        mainWindow->startTimer(100);
        setVisible(false);
        return;
    }
    
    if ( buttonThatWasClicked == loadButton ) {
        FileChooser fc ("Import original DX sysex...", File::nonexistent, "*.syx;*.SYX;*.*", 1);
        
        if ( fc.browseForFileToOpen())
            mainWindow->loadCart(fc.getResult());
        return;
    }
    
    if ( buttonThatWasClicked == saveButton ) {
        mainWindow->saveCart();
    }
    
    if ( buttonThatWasClicked == fileMgrButton ) {
        cartDir.revealToUser();
        return;
    }

    // THIS IS NOT USED
    if ( buttonThatWasClicked == getDXPgmButton ) {
        if ( mainWindow->processor->sysexComm.isInputActive() && mainWindow->processor->sysexComm.isOutputActive() ) {
            unsigned char msg[] = { 0xF0, 0x43, 0x20, 0x09, 0xF7 };
            mainWindow->processor->sysexComm.send(MidiMessage(msg, 5));
        } else {
            showSysexConfigMsg();
        }
        return;
    }
    
    if ( buttonThatWasClicked == getDXCartButton ) {
        if ( mainWindow->processor->sysexComm.isInputActive() && mainWindow->processor->sysexComm.isOutputActive() ) {
            unsigned char msg[] = { 0xF0, 0x43, 0x20, 0x00, 0xF7 };
            mainWindow->processor->sysexComm.send(MidiMessage(msg, 5));
        } else {
            showSysexConfigMsg();
        }
        return;
    }
}

void CartManager::fileDoubleClicked(const File& file) {
    if ( file.isDirectory() )
        return;
    mainWindow->loadCart(file);
    activeCart->setCartridge(mainWindow->processor->currentCart);
}

void CartManager::fileClicked(const File& file, const MouseEvent& e) {
    if ( e.mods.isRightButtonDown() || e.mods.isAnyModifierKeyDown() ) {
        PopupMenu menu;
        
        menu.addItem(1000, "Open location");
        if ( ! file.isDirectory() ) {
            menu.addItem(1010, "Send sysex cartridge to DX7");
        }
        menu.addSeparator();
        menu.addItem(1020, "Refresh");
        
        switch(menu.show()) {
        case 1000:
            file.revealToUser();
            break;
        case 1010 :
            mainWindow->processor->sendSysexCartridge(file);
            break;
        case 1020:
            cartBrowserList->refresh();
            break;
        }
        return;
    }
}

void CartManager::setActiveProgram(int idx, String activeName) {
    if ( activeCart->programNames[idx] == activeName ) {
        activeCart->setSelected(idx);
        browserCart->setSelected(-1);
    }
    activeCart->repaint();
}

void CartManager::resetActiveSysex() {
    activeCart->setCartridge(mainWindow->processor->currentCart);
}

void CartManager::selectionChanged() {
    File file = cartBrowser->getSelectedFile();

    if ( ! file.exists() )
        return;
    
    if ( file.isDirectory() )
        return;
    
    Cartridge browserSysex;
    int rc = browserSysex.load(file);
    if ( rc < 0 ) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Error", "Unable to open file");
        return;
    }
    
    if ( rc != 0 ) {
        browserCart->readOnly = true;
    } else {
        browserCart->readOnly = false;
    }
    browserCart->setSelected(-1);
    browserCart->setCartridge(browserSysex);
}

void CartManager::programRightClicked(ProgramListBox *source, int pos) {
    PopupMenu menu;
    
    menu.addItem(1000, "Send program '" + source->programNames[pos] + "' to DX7");
    
    if ( source == activeCart )
        menu.addItem(1010, "Send current sysex cartridge to DX7");

    switch(menu.show())  {
        case 1000:
            uint8_t unpackPgm[161];
            
            if ( source == activeCart ) {
                mainWindow->processor->currentCart.unpackProgram(unpackPgm, pos);
            } else {
                source->getCurrentCart().unpackProgram(unpackPgm, pos);
            }
            
            if ( mainWindow->processor->sysexComm.isOutputActive() ) {
                uint8_t msg[163];
                exportSysexPgm(msg, unpackPgm);
                mainWindow->processor->sysexComm.send(MidiMessage(msg, 163));
            }
            break;
            
        case 1010:
            mainWindow->processor->sendCurrentSysexCartridge();
            break;
    }

}

void CartManager::programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) {
    if ( destListBox == activeCart ) {
        char *sysex = mainWindow->processor->currentCart.getRawVoice();
        memcpy(sysex+(dest*128), packedPgm, 128);
        mainWindow->updateUI();
    } else {
        File file = cartBrowser->getSelectedFile();
        
        if ( ! file.exists() )
            return;
        
        if ( file.isDirectory() )
            return;
        if ( file.getSize() != 4104 && file.getSize() != 4096 )
            return;

        Cartridge cart;
        cart.load(file);
        memcpy(cart.getRawVoice()+(dest*128), packedPgm, 128);
        cart.saveVoice(file);
        browserCart->setCartridge(cart);
    }
}

void CartManager::initialFocus() {
    cartBrowser->grabKeyboardFocus();
}

bool CartManager::keyPressed(const KeyPress& key, Component* originatingComponent) {
    if ( key.getKeyCode() == 13 ) {
        File file = cartBrowser->getSelectedFile();
        if ( file.isDirectory() )
            return true;
        mainWindow->loadCart(file);
        activeCart->setCartridge(mainWindow->processor->currentCart);
        return true;
    }
    return false;
}

void CartManager::showSysexConfigMsg() {
    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Warning", "The DX7 midi interface is not configured correctly.\n\n"
            "These buttons are used to 'ask' the DX7 to send the current program/cartridge.\n\n" 
            "In order to use this correctly, you need to connect your midi in and midi out of your DX7 to a midi interface and configure this midi interface with the [PARM] dialog. THIS ONLY WORKS ON A DX7-II");
}

// unused stuff from FileBrowserListener
void CartManager::browserRootChanged (const File& newRoot) {}


