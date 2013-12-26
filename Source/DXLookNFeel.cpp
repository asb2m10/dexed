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

#include "DXLookNFeel.h"
#include "PluginProcessor.h"

void EnvDisplay::paint(Graphics &g) {
	int rate[4];
	int level[4];

	for(int i=0;i<4;i++) {
		rate[i] = s_rate[i]->getValue();
		level[i] = s_level[i]->getValue();
	}

	env.init(rate, level, 99 << 5, 0);
    env.keydown(true);
	for (int i=0;i<72;i++) {
		int32_t pos = env.getsample();
		for(int j=0;j<16;j++) {
			env.getsample();
		}
		g.setPixel(i, 32 - (sqrt(pos) / 512));
	}
    env.keydown(false);
	for (int i=0;i<24;i++) {
		int32_t pos = env.getsample();
		for(int j=0;j<16;j++) {
			env.getsample();
		}
		g.setPixel(i+72,  32 - (sqrt(pos) / 512));
	}
}

void AlgoDisplay::updateUI() {

}

void AlgoDisplay::paint(Graphics &g) {

}

DXLookNFeel::DXLookNFeel() {
	setColour(TextButton::buttonColourId,Colour(0xFF0FC00F));
	setColour(Slider::rotarySliderOutlineColourId,Colour(0xFF0FC00F));
	setColour(Slider::rotarySliderFillColourId,Colour(0xFFFFFFFF));
}
