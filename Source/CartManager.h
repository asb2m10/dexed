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

#ifndef CARTMANAGER_H_INCLUDED
#define CARTMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginData.h"
#include "ProgramListBox.h"

class CartManager  : public Component, public ButtonListener, public DragAndDropContainer, public FileBrowserListener
    , public ProgramListBoxListener {
    ScopedPointer<TextButton> newButton;
    ScopedPointer<TextButton> loadButton;
    ScopedPointer<TextButton> saveButton;
    ScopedPointer<TextButton> closeButton;
    ScopedPointer<TextButton> fileMgrButton;
    ScopedPointer<TextButton> getDXPgmButton;
    ScopedPointer<TextButton> getDXCartButton;
    
    ScopedPointer<ProgramListBox> activeCart;
    ScopedPointer<ProgramListBox> browserCart;
    
    ScopedPointer<FileFilter> syxFileFilter;
        
    FileTreeComponent *cartBrowser;
    TimeSliceThread *timeSliceThread;
    DirectoryContentsList *cartBrowserList;
        
    File cartDir;
    
    DexedAudioProcessorEditor *mainWindow;

    void showSysexConfigMsg();

public:
    CartManager(DexedAudioProcessorEditor *editor);
    virtual ~CartManager();
    void paint(Graphics& g);
    void buttonClicked (Button* buttonThatWasClicked);
    
    void selectionChanged();
    void fileClicked (const File& file, const MouseEvent& e);
    void fileDoubleClicked (const File& file);
    void browserRootChanged (const File& newRoot);
        
    void setActiveProgram(int idx, String activeName);
    void resetActiveSysex();
        
    virtual void programSelected(ProgramListBox *source, int pos) override;
    virtual void programRightClicked(ProgramListBox *source, int pos) override;
    virtual void programDragged(ProgramListBox *destListBox, int dest, char *packedPgm) override;
        
    void initialFocus();
};


#endif  // CARTMANAGER_H_INCLUDED
