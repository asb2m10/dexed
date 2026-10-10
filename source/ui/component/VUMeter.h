/*
  ==============================================================================

    VUMeter.h
    Created: 29 Jun 2024 1:02:58pm
    Author:  Fulop Nandor

    
    Summary
    -------

    The class ``VuMeter`` is instantiated as solid-colored logarithmic level 
    indicators for the FM-operators  in ``OperatorEditor.cpp``, and the class 
    ``VuMeterOutput`` is instantiated as a tricolor VU meter (more precisely: 
    as a  logarithmic peak indicator with a leaky-integrator with about 300 
    ms falling time for -40 dB changes) for output level in ``GlobalEditor.cpp``.
    Both of them are derived from ``VuMeterBase`` class.
    Inside the ``paint()`` method of ``VuMeterBase`` class,  an ``index`` value 
    is calculated, based on an approximating decibel value of the proper 
    amplutude. When called, the ``paint()`` method draws a single stripe that
    contains exactly as many bright LEDs  as the value of the ``index`` variable.
    A solid-colored set and a tricolor set of images of LED-stripes, each 
    containing 0, 1, 2, ... etc. bright LEDs are created only once by the 
    ``VuStripesSingleton`` from image file ``Meter_140x8.png`` 
    containing a dark and a bright solid-color LED-stripes.
   

    Notes on a Quick Approximating Decibel Calculation
    --------------------------------------------------

    The use of any kind of float/double logarithm functions could be avoided
    for the dB-calculations by a substituion with a few integer operations 
    ("float-to-int punning"), if we satisfied with an appoximating dB-value 
    instead of the exact one, based on the following tricks:
    (1) The definition of the exact dB values related to the amplitude (not 
    the power!) is:
        dB = 10 * log10(amplitude);
    (2) Let us define the 2-based dB (hereinafter denoted as ``dB2``) by 
    replacing the ten-based ``log10()`` function by the two-base ``log2()`` 
    function, utilizing the fact that log10(2) is approximately 0.3010...:
        dB2 = 3 * log2(amplitude);
    (3) In the 4-byte sized floats, according to IEEE 754 standard, there 
    are 3 parts:
        1 bit for the sign
        8 bits for the 2-based exponent (biased by 127, see in case of the 
        value 1.0F)
        23 bits for the normalized mantissa (with a hidden 24th bit equals 
        to 1, by the definition).
    (4) Notice that if we are accepting of a 3 dB precision VU meter, this 
    exponent (disregarding its bias) provides an approximation of the decibel 
    values in the two-based logarithmic scale. So, let us introduce the 
    examination of mantissa parts to get 1 dB precision approximating decibel 
    value, defining the 1-dB-precision approximating decibel quantity, 
    ``dB21``, as follows:
        dB21 = 3 * exponent;
        if (msb_of_mantissa >= 66) {
            ++db21;
        if (msb_of_manstissa >= 150)
            ++dB21;
        }
    Here the 66 and the 150 are the decimal values of most significant 
    bytes of the mantissa parts in the normalized float values of the 
    2 to the power of 1/3 and 2/3, respectively, for the case of the 
    4-byte sized IEEE 754 floats.
    So the difference between two ``dB21`` values, calculated from a 
    reference amplitude and the actual amplitude, provides the approximating 
    value in a two-based logarithmic scale that can be interpreted as decibels. 
    The idea above is originated from comment of User Gwirk, see at:
    https://www.reddit.com/r/DSP/comments/65up2e/cheap_algorithm_for_vu_meter/?rdt=56174
    
    For the sake of the faster execution time,
    a) the multiplication by 3 is replaced by additions, and
    b) the nested if statements are replaced by a reading out from 
    look-up-table:
        db21 = exponent + exponent + exponent + mlut[msb_mantissa]
    where the elements 0...65 of ``mlut[]`` are filled by 0s,
    elements 66...149 are filled by 1s, and the elements 150...255 are 
    filled by 2s.

    This methods are implemented in member functions ``ampTodB21_*`` 
    of ``VuMeterBase`` class. Because the byte order itself of IEEE 754 float 
    values are not defined in the standard, the endianess of floats are 
    determined in the constuctor of ``VuMeterBase`` during runtime. 
    If the determination of the endianess failed, the usual float ``log()`` 
    function, otherwise the proper float-to-int-punning method above is 
    used. The float-to-int-punning based decibel calculation methods showed 
    30% faster execution than calculation based on calls to ``log()`` 
    function, in the following cases of CPUs:
    - Intel(R) Core(TM) i5-4460 @ 3.20GHz (MS Windows 10; MS Visual Studio 2022 Community)
    - Broadcom BCM2711 (Raspbian OS bookworm 64-bit; g++ Debian 12.2.0-14)
    according to the tests performed by Google Benchmark (https://github.com/google/benchmark)
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

// defintion of some structs and a union used for the float-to-int punning
#pragma pack(push)
#pragma pack(1)
typedef uint8_t b4_t[sizeof(float)];
typedef struct {// struct of individual bytes of an IEEE 754 float in Little Endian Float Systems
    uint8_t m0;
    uint8_t m1;
    uint8_t m2; // LS bit of exponent and 7 MS bits of the mantissa
    uint8_t e;  // exponent and sign
} b4LE_t;
typedef struct {// struct of individual bytes of an IEEE 754 float in Big Endian Float Systems
    uint8_t e;  // exponent
    uint8_t m2; // LS bit of the exponent and 7 MS bits of the mantisse
    uint8_t m1;
    uint8_t m0;
} b4BE_t;
typedef union {
    float f;    // access as a float
    uint32_t n; // access as a 32-bit unsigned integer
    b4_t b4;    // access as a 4 element byte array
    b4LE_t bLE; 
    b4BE_t bBE;
} u_fip_t;
#pragma pack(pop)


// Fundamental class to define VU Meters.
// Not intended to instantiate directly.
class VuMeterBase : public Component {
    // Paints an image of a single stripe addresses by the
    // index calculated by the index calculating method
    // set by its constructor.
    // (The pointers of the images of the single stripes 
    // to be drawn are stored in an array initialized by the 
    // ``VuStripesSingleton``, which is created from
    // the derived classes ``VuMeter`` and ``VuMeterOutput``.)
    void paint(Graphics& g);

    // the log()-function based stripe index calculator function
    int ampToStripeIndex_log(float amp);

    // the dB21 based decibel and stripe index calculator functions,
    // for Little Endian Float Systems
    inline int ampTodB21_le(float amp);
    int ampToStripeIndex_le(float amp);

    // the dB21 based decibel and stripe index calculator functions,
    // for Big Endian Float Systems
    inline int ampTodB21_be(float amp);
    int ampToStripeIndex_be(float amp);

public: 

    // additional parameters characterizing the original double-stripe image for VU meters
    // specified by ``BinaryData::Meter_140x8_png`` and ``BinaryData::Meter_140x8_pngSize``
    // and loaded in ``VuStripesSingleton::createStripes()`` from ``juce::ImageCache``.

    // total number of blocks ("LED"s) of a stripe
    static const int totalBlocks = 46;

    // dimensions of a single block (LED) in pixels (for the case of the 140x80 sized image)
    static const int blockWidth = 2;
    static const int blockGap = 1;
    /*
        // dimension of a single block in pixels (for the case of the 280x16 sized image)
        static const int blockWidth = 2*2;
        static const int blockGap = 1*2;
    */

    // dimension of the border
    static const int border = 1;

    // dimension of a block plus the size of a gap between two adjacent blocks
    static const int blockWidthWithGap = blockWidth + blockGap;

    // array of pointers to images of individual single stripes, 
    // each having 0, 1, 2, ... ``totalBlocks``  bright LEDs;
    // it is initialized by the derived classes ``VuMeter`` and
    // ``VuMeterOutput``.
    juce::Image** singleStripes;

    // minimum dB associated to leftmost LED;
    // (initialized now to default for the solid stripe)
    int mindB = -46;

    // minimum amplitude equivalent to mindB;
    // (initialized now to default for the solid stripe)
    float minamp = (float)pow(10.0, mindB * 0.1);

    // maximum amplitude;
    // (initialized now to default value for the solid stripe)
    float maxamp = 1.0F;

    // Calculates amplitude from dB value by the traditional way.
    float dB10ToAmp(float dB);

    float tenPerLog10 = (float)(10.0 / log(10.0));

    // Calculates amplitude value from a dB21 value.
    float dB2ToAmp(float dB2);

    // a bias value associated to ``mindB``
    // for the case of the approximating db21 values
    int indexBiasMindB2 = 0;

    // current amplitude value to be displayed in VU meter
    float v = 0.0F;

    VuMeterBase(int maxdB);
    ~VuMeterBase();

    // a pointer to the function to calculate index of the LED-stripe;
    // it might point to ``ampToStripeIndex_log``, ``ampToStripeIndex_le``,
    // or ``ampToStripeIndex_be``
    typedef int (VuMeterBase::* funcptr_t)(float);
    funcptr_t ampToStripeIndex;

    // lookup table for the mantissa;
    // used by the dB21-calculations
    static const uint8_t mlut[256];
};

// A singleton to contain an array of pointers to
// images of individual single LED-stripes.
class VuStripesSingleton
{
public:
    static VuStripesSingleton& getInstance()
    {
        static VuStripesSingleton instance;
        return instance;
    }

    // total number of LEDs in a stripe
    const static int numStripes = 47;
    int getNumStripes() { return numStripes; };

    juce::Image** getSolidVuMeterStripes() { return solidVuMeterStripes; };
        
    juce::Image** getTricolorVuMeterStripes() { return tricolorVuMeterStripes; };

    ~VuStripesSingleton();

private:
    VuStripesSingleton();

    // array of pointers to images of solid color single stripes
    juce::Image* solidVuMeterStripes[numStripes];

    // array of pointers of images of tricolor single stripes
    juce::Image* tricolorVuMeterStripes[numStripes];
    
    void createStripes(juce::Image** array, bool tricolor);
    void destroyStripes(juce::Image** array);
};

// Solid color VU Meter used to indicate output levels of the FM-operators
// (The reference amplidute is 1.0F, implicitely.)
class VuMeter : public VuMeterBase {
public:
    VuMeter();
};

// Tricolor VU Meter used for main output
// (The reference amplidute is 1.0F, implicitely.)
class VuMeterOutput : public VuMeterBase {
public:
    // number of blocks (LEDs) having different colors 
    static const int numRedBlocks = 6;
    static const int numYellowBlocks = 6;
    static const int numGreenBlocks = totalBlocks - numRedBlocks - numYellowBlocks;

    VuMeterOutput();

    static double getDecayFactor(double sampleRate);
};

//EOF