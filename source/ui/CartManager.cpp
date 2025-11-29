/**
 *
 * Copyright (c) 2015-2024 Pascal Gauthier.
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

/**
 * This is a hack to override global dexed focus traversal when the cart manager is shown. We could have used a 
 * modal window but this would have blocked the keys and the user would not have been able to play the current
 * selected preset. Anything that is keyboard related should be ordered in the focusOrder vector.
 */
class CartBrowserFocusTraverser : public KeyboardFocusTraverser {
    std::vector<Component*> &orders;
    Component *root;
public:
    CartBrowserFocusTraverser(Component *root, std::vector<Component*> &orders) : orders(orders), root(root) {}

    Component* getDefaultComponent(Component* parentComponent) override {
        return orders[0];
    }

    Component* getNextComponent(Component* current) override {
        bool srcFound = false;
        int i;

        for (i=0;i<orders.size();i++) {
            if ( orders[i] == current ) {
                srcFound = true;
                continue;
            }

            if ( srcFound ) {
                ProgramLabel *label = dynamic_cast<ProgramLabel*>(orders[i]);
                if ( label != nullptr && !label->isActive() )
                    continue;
                break;
            }
        }

        if ( i == orders.size() )
            return orders.front();
        return orders[i];
    }

    Component* getPreviousComponent(Component* current) override {
        bool srcFound = false;
        int i=0;

        for(i=orders.size()-1;i>=0;i--) {
            if ( orders[i] == current ) {
                srcFound = true;
                continue;
            }
            if ( srcFound ) {
                ProgramLabel *label = dynamic_cast<ProgramLabel*>(orders[i]);
                if ( label != nullptr && !label->isActive() )
                    continue;
                break;
            }
        }
        if ( i == -1 )
            return orders.back();
        return orders[i];
    }

    std::vector<Component*> getAllComponents(Component* parentComponent) override {
        return orders;
    }
};

CartManager::CartManager(DexedAudioProcessorEditor *editor) : Component("CartManager") {
    mainWindow = editor;
    cartDir = DexedAudioProcessor::dexedCartDir;

    activeCart.reset(new ProgramListBox("Active Programs Selector", 8));
    addAndMakeVisible(activeCart.get());
    activeCart->addListener(this);

    browserCart.reset(new ProgramListBox("Browser Programs Selector", 2));
    addAndMakeVisible(browserCart.get());
    browserCart->addListener(this);

    // -------------------------
    syxFileFilter.reset(new SyxFileFilter());
    timeSliceThread.reset(new TimeSliceThread("Cartridge Directory Scanner"));
    timeSliceThread->startThread();

    cartBrowserList.reset(new DirectoryContentsList(syxFileFilter.get(), *timeSliceThread.get()));
    cartBrowserList->setDirectory(cartDir, true, true);
    cartBrowser.reset(new FileTreeDrop(*cartBrowserList));
    cartBrowser->addKeyListener(this);
    addAndMakeVisible(cartBrowser.get());

    cartBrowser->setTitle("Cartridge file browser");
    cartBrowser->setDragAndDropDescription("Sysex Browser");
    cartBrowser->addListener(this);

    closeButton.reset(new TextButton("CLOSE"));
    addAndMakeVisible(closeButton.get());
    closeButton->addListener(this);

    loadButton.reset(new TextButton("LOAD"));
    addAndMakeVisible(loadButton.get());
    loadButton->addListener(this);

    saveButton.reset(new TextButton("SAVE"));
    addAndMakeVisible(saveButton.get());
    saveButton->addListener(this);

    fileMgrButton.reset(new TextButton("SHOW DIR"));
    addAndMakeVisible(fileMgrButton.get());
    fileMgrButton->addListener(this);

    activeCartName.reset(new CartridgeFileDisplay());
    addAndMakeVisible(activeCartName.get());

    focusOrder.push_back(cartBrowser.get());
    //focusOrder.push_back(browserCart.get());
    for(int i=0;i<32;i++) {
        focusOrder.push_back(browserCart->getProgramComponent(i));
    }
    //focusOrder.push_back(activeCart.get());
    for(int i=0;i<32;i++) {
        focusOrder.push_back(activeCart->getProgramComponent(i));
    }
    focusOrder.push_back(closeButton.get());
    focusOrder.push_back(loadButton.get());
    focusOrder.push_back(saveButton.get());
    focusOrder.push_back(fileMgrButton.get());

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
    cartBrowser.reset(NULL);
    cartBrowserList.reset(NULL);
}

std::unique_ptr<ComponentTraverser> CartManager::createKeyboardFocusTraverser() {
    return std::make_unique<CartBrowserFocusTraverser>(this, focusOrder);
}

void CartManager::resized() {
    float activeSize = 100;

    activeCart->setBounds(14, 402, activeSize * 8, 96);
    browserCart->setBounds(activeSize * 6 + 15, 10, activeSize * 2, 385);
    cartBrowser->setBounds(14, 10, activeSize * 6 - 4, 385);
    closeButton->setBounds(4, getHeight() - 40, 70, 30);
    saveButton->setBounds(144, getHeight() - 40, 70, 30);
    loadButton->setBounds(74, getHeight() - 40, 70, 30);
    fileMgrButton->setBounds(214, getHeight() - 40, 90, 30);
    activeCartName->setBounds(getWidth() - 8 - 300, getHeight() - 40, 300, 30);
}

void CartManager::paint(Graphics &g) {
    g.fillAll(DXLookNFeel::lightBackground);
}

void CartManager::updateCartFilename() {
    activeCartName->setCartrdigeFile(mainWindow->processor->activeFileCartridge);
}

void CartManager::programSelected(ProgramListBox *source, int pos) {
    if ( source == activeCart.get() ) {
        browserCart->setActive(-1);
        mainWindow->processor->setCurrentProgram(pos);
        mainWindow->processor->updateHostDisplay();
    } else {
        uint8_t unpackPgm[161];
        source->getCurrentCart().unpackProgram(unpackPgm, pos);
        activeCart->setActive(-1);
        browserCart->setActive(pos);
        repaint();
        mainWindow->processor->updateProgramFromSysex((uint8_t *) unpackPgm);
        mainWindow->processor->updateHostDisplay();
    }
}

void CartManager::buttonClicked(juce::Button *buttonThatWasClicked) {
    if ( buttonThatWasClicked == closeButton.get() ) {
        hideCartridgeManager();
        return;
    }

    if ( buttonThatWasClicked == loadButton.get() ) {
        FileChooser fc ("Import original DX sysex...", File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory), "*.syx;*.SYX;*.*", 1);

        if ( fc.browseForFileToOpen()) {
            mainWindow->loadCart(fc.getResult());
            updateCartFilename();
        }
        return;
    }

    if ( buttonThatWasClicked == saveButton.get() ) {
        mainWindow->saveCart();
    }

    if ( buttonThatWasClicked == fileMgrButton.get() ) {
        cartDir.revealToUser();
        return;
    }

    // THIS IS NOT USED
    if ( buttonThatWasClicked == getDXPgmButton.get() ) {
        if ( mainWindow->processor->sysexComm.isInputActive() && mainWindow->processor->sysexComm.isOutputActive() ) {
            unsigned char msg[] = { 0xF0, 0x43, 0x20, 0x09, 0xF7 };
            mainWindow->processor->sysexComm.send(MidiMessage(msg, 5));
        } else {
            showSysexConfigMsg();
        }
        return;
    }

    if ( buttonThatWasClicked == getDXCartButton.get() ) {
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
    updateCartFilename();
}

void CartManager::fileClicked(const File& file, const MouseEvent& e) {
    if ( e.mods.isPopupMenu()) {
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
        activeCart->setActive(idx);
        browserCart->setActive(-1);
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
    browserCart->setActive(-1);
    browserCart->setCartridge(browserSysex);
}

void CartManager::programRightClicked(ProgramListBox *source, int pos) {
    PopupMenu menu;

    menu.addItem(1000, "Send program '" + source->programNames[pos] + "' to DX7");

    if ( source == activeCart.get() )
        menu.addItem(1010, "Send current sysex cartridge to DX7");

    switch(menu.show())  {
        case 1000:
            uint8_t unpackPgm[161];

            if ( source == activeCart.get() ) {
                mainWindow->processor->currentCart.unpackProgram(unpackPgm, pos);
            } else {
                source->getCurrentCart().unpackProgram(unpackPgm, pos);
            }

            if ( mainWindow->processor->sysexComm.isOutputActive() ) {
                uint8_t msg[163];
                exportSysexPgm(msg, unpackPgm);
                msg[2] |= mainWindow->processor->sysexComm.getChl();
                mainWindow->processor->sysexComm.send(MidiMessage(msg, 163));
            }
            break;

        case 1010:
            mainWindow->processor->sendCurrentSysexCartridge();
            break;
    }
}

void CartManager::programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) {
    if ( destListBox == activeCart.get() ) {
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
        cart.replaceProgram(dest, packedPgm);;
        cart.saveVoice(file);
        browserCart->setCartridge(cart);
    }
}

void CartManager::initialFocus() {
    cartBrowser->grabKeyboardFocus();
}

void CartManager::hideCartridgeManager() {
        mainWindow->startTimer(100);
        getParentComponent()->setVisible(false);
}

bool CartManager::keyPressed(const KeyPress& key, Component* originatingComponent) {
    if ( key.getKeyCode() == KeyPress::returnKey ) {
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
