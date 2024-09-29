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

#ifndef CARTMANAGER_H_INCLUDED
#define CARTMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginData.h"
#include "ProgramListBox.h"
#include "PluginData.h"

class CartridgeFileDisplay : public Component {
    File file;
    float clickableArea = 0;
public:
    void setCartrdigeFile(File &file) {
        this->file = file;
        repaint();
    }

    void paint(Graphics& g) override {
        g.setColour(Colours::whitesmoke);
        if ( file.exists() ) {
            g.setFont(g.getCurrentFont().withStyle(Font::underlined));
            clickableArea = g.getCurrentFont().getStringWidthFloat(file.getFileName());
            g.drawText(file.getFileName(), 0, 0, getWidth(), getHeight(), Justification::right);
            g.setFont(g.getCurrentFont().withStyle(Font::plain));
            g.drawText("Based on cartridge: ", 0, 0, getWidth() - (clickableArea + 2), getHeight(), Justification::right);
        } else {
            g.drawText("[No reference to original cartridge]", 0, 0, getWidth(), getHeight(), Justification::right);
            clickableArea = 0;
        }
    }

    void mouseDown(const MouseEvent &event) override {
        if ( event.getMouseDownX() > getWidth() - clickableArea ) {
            if ( file.exists() )
                file.revealToUser();
        }
    }
};

class CartManager  : public Component, public Button::Listener, public DragAndDropContainer, public FileBrowserListener
    , public ProgramListBoxListener, public KeyListener {
    std::unique_ptr<TextButton> newButton;
    std::unique_ptr<TextButton> loadButton;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> closeButton;
    std::unique_ptr<TextButton> fileMgrButton;
    std::unique_ptr<TextButton> getDXPgmButton;
    std::unique_ptr<TextButton> getDXCartButton;

    std::unique_ptr<ProgramListBox> activeCart;
    std::unique_ptr<ProgramListBox> browserCart;

    std::unique_ptr<FileFilter> syxFileFilter;

    std::unique_ptr<FileTreeComponent> cartBrowser;
    std::unique_ptr<TimeSliceThread> timeSliceThread;
    std::unique_ptr<DirectoryContentsList> cartBrowserList;
    std::unique_ptr<CartridgeFileDisplay> activeCartName;

    File cartDir;

    DexedAudioProcessorEditor *mainWindow;

    void showSysexConfigMsg();

    std::vector<Component*> focusOrder;

public:
    CartManager(DexedAudioProcessorEditor *editor);
    virtual ~CartManager();
    void paint(Graphics& g) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    void selectionChanged() override;
    void fileClicked (const File& file, const MouseEvent& e) override;
    void fileDoubleClicked (const File& file) override;
    void browserRootChanged (const File& newRoot) override;

    void setActiveProgram(int idx, String activeName);
    void resetActiveSysex();

    void updateCartFilename();

    void resized() override;

    virtual void programSelected(ProgramListBox *source, int pos) override;
    virtual void programRightClicked(ProgramListBox *source, int pos) override;
    virtual void programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) override;
    virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;

    void initialFocus();
    void hideCartridgeManager();

    std::unique_ptr< ComponentTraverser> createKeyboardFocusTraverser() override;
};

#endif  // CARTMANAGER_H_INCLUDED
