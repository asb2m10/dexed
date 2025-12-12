/**
 *
 * Copyright (c) 2014-2025 Pascal Gauthier.
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

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "parameter/DexedApvts.h"
#include "parameter/Model.h"
#include "Dexed.h"

class AlgoDisplay : public juce::Component {
    juce::CachedValue<int> algoValue;
    juce::CachedValue<bool> opSwitch[6];

    void displayOp(juce::Graphics &g, char id, int x, int y, char link, char fb);
public:
    AlgoDisplay(DexedApvts &apvts) {
        apvts.attachTo(IDs::algorithm.name, algoValue);
        for(int i=0;i<6;i++) {
            apvts.attachTo(IDs::on.op(i).name, opSwitch[i]);
        }
    }
    
    void paint(juce::Graphics &g) override;
};
