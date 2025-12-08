#pragma once

#include "MetaParameter.h"

#define DECLARE_ID(name) const juce::Identifier name (#name);
#define DECLARE_META(name) const MetaParameterID name (#name);
#define DECLARE_METADX(name, pos, displayOffset) const MetaParameterID name (#name, (pos), (displayOffset));

namespace IDs {
    DECLARE_ID(root)
        DECLARE_ID(version)

        DECLARE_ID(parameters)
            DECLARE_METADX(algorithm, 134, -1)
            DECLARE_METADX(feedback, 135, 0)
            DECLARE_METADX(oscKeySync, 136, 0)
            DECLARE_METADX(lfoRate, 137, 0)
            DECLARE_METADX(lfoDelay, 138, 0)
            DECLARE_METADX(lfoPmDepth, 139, 0)
            DECLARE_METADX(lfoAmpDepth, 140, 0)
            DECLARE_METADX(lfoKeySync, 141, 0)
            DECLARE_METADX(lfoWaveform, 142, 0)
            DECLARE_METADX(pitchModSens, 143, 0)
            DECLARE_METADX(pitchEgRate, 126, 0)
            DECLARE_METADX(pitchEgLevel, 138, 0)
            DECLARE_METADX(transpose, 144, 0)

                // Parameters based on operators (1-6)
                DECLARE_METADX(egRate, 0, 0)
                DECLARE_METADX(egLevel, 4, 0)
                DECLARE_METADX(breakpoint, 8, -8)
                DECLARE_METADX(lScaleDepth, 9, 0)
                DECLARE_METADX(rScaleDepth, 10, 0)
                DECLARE_METADX(lKeyScale, 11, 0)
                DECLARE_METADX(rKeyScale, 12, 0)
                DECLARE_METADX(rateScaling, 13, 0)
                DECLARE_METADX(aModSens, 14, 0)
                DECLARE_METADX(keyVelocity, 15, 0)
                DECLARE_METADX(outputLevel, 16, 0)
                DECLARE_METADX(mode, 17, 0)
                DECLARE_METADX(frequencyCoarse, 18, 0)
                DECLARE_METADX(frequencyFine, 19, 0)
                DECLARE_METADX(detune, 20, 7)
                DECLARE_META(on)

            DECLARE_META(output)
            DECLARE_META(tune)
            DECLARE_META(monoMode)
            DECLARE_META(masteTuneAdj)

        // This should be public for parameter attributes
        DECLARE_ID(id)
        DECLARE_ID(value)
}
