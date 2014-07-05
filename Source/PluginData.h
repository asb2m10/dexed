/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#ifndef PLUGINDATA_H_INCLUDED
#define PLUGINDATA_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#define SYSEX_SIZE 4104


enum UnpackedOffset {
	egRate,
	egLevel = 4,
	breakpoint = 8,
	lScaleDepth,
	rScaleDepth,
	lKeyScale,
	rKeyScale,
	rateScaling,
	keyVelocity,
	outputLevel,
	mode,
	fCoarse,
	fFine, 
	oscDetune,

	// Global values
	pitchEgRate = 126,
	pitchEgLevel = 130,
	algorythm = 134,
	feedback,
	oscKeySync,
	lfoSpeed,
	lfoDelay,
	lfoPmDepth,
	lfoAmDepth,
	lfoKeySync,
	lfoWave,
	middleC,
	pModeSens,
    osc6state
};

String normalizeSysexName(const char *sysexName);
void extractProgramNames(const char *block, StringArray &dest);
void exportSysexCart(char *dest, char *src, char sysexChl);
void exportSysexPgm(char *dest, char *src, char sysexChl);
void packProgram(uint8_t *dest, uint8_t *src, int idx, String name);

class CartridgeManager {
    ScopedPointer<ZipFile> builtin_pgm;
public:
	StringArray cartNames;

	CartridgeManager();
	void getSysex(int idx, char *data);
};

#endif  // PLUGINDATA_H_INCLUDED
