/**
 *
 * Copyright (c) 2015 Pascal Gauthier.
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
        return file.getFileExtension().toLowerCase() == ".syx";
    }
    bool isDirectorySuitable(const File &file) const {
        return true;
    };
};

CartManager::CartManager(DexedAudioProcessorEditor *editor) : TopLevelWindow("CartManager", false),
        timeSliceThread("Cartridge Directory Scanner") {
            
    mainWindow = editor;
    cartDir = DexedAudioProcessor::dexedCartDir;            
            
    addAndMakeVisible(activeCart = new ProgramListBox("activepgm", 8));
    activeCart->setBounds(8, 430, 843, 100);
    activeCart->addListener(this);
            
    memset(browserSysex, 0, 4096);
    addAndMakeVisible(browserCart = new ProgramListBox("browserpgm", 2));
    browserCart->setBounds(635, 10, 210, 400);
    browserCart->addListener(this);
    
    // -------------------------
    syxFileFilter = new SyxFileFilter();
    cartBrowserList = new DirectoryContentsList(syxFileFilter, timeSliceThread);
    cartBrowserList->setDirectory(cartDir, true, true);
    timeSliceThread.startThread();
    cartBrowser = new FileTreeComponent(*cartBrowserList);
    addAndMakeVisible(cartBrowser);
    cartBrowser->setBounds(5, 10, 620, 400);
    cartBrowser->setDragAndDropDescription("Sysex Browser");
    cartBrowser->addListener(this);
    
    /*addAndMakeVisible(newButton = new TextButton("NEW"));
    newButton->setBounds(400, 540, 50, 30);*/
            
    addAndMakeVisible(closeButton = new TextButton("CLOSE"));
    closeButton->setBounds(10, 540, 50, 30);
    addAndMakeVisible(loadButton = new TextButton("LOAD"));
    loadButton->setBounds(58, 540, 50, 30);
    loadButton->addListener(this);
    addAndMakeVisible(saveButton = new TextButton("SAVE"));
    saveButton->setBounds(106, 540, 50, 30);
    saveButton->addListener(this);

    closeButton->addListener(this);
    addAndMakeVisible(fileMgrButton = new TextButton("SHOW DIR"));
    fileMgrButton->setBounds(154, 540, 70, 30);
    fileMgrButton->addListener(this);  
}

// 856, 571

void CartManager::paint(Graphics &g) {
    g.fillAll(DXLookNFeel::lightBackground);
    //g.setColour(Colours::black);
    //g.fillRect(0, 5, 859, 410);
}

void CartManager::programSelected(ProgramListBox *source, int pos) {
    if ( source == activeCart ) {
        browserCart->setSelected(-1);
        mainWindow->processor->setCurrentProgram(pos);
        mainWindow->processor->updateHostDisplay();
    } else {
        if ( browserSysex == nullptr )
            return;
        char unpackPgm[161];
        unpackProgramFromSysex(unpackPgm, browserSysex, pos);
        activeCart->setSelected(-1);
        browserCart->setSelected(pos);
        repaint();
        mainWindow->processor->updateProgramFromSysex((uint8_t *) unpackPgm);
        mainWindow->processor->updateHostDisplay();        
    }
}

void CartManager::buttonClicked(juce::Button *buttonThatWasClicked) {
    if ( buttonThatWasClicked == closeButton ) {
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
}

void CartManager::fileDoubleClicked(const File& file) {
    if ( file.isDirectory() )
        return;
    
    mainWindow->loadCart(file);
    activeCart->setCartridge(mainWindow->processor->sysex);
}

void CartManager::fileClicked(const File& file, const MouseEvent& e) {
    if ( ! e.mods.isLeftButtonDown() )
        return;
    
    if ( file.isDirectory() )
        return;
    
    String f = file.getFullPathName();
    uint8_t syx_data[4104];
    ifstream fp_in(f.toRawUTF8(), ios::binary);
    if (fp_in.fail()) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Error", "Unable to open: " + f);
         return;
    }
    
    fp_in.read((char *)syx_data, 4104);
    fp_in.close();
    memcpy(browserSysex, syx_data+6, 4096);
    int checksum = sysexChecksum(((char *) &browserSysex), 4096);
        
    if ( checksum != syx_data[4102] ) {
        String message = "Sysex import checksum doesnt match ";
        message << checksum << " != " << syx_data[4102];
        
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Error", message);
        return;
    }
        
    browserCart->setCartridge(browserSysex);
}

void CartManager::setActiveProgram(int idx) {
    activeCart->setSelected(idx);
    browserCart->setSelected(-1);
    activeCart->repaint();
}

void CartManager::resetActiveSysex() {
    activeCart->setCartridge(mainWindow->processor->sysex);
}

// unused stuff from FileBrowserListener
void CartManager::browserRootChanged (const File& newRoot) {}
void CartManager::selectionChanged() {}

