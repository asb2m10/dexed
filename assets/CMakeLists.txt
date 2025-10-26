# Linker options for Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-fPIC)
endif()

juce_add_binary_data(DexedResources SOURCES
    ui/ButtonUnlabeled_50x30.png
    ui/dexedIcon.png
    ui/dexed-logo.png
    ui/GlobalEditor_1728x288.png
    ui/HelpButton.png
    ui/HelpButton.svg
    ui/Knob_68x68.png
    ui/LFO_36_26.png
    ui/Light_28x28.png
    ui/Meter_140x8.png
    ui/NotoSans-Bold.ttf
    ui/NotoSans-Regular.ttf
    ui/OperatorEditor_574x436.png
    ui/Scaling_36_26.png
    ui/Slider_52x52.png
    ui/Switch_64x64.png
    ui/Switch_96x52.png
    ui/SwitchLighted_48x26.png
    builtin_pgm.zip
)
