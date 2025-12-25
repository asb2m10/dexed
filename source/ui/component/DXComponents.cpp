/**
 *
 * Copyright (c) 2014-2018 Pascal Gauthier.
 *
 * DX7 Envelope Tables from legasynth-0.4.1 / DX7 Patch Editor
 * Copyright (C) 2002 Juan Linietsky <coding@reduz.com.ar>
 * Copyright (C) 2006 Mark-André Hopf <mhopf@mark13.org>
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

#include "Dexed.h"
#include "DXComponents.h"
#include "../DXLookNFeel.h"
#include "PluginProcessor.h"
#include "EnvDisplay.h"

static void comboBoxPopupMenuFinishedCallback(int result, ComboBoxImage* combo) {
    if (combo != nullptr) {
        combo->hidePopup();

        if (result != 0)
            combo->setSelectedId (result);
    }
}

ComboBoxImage::ComboBoxImage() {
    itemPos[0] = -1;
}

void ComboBoxImage::paint(Graphics &g) {
    int idx = getSelectedItemIndex();
    if ( itemPos[0] != -1 ) {
        if ( idx < 4 )
            idx = itemPos[idx];
    }
    
    g.drawImage(items, 0, 0, items.getWidth(), itemHeight, 0, idx * itemHeight, items.getWidth(), itemHeight);
}

void ComboBoxImage::showPopup() {
    popup.showMenuAsync (PopupMenu::Options().withTargetComponent (this)
                         .withItemThatMustBeVisible(getSelectedId())
                         .withMinimumWidth(getWidth())
                         .withMaximumNumColumns(1)
                         .withStandardItemHeight(itemHeight),
                         ModalCallbackFunction::forComponent(comboBoxPopupMenuFinishedCallback, this));
}

void ComboBoxImage::setImage(Image image) {
    items = image;
    
    int numItems = getNumItems();
    itemHeight = image.getHeight() / numItems;

    for(int i=0;i<numItems;i++) {
        Image tmp = image.getClippedImage(Rectangle<int>(0,itemHeight*i, image.getWidth(), itemHeight));
        popup.addItem(i+1, getItemText(i), true, false, tmp);
    }
}

void ComboBoxImage::setImage(Image image, int pos[]) {
    items = image;

    int numItems = getNumItems();
    itemHeight = 26;

    for(int i=0;i<numItems;i++) {
        Image tmp = image.getClippedImage(Rectangle<int>(0,itemHeight*pos[i], image.getWidth(), itemHeight));
        popup.addItem(i+1, getItemText(i), true, false, tmp);
    }

    for(int i=0;i<4;i++)
        itemPos[i] = pos[i];
}

void ProgramSelector::mouseDown(const MouseEvent &event) {
    if ( event.x < getWidth() - 8) {
        ComboBox::mouseDown(event);
        return;
    }
    
    int cur = getSelectedItemIndex();
    if ( event.y < getHeight() / 2 ) {
        if ( cur == 0 )
            cur = 31;
        else
            cur--;
    } else {
        if ( cur == 31 )
            cur = 0;
        else
            cur++;
    }
    setSelectedItemIndex(cur);
}

void ProgramSelector::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {
    accum_wheel += wheel.deltaY;

    float wheelFactor = 0.2; // higher is slower
#if JUCE_WINDOWS
    wheelFactor = 0.4;
#endif    
    
    bool up = (accum_wheel > wheelFactor);
    bool dn = (accum_wheel < -wheelFactor);
    int sn = 1;
#if ! JUCE_MAC
    auto t = up;
    up = dn;
    dn = t;
    sn = -1;
#endif    
    
    if( up )
    {
        accum_wheel -= sn * wheelFactor;
        int c = getSelectedItemIndex();
        if( c == 31 ) c = 0;
        else c++;
        setSelectedItemIndex(c);
    }
    else if( dn )
    {
        accum_wheel += sn * wheelFactor;
        int c = getSelectedItemIndex();
        if( c == 0 ) c = 31;
        else c--;
        setSelectedItemIndex(c);
    }
}

void ProgramSelector::paint(Graphics &g) {
    int x = getWidth();
    int y = getHeight();
    
    Path path;
    path.addTriangle(x-8, y/2-1, x-4, 2,   x, y/2-1);
    path.addTriangle(x-8, y/2+1, x-4, y-2, x, y/2+1);
    
    g.setColour(Colours::white);
    g.fillPath(path);
}
