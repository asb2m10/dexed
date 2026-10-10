Dexed Theme
===========
Last update: 26 July 2024


Dexed UI can be themed if you want a different colour combination or background images.

You need need to create a file named "DexedTheme.xml" and it must be placed in the same directory where "Dexed.xml" is created. On Windows, this is "C:\Users\&lt;your user name&gt;\AppData\Roaming\DigitalSuburban\Dexed" and on Mac it is "~/Library/Application Support/DigitalSuburban/Dexed" (FIXME!), and in Linux it is "~/.local/share/DigitalSuburban/Dexed".

Colour id are analogous to JUCE color ID; see [DXLookNFeel() class](../Source/DXLookNFeel.cpp) for a complete list a defined colours id. Colour value is the hexa decimal values (from 0 to 0xFF) for ALPHA RED BLUE GREEN.
For ALPHA, 0x00 is completely transparent, 0xFF is opaque.

Known colour keys; simply override those you want different.

```cpp
TextButton::buttonColourId
TextButton::textColourOnId
TextButton::textColourOffId
Slider::rotarySliderOutlineColourId
Slider::rotarySliderFillColourId
AlertWindow::backgroundColourId
AlertWindow::textColourId
TextEditor::backgroundColourId
TextEditor::textColourId
TextEditor::highlightColourId
TextEditor::outlineColourId
ComboBox::backgroundColourId
ComboBox::textColourId
ComboBox::buttonColourId
PopupMenu::backgroundColourId
PopupMenu::textColourId
PopupMenu::highlightedTextColourId
PopupMenu::highlightedBackgroundColourId
TreeView::backgroundColourId
DirectoryContentsDisplayComponent::highlightColourId
DirectoryContentsDisplayComponent::textColourId
ScrollBar::thumbColourId
Dexed::backgroundId
Dexed::fillColourId
```

Image id are the file name defined in [ui folder](../Resources/ui). If it cannot find the file, the image will no longer be rendered. The image path is relative to the path where "DexedTheme.xml" is defined.
Note that sizes in pixels and structures of the images should be same as original ones, because the layout of GUI based on these hardcoded fix sizes. Recently, the following image ids could be specified: 

Knob_68x68.png
Switch_96x52.png
SwitchLighted_48x26.png
Switch_64x64.png
ButtonUnlabeled_50x30.png
Slider_52x52.png
Scaling_36_26.png
Light_28x28.png
LFO_36_26.png
OperatorEditor_574x436.png
GlobalEditor_1728x288.png


Example ``DexedTheme.xml`` configuration
----------------------------------------

```xml
<dexedTheme>
	<colour id="PopupMenu::backgroundColourId"            value="0xff0000ff"/>
	<colour id="PopupMenu::textColourId"                  value="0xffffffff"/>
	<colour id="PopupMenu::highlightedTextColourId"       value="0xffffff00"/>
	<colour id="PopupMenu::highlightedBackgroundColourId" value="0xff00ffff"/>
	<colour id="ScrollBar::thumbColourId"                 value="0xFFC0C0C0"/>
	<image id="Light_28x28.png" path="myTheme/GreenLight_28x28.png"/>
</dexedTheme>
```

As the result:
- the colors of the background of popup menus, the text of normal menu items, and the text and background of highlighted menu items are changed to opaque blue, white, yellow, and cyan,
- the color of thumbnail of scrollbars is changed to opaque lightgray,
- the color of bright LEDs are changed to green from red (if there is a folder named ``myTheme`` within the subdirectory where your ``Dexed.xml`` and  ``DexedTheme.xml`` files are located, and there is the file ``GreenLight_28x28.png`` containing the image of a red LED inside that ``myTheme`` subdirectory).

Note that ``DexedTheme.xml``are loaded only once during the initialization when Dexed (either the Standalone or the plugin version) is launched.