/*
  ==============================================================================

    VUMeter.cpp
    Created: 29 Jun 2024 1:03:22pm
    Author:  Fulop Nandor

  ==============================================================================
*/

#include <JuceHeader.h>

#include "VUMeter.h"

// Constructor: sets the index calculating function
// according to the determined endianess.
VuMeterBase::VuMeterBase(int maxdB) {
    mindB = maxdB - totalBlocks + 1;

    u_fip_t u;

    // a "magic float number"; in a Little Endian System, 
    // the values of its individual bytes are, in order:
    // 0x20 (LSB), 0x10, 0x08 and 0x40 (MSB).
    u.f = 2.125984191894531e+00F;

    if (u.b4[3] == 0x40 && u.b4[2] == 0x08) {
        // a Little Endian system identified;
        // set log-free faster index calculationg function
        ampToStripeIndex = &VuMeterBase::ampToStripeIndex_le;
        indexBiasMindB2 = ampTodB21_le(dB2ToAmp((float)mindB));
//DBG("VuMeterBase::VuMeterBase(): Little Endian Float System identified");
    }
    else if (u.b4[0] == 0x40 && u.b4[1] == 0x08) {
        // a Big Endian system identified
        // set log-free faster index calculationg function
        ampToStripeIndex = &VuMeterBase::ampToStripeIndex_be;
        indexBiasMindB2 = ampTodB21_be(dB2ToAmp((float)mindB));
//DBG("VuMeterBase::VuMeterBase(): Big Endian Float System identified");
    }
    else {
        // the endianess of floats is not identified;
        // fall back to function using the slower ``log()``
        ampToStripeIndex = &VuMeterBase::ampToStripeIndex_log;
        minamp = dB10ToAmp((float)mindB);
        maxamp = dB10ToAmp((float)maxdB);
//DBG("VuMeterBase::VuMeterBase(): Unidentified Endian Float System");
    }
}

// Calculates the index to be used to address array of images of stripes
// based on the exact decibel values (calls to ``log()``).
int VuMeterBase::ampToStripeIndex_log(float amp) {
    int index;

    amp = fabs(amp);

    // check amplitude, whether it is out of the range
    if (amp <= minamp) {
        index = 0;
    }
    else if (amp >= maxamp) {
        index = totalBlocks;
    }
    else {
        // index = (int) (10.0F * log10(amp)) - mindB;
        // Note: in some systems, the execution of ``log()`` is faster than ``log10()``,
        // while ``log10`` was never experienced faster than ``log()``  
        index = (int)(tenPerLog10 * log(amp)) - mindB;

        // against the perpetual paranoia related to rounding and /under/overflow of floats
        // (in principle, the following conditions should never be met... however, "the devil never sleeps").
        if (index < 0) index = 0;
        if (index > totalBlocks) index = totalBlocks;
    }

    return index;
};

// Calculates the approximating dB21 value
// in case of Little Endian Float Systems.
inline int VuMeterBase::ampTodB21_le(float amp) {
    // room for the float-int punning
    u_fip_t u_amp;

    // store the float amplitude value for the float-int punning
    u_amp.f = amp;

    // shift bits of exponent and mantissa to by boundaries,
    // throwing sign bit away
    u_amp.n <<= 1;

    // get value of the exponent multiplied by 3 
    // and increase it by 0, 1 or 2, depending on MSB of the mantissa
    return u_amp.bLE.e + u_amp.bLE.e + u_amp.bLE.e + mlut[u_amp.bLE.m2];
}

// Calculates the index to be used to address array of images of stripes
// based on the approximating dB21 values
// in case of Little Endian Float Systems.
int VuMeterBase::ampToStripeIndex_le(float amp) {
    int index = ampTodB21_le(amp)  - indexBiasMindB2;

    if (index < 0)
        index = 0;
    else if (index > totalBlocks)
        index = totalBlocks;

    return index;
}

// Calculates the approximating dB21 value
// in case of Big Endian Float Systems.
inline int VuMeterBase::ampTodB21_be(float amp) {
    // room for the float-int punning
    u_fip_t u_amp;

    // store the float amplitude value for the float-int punning
    u_amp.f = amp;

    // shift bits of exponent and mantissa to byte boundaries,
    // throwing sign bit away
    u_amp.n <<= 1;

    // get value of the exponent multiplied by 3 
    // and increase it by 0, 1 or 2, depending on MSB of the mantissa
    return u_amp.bBE.e + u_amp.bBE.e + u_amp.bBE.e + mlut[u_amp.bBE.m2];
}

// Calculates the index to be used to address array of images of stripes
// based on the approximating dB21 values
// in case of Big Endian Float Systems.
int VuMeterBase::ampToStripeIndex_be(float amp) {
    int index = ampTodB21_be(amp) - indexBiasMindB2;

    if (index < 0)
        index = 0;
    else if (index > totalBlocks)
        index = totalBlocks;

    return index;
}

// Paints a single stripe.
void VuMeterBase::paint(Graphics& g) {
    int index = (this->*ampToStripeIndex)(v);
    g.drawImageAt(*(singleStripes[index]), 0, 0); // it might be more efficient than the ``drawImage()``
}

// Calculates amplitude value from the traditional decibel (based on log10),
// supposing that reference amplitude is the unity (i.e 1.0F).
float VuMeterBase::dB10ToAmp(float dB10) {
    return (float)pow(10.0, ((double)dB10 * 0.1));
}

// Calculates amplitude value from the approximating log2-based decibel,
// supposing that reference amplitude is the unity (i.e 1.0F).
float VuMeterBase::dB2ToAmp(float dB2) {
    return (float)pow(2.0, ((double)dB2 / 3.0));
}

// Look up table for mantissa; it is filled by:
// - 0 for elements 0...65,
// - 1 for elements 66..149,
// - 2 for elements 150..255.
// Hint: 66 and 150 is the value of MSB of the mantissa
// of float values of the power of 2.0F up to 1/3 and 2/3,
// in the 4-byte IEEE 754 float representations.
const uint8_t VuMeterBase::mlut[256] = {
    // 0  1  2  3  4  5  6  7  8  9 
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 00
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 10
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 20
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 30
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 40
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 50
       0, 0, 0, 0, 0, 0, 1, 1, 1, 1,  // 60
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 70
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 80
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 90

       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 100
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 110
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 120
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 130
       1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 140
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 150
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 160
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 170
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 180
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 190

       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 200
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 210
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 220
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 230
       2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 240
       2, 2, 2, 2, 2, 2               // 250
};

VuMeterBase::~VuMeterBase() {
    // intentionally blank (for testing only)
}

VuStripesSingleton::~VuStripesSingleton() {
    // release memblks reserved for images of solid stripes
    destroyStripes(tricolorVuMeterStripes);
    // release memblks reserved for images of tricolor stripes
    destroyStripes(solidVuMeterStripes);
//DBG("VuStripesSingleton::~VuStripesSingleton() called");
}

VuStripesSingleton::VuStripesSingleton() {
    // create solid color single stripes
    createStripes(solidVuMeterStripes, false);
    // create tricolor single stripes
    createStripes(tricolorVuMeterStripes, true);
//DBG("VuStripesSingleton::VuStripesSingleton() called");
}


void VuStripesSingleton::createStripes(juce::Image** array, bool tricolor) {
    // get a copy of the original image, having two bands
    juce::Image origDoubleStripe = ImageCache::getFromMemory(BinaryData::Meter_140x8_png, BinaryData::Meter_140x8_pngSize).createCopy();

    if (origDoubleStripe.isValid()) {
        // get dimensions of the image
        int height = origDoubleStripe.getHeight();
        int height2 = height / 2;
        int width = origDoubleStripe.getWidth();

        // create a tricolor image if specified
        if (tricolor) {
            uint8_t a; // alfa
            uint8_t r; // red
            uint8_t g; // green
            uint8_t b; // blue

            int iFirstRedBlock = VuMeterBase::totalBlocks - VuMeterOutput::numRedBlocks;
            int xFirstRedBlock = iFirstRedBlock * VuMeterBase::blockWidthWithGap + VuMeterBase::blockGap + VuMeterBase::border;
            int xFirstYellowBlock = (iFirstRedBlock - VuMeterOutput::numYellowBlocks) * VuMeterBase::blockWidthWithGap + VuMeterBase::blockGap + VuMeterBase::border;

            // repaint
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    // get color of the given pixel
                    juce::Colour pixel = origDoubleStripe.getPixelAt(x, y);

                    // get the components of the current color
                    uint32_t rgba = pixel.getARGB();
                    a = (uint8_t)((rgba & 0xFF000000) >> 24);
                    r = (uint8_t)((rgba & 0x00FF0000) >> 16);
                    g = (uint8_t)((rgba & 0x0000FF00) >> 8);
                    b = (uint8_t)(rgba & 0x000000FF);

                    // create new colors
                    if (x >= xFirstRedBlock) {
                        // leave red blocks as are
                    }
                    else if (x >= xFirstYellowBlock) {
                        // make yellow blocks: set blue component to 0, and green and red ones to their max
                        b = 0;
                        r = g = r > g ? r : b;
                    }
                    else {
                        // make green blocks: swap the original red and green components
                        uint8_t t = r; r = g; g = t;
                    }
                    juce::Colour newpixel = juce::Colour::fromRGBA(r, g, b, a);

                    // set it
                    origDoubleStripe.setPixelAt(x, y, newpixel);
                }
            }
        }

// TODO: these two methods below should be tested, which one is the faster 
#ifdef CREATE_STRIPES_PIXEL_BY_PIXEL

        // create individual stripes - via the simplest, a pixel-by-pixel setting method
         
        // the boundary between the bright and dark LEDs is determined by xLimit;
        // the ``iStripe``-th stripe has number of ``iStripe`` bright LEDs
        for (int iStripe = 0, xLimit = VuMeterBase::blockGap + VuMeterBase::border; iStripe <= VuMeterBase::totalBlocks; ++iStripe, xLimit += VuMeterBase::blockWidthWithGap) {
            // create an RGB-based single stripe
            juce::Image* img = new juce::Image(juce::Image::RGB, width, height2, false);

            // set the iStripe pieces of the leftmost LEDs to a bright color,
            // leave the remaining rightmost LEDs in dark color
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height2; ++y) {
                    juce::Colour pixel;
                    if (x < xLimit) {
                        // get a bright pixel from the lower bright band
                        pixel = origDoubleStripe.getPixelAt(x, y + height2);
                    }
                    else {
                        // get a dark pixel from the upper dark band
                        pixel = origDoubleStripe.getPixelAt(x, y);
                    }
                    // set the pixel of the current stripe
                    img->setPixelAt(x, y, pixel);
                }
            }

            array[iStripe] = img;
        }

#else //CREATE_STRIPES_PIXEL_BY_PIXEL

        // create individual stripes - via JUCE's image manipulation methods

        // create a single, RGB colors based, opaque, dark stripe
        juce::Image* img = new juce::Image(juce::Image::RGB, width, height2, false);
        juce::Graphics gimg(*img);
        juce::Rectangle upperStripeRectangle(0, 0, width, height2);
        juce::Image upperStripe = origDoubleStripe.getClippedImage(upperStripeRectangle);
        gimg.drawImageAt(upperStripe, 0, 0, false);
        array[0] = img;

        // create individual stripes having 1, 2, ... bright LEDs;
        // the ``iStripe``-th stripe has number of ``iStripe`` bright LEDs
        int xStart = VuMeterBase::blockGap + VuMeterBase::border;         // left edge of new bright LED
        for (int iStripe = 1; iStripe <= VuMeterBase::totalBlocks; ++iStripe) {
            // get a copy of the previous single stripe,
            // having one bright LED less then needed
            juce::Image* imgCurrent = new juce::Image(img->createCopy());

            // get a bright LED at the ``iStripe``-th position from the lower stripe
            juce::Rectangle brightLEDRectangle(xStart, height2, VuMeterBase::blockWidthWithGap, height2);
            juce::Image brightLED = origDoubleStripe.getClippedImage(brightLEDRectangle);

            // draw a single bright LED only at ``iStripe``-th position on the current single stripe
            juce::Graphics gimgCurrent(*imgCurrent);
            gimgCurrent.drawImageAt(brightLED, xStart, 0, false);

            // store this stripe having number of ��iStripe`` bright LEDs
            array[iStripe] = imgCurrent;

            // use this image as source in the next loop
            img = imgCurrent;

            // advance the left edge for the next bright LED
            xStart += VuMeterBase::blockWidthWithGap;
        }

#endif //CREATE_STRIPES_PIXEL_BY_PIXEL

    }
}

void VuStripesSingleton::destroyStripes(juce::Image** array) {
    for (int iStripe = numStripes - 1; iStripe >= 0; --iStripe) {
        if (array[iStripe] != nullptr) {
            delete array[iStripe];
        }
    }
}

// Constructor of VuMeter (solid color), using 0 dB as upper limit
VuMeter::VuMeter(): VuMeterBase(0) {
    singleStripes = VuStripesSingleton::getInstance().getSolidVuMeterStripes();
}

// Constructor of VuMeterOutput (tricolor), using +6 dB as upper limit
VuMeterOutput::VuMeterOutput() : VuMeterBase(6) {
    singleStripes = VuStripesSingleton::getInstance().getTricolorVuMeterStripes();
}

double VuMeterOutput::getDecayFactor(double sampleRate) {
    constexpr double dBdrop = -40.0;    // a range of amplitude drop in dB
    constexpr double fallingTime = 0.3; // falling time in seconds for the amplitude drop

    return (float) exp(dBdrop / 10.0 * log(10.0) / (fallingTime * sampleRate));
}

//EOF