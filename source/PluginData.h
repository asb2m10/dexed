/**
 *
 * Copyright (c) 2014-2016 Pascal Gauthier.
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

#ifndef PLUGINDATA_H_INCLUDED
#define PLUGINDATA_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <stdint.h>
#include <string.h>
#include "Dexed.h"
#include <core/Cartridge.h>

class DexedClipboard {
    // In the future complete sysex will be supported
    uint8_t content[SYSEX_SIZE];
    int size = 0;
public:
    DexedClipboard() {
        String text = SystemClipboard::getTextFromClipboard();
        // For now we only support operator clipboard
        if ( text.length() < (21*2) ) {
            TRACE("Clipboard is empty or too small");
            return;
        }

        CharPointer_UTF8 t = text.getCharPointer();
        for(int i=0;i<21;i++) {
            int high = CharacterFunctions::getHexDigitValue(t.getAndAdvance());
            if ( high == -1 ) {
                TRACE("Malformed clipboard data");
                return;
            }
        
            int low  = CharacterFunctions::getHexDigitValue(t.getAndAdvance());
            if ( low == -1 ) {
                TRACE("Malformed clipboard data");
                return;
            }

            content[i] = (high << 4) + low;
        }
        size = 21;
    }

    DexedClipboard(const uint8_t *data, int s) {
        size = s;
        memcpy(content, data, size);
    }

    bool isOperatorData() {
        if ( size == 21 ) 
            return true;
        else
            return false;
    }

    uint8_t *getRawData() {
        return content;
    }

    void write(String description) {
        String clipboardValue = String::toHexString(content, size, 0);
        if ( description.isNotEmpty() ) {
            clipboardValue.append("\n; ", 3);
            clipboardValue.append(description, 1024);
        }
        SystemClipboard::copyTextToClipboard(clipboardValue);
    }
};

#endif  // PLUGINDATA_H_INCLUDED
