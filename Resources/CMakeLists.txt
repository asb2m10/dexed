# Linker options for Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-fPIC)
endif()

juce_add_binary_data(DexedResources SOURCES
    ui/ButtonUnlabeled_50x30.png
    ui/dexedIcon.png
    ui/dexed-logo.png
    ui/GlobalEditor_864x144.png
    ui/HelpButton.png
    ui/HelpButton.svg
    ui/Knob_34x34.png
    ui/LFO_36_26.png
    ui/Light_14x14.png
    ui/Meter_140x8.png
    ui/NotoSans-Bold.ttf
    ui/NotoSans-Regular.ttf
    ui/OperatorEditor_287x218.png
    ui/Scaling_36_26.png
    ui/Slider_26x26.png
    ui/Switch_32x32.png
    ui/Switch_48x26.png
    ui/SwitchLighted_48x26.png
    builtin_pgm.zip
)
