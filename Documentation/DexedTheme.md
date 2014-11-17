Dexed Theme
===========

Dexed UI can be themed if you want a different colour combination or background images.

You need need to create a file named "DexedTheme.xml" and it must be placed in the same directory where "Dexed.xml" is created. On Windows, this is C:\Users\\<your user name>\AppData\Roaming\DigitalSuburban" and on Mac it is "~/Library/Application Support/DigitalSuburban"

Colour id are analogous to JUCE color ID; see [DXLookNFeel() class](../Source/DXLookNFeel.cpp) for a complete list a defined colours id. Colour value is the hexa decimal values (from 0 to 0xFF) for ALPHA RED BLUE GREEN.

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
Dexed::backgroundId
Dexed::fillColourId
```

Image id are the file name defined in [ui folder](../Resources/ui). If it cannot find the file, the image will no longer be rendered. The image path is relative to the path where "DexedTheme.xml" is defined.

Example configuration
---------------------

```xml
<dexedTheme>
	<colour id="TextEditor::textColourId" value="0xFFFF33FF"/>
	<image id="Knob_34x34.png" path="myTheme/Knob_34x34.png"/>
</dexedTheme>
```