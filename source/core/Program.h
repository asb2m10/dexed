#pragma once

#include "parameter/DexedApvts.h"

uint8_t sysexChecksum(const uint8_t *sysex, int size);

/**
 * Programs are designed to be used as immutable objects.
 *
 * Only DexedAudioProcessor is allowed to modify.
 */
class Program {
public:
    static constexpr int PROGRAM_SIZE = 155;
    const uint8_t ALL_OPERATORS_ON = 63;

    Program() {
        memset(data, 0, PROGRAM_SIZE);
        data[155] = ALL_OPERATORS_ON;
    }

    Program(const uint8_t *data) {
        memcpy(this->data, data, PROGRAM_SIZE);
        this->data[155] = ALL_OPERATORS_ON;
    }

    Program(const juce::MidiMessage message);

    Program& operator=(const Program& other) {
        memcpy(data, other.data, PROGRAM_SIZE + 1);
        return *this;
    }

    juce::String getName() const {
        return normalizePgmName(reinterpret_cast<const char *>(data + 145));
    }

    void setName(const juce::String &name);

    const uint8_t& operator[](int offset) const {
        jassert(offset >= 0 && offset < PROGRAM_SIZE);
        return data[offset];
    }

    const uint8_t *getParameters(int offset) const {
        jassert(offset >= 0 && offset < PROGRAM_SIZE);
        return data + offset;
    }

    const uint8_t *getRawData() const {
        return data;
    }

    /**
     * Push current program data into the given AudioProcessorValueTreeState
     * @param apvts
     */
    void pushToParameters(const DexedApvts &apvts) const;

    juce::MidiMessage toMidiMessage(int targetChannel = 0);

    /**
     * This will apply the initial program data to preset (sine wave)
     */
    static Program initialProgram();
    static juce::String normalizePgmName(const char *sysexName);

    friend class DexedAudioProcessor;

private:
    // We add the 155 byte of operator switch
    uint8_t data[PROGRAM_SIZE+1];

    uint8_t &operator[](int offset) {
        jassert(offset >= 0 && offset < PROGRAM_SIZE+1);
        return data[offset];
    }

    [[deprecated]]
    uint8_t *getUnpackedData() const {
        return const_cast<uint8_t*>(data);
    }
};
