#include "Program.h"
#include "Dexed.h"

void Program::applyInitialProgram() {
    const char init_voice[] =
    { 99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
      99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
      99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
      73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };

    for(int i=0;i<sizeof(init_voice);i++) {
        data[i] = init_voice[i];
    }
}

void Program::pushToParameters(const DexedApvts &apvts) const {
    //juce::ScopedValueSetter<bool> setter(apvts.pushToParameterInProgress, true);

    uint8_t check[PROGRAM_SIZE];
    memcpy(check, data, PROGRAM_SIZE);

    for (const auto &param : dxParameters) {
        juce::RangedAudioParameter *parameter = apvts.getParameter(param.name);
        if ( parameter != nullptr ) {
            uint8_t value = data[param.pos];
            float displayValue = static_cast<float>(value - param.displayOffset);
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(parameter->convertTo0to1(displayValue));
            parameter->endChangeGesture();
        }
    }

    for (const auto &param : dxParameters) {
        if ( check[param.pos] != data[param.pos] ) {
            TRACE("Warning : parameter(%s[%d]) %d -> %d changed during pushToParameters()", param.name.toRawUTF8(), param.pos, check[param.pos], data[param.pos]);
        }
    }
}

juce::String Program::normalizePgmName(const char *sysexName) {
    char buffer[11];

    memcpy(buffer, sysexName, 10);

    for (int j = 0; j < 10; j++) {
        char c = (unsigned char) buffer[j];
        c &= 0x7F; // strip don't care most-significant bit from name
        switch (c) {
            case 92:
                c = 'Y';
                break; /* yen */
            case 126:
                c = '>';
                break; /* >> */
            case 127:
                c = '<';
                break; /* << */
            default:
                if (c < 32 || c > 127)
                    c = 32;
                break;
        }
        buffer[j] = c;
    }
    buffer[10] = 0;

    return juce::String(buffer);
}
