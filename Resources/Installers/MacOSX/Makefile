all:
	mkdir -p target/Applications
	mkdir -p target/VST/Audio/Plug-Ins/VST
	mkdir -p target/Components/Audio/Plug-Ins/Components
	cp -R ../../../LICENSE target/LICENSE.txt
	cp -RL Dexed.xcarchive/Products/Applications/Dexed.app target/Applications
	cp -RL Dexed.xcarchive/Products/Users/asb2m10/Library/Audio/Plug-Ins/VST/Dexed.vst target/VST/Audio/Plug-Ins/VST
	cp -RL Dexed.xcarchive/Products/Users/asb2m10/Library/Audio/Plug-Ins/Components/Dexed.component target/Components/Audio/Plug-Ins/Components

clean:
	rm -f -r target Dexed.mpkg

