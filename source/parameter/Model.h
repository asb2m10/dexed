#pragma once

#include "MetaParameter.h"

#define DECLARE_ID(name) const juce::Identifier name (#name);
#define DECLARE_META(name) const MetaParameterID name (#name);

namespace IDs {
    DECLARE_ID(root)
        DECLARE_ID(version)

        DECLARE_ID(parameters)
            DECLARE_META(output)
            DECLARE_META(algorithm)
            DECLARE_META(feedback)
            DECLARE_META(tune)
            DECLARE_META(monoMode)
            DECLARE_META(lfoRate)
            DECLARE_META(lfoDelay)
            DECLARE_META(lfoKeySync)
            DECLARE_META(lfoWaveform)
            DECLARE_META(lfoAmpDepth)
            DECLARE_META(lfoPitchDepth)
            DECLARE_META(oscSync)
            DECLARE_META(transpose)
            DECLARE_META(pitchModSens)
            DECLARE_META(pitchEgRate)
            DECLARE_META(pitchEgLevel)

                // Parameters based on operators (1-6)
                DECLARE_META(outputLevel)
                DECLARE_META(on)
                DECLARE_META(egRate)
                DECLARE_META(egLevel)

        // This should be public for parameter attributes
        DECLARE_ID(offset)
        DECLARE_ID(displayValue)
        DECLARE_ID(id)
        DECLARE_ID(value)
}
