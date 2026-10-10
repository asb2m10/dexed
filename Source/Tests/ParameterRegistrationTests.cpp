/**
 * Regression test for the crash fixed alongside this file: DexedAudioProcessor must register
 * a real AudioProcessorParameter for every Ctrl/CtrlDX it owns (see initCtrl() in PluginParam.cpp),
 * or Ctrl::publishValue()'s JUCE-8-style gesture calls dereference a null parameter pointer.
 */

#include "../PluginProcessor.h"
#include "../PluginParam.h"

class DexedParameterRegistrationTests : public UnitTest {
public:
    DexedParameterRegistrationTests() : UnitTest("Dexed parameter registration", "Dexed") {}

    void runTest() override {
        DexedAudioProcessor processor;

        beginTest("getParameters() count matches getNumParameters()");
        expectEquals(processor.getParameters().size(), processor.getNumParameters());

        beginTest("every parameter index has a registered, non-null AudioProcessorParameter");
        for (int i = 0; i < processor.getNumParameters(); ++i) {
            expect(processor.getParameters()[i] != nullptr, "null parameter at index " + String(i));
        }

        beginTest("registered parameter IDs match the processor's existing legacy IDs");
        for (int i = 0; i < processor.getNumParameters(); ++i) {
            auto *param = dynamic_cast<HostedAudioProcessorParameter *>(processor.getParameters()[i]);
            expect(param != nullptr, "parameter at index " + String(i) + " is not a HostedAudioProcessorParameter");
            if (param == nullptr) continue;
            expectEquals(param->getParameterID(), processor.getParameterID(i));
        }

        beginTest("registered parameter values round-trip through the processor's legacy accessors");
        const int sampleIndices[] = { 0, processor.getNumParameters() / 2, processor.getNumParameters() - 1 };
        for (int i : sampleIndices) {
            auto *param = processor.getParameters()[i];
            if (param == nullptr) continue;
            expectEquals(param->getValue(), processor.getParameter(i));
        }
    }
};

static DexedParameterRegistrationTests dexedParameterRegistrationTests;
