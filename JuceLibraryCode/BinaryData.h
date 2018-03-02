/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   dexedIcon_png;
    const int            dexedIcon_pngSize = 14239;

    extern const char*   Switch_32x32_png;
    const int            Switch_32x32_pngSize = 841;

    extern const char*   Switch_48x26_png;
    const int            Switch_48x26_pngSize = 2261;

    extern const char*   ButtonUnlabeled_50x30_png;
    const int            ButtonUnlabeled_50x30_pngSize = 1850;

    extern const char*   Knob_34x34_png;
    const int            Knob_34x34_pngSize = 13011;

    extern const char*   LFO_36_26_png;
    const int            LFO_36_26_pngSize = 1702;

    extern const char*   Light_14x14_png;
    const int            Light_14x14_pngSize = 796;

    extern const char*   Meter_140x8_png;
    const int            Meter_140x8_pngSize = 402;

    extern const char*   Scaling_36_26_png;
    const int            Scaling_36_26_pngSize = 1538;

    extern const char*   Slider_26x26_png;
    const int            Slider_26x26_pngSize = 744;

    extern const char*   NotoSansRegular_ttf;
    const int            NotoSansRegular_ttfSize = 306788;

    extern const char*   NotoSansBold_ttf;
    const int            NotoSansBold_ttfSize = 311508;

    extern const char*   builtin_pgm_zip;
    const int            builtin_pgm_zipSize = 88890;

    extern const char*   about_png;
    const int            about_pngSize = 24863;

    extern const char*   GlobalEditor_864x144_png;
    const int            GlobalEditor_864x144_pngSize = 16209;

    extern const char*   OperatorEditor_287x218_png;
    const int            OperatorEditor_287x218_pngSize = 10927;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 16;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
