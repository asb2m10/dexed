#pragma once

#include "MetaParameter.h"

#define DECLARE_ID(name) const juce::Identifier name (#name);
#define DECLARE_META(name) const MetaParameterID name (#name);
#define DECLARE_METADX(name, pos, displayOffset) const MetaParameterID name (#name, (pos), (displayOffset));

namespace IDs {
    DECLARE_ID(root)
        DECLARE_ID(version)

        DECLARE_ID(parameters)
            DECLARE_METADX(pitchEgRate, 126, 0)
            DECLARE_METADX(pitchEgLevel, 130, 0)
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
            DECLARE_METADX(transpose, 144, -24)
            DECLARE_METADX(on, 155, 0)  // Packed binary flags for operators

                // Parameters based on operators (1-6)
                DECLARE_METADX(egRate, 0, 0)
                DECLARE_METADX(egLevel, 4, 0)
                DECLARE_METADX(breakpoint, 8, -8)
                DECLARE_METADX(lScaleDepth, 9, 0)
                DECLARE_METADX(rScaleDepth, 10, 0)
                DECLARE_METADX(lKeyScale, 11, 0)
                DECLARE_METADX(rKeyScale, 12, 0)
                DECLARE_METADX(rateScaling, 13, 0)
                DECLARE_METADX(ampModeSens, 14, 0)
                DECLARE_METADX(keyVelocity, 15, 0)
                DECLARE_METADX(outputLevel, 16, 0)
                DECLARE_METADX(mode, 17, 0)
                DECLARE_METADX(frequencyCoarse, 18, 0)
                DECLARE_METADX(frequencyFine, 19, 0)
                DECLARE_METADX(detune, 20, 7)

            DECLARE_META(output)
            DECLARE_META(monoMode)
            DECLARE_META(masterTuneAdj)
            DECLARE_META(cutoff)
            DECLARE_META(resonance)
            DECLARE_META(pitchBendUp)
            DECLARE_META(pitchBendDown)
            DECLARE_META(pitchBendStep)

            DECLARE_META(modWheel)
            DECLARE_META(modWheelPitch)
            DECLARE_META(modWheelAmp)
            DECLARE_META(modWheelEgBias)

            DECLARE_META(modFoot)
            DECLARE_META(modFootPitch)
            DECLARE_META(modFootAmp)
            DECLARE_META(modFootEgBias)

            DECLARE_META(modBreath)
            DECLARE_META(modBreathPitch)
            DECLARE_META(modBreathAmp)
            DECLARE_META(modBreathEgBias)

            DECLARE_META(modAftertouch)
            DECLARE_META(modAftertouchPitch)
            DECLARE_META(modAftertouchAmp)
            DECLARE_META(modAftertouchEgBias)

        DECLARE_ID(profile)
            DECLARE_ID(zoom)
            DECLARE_ID(showKeyboard)
            DECLARE_ID(midiCCMappings)
                DECLARE_ID(midiCCMapping)
                    DECLARE_ID(midiCC)
                    DECLARE_ID(midiParameterName)

        // This should be public for parameter attributes
        DECLARE_ID(id)
        DECLARE_ID(value)
}
