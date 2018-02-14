all:
	mkdir -p target/Applications
	mkdir -p target/VST/Audio/Plug-Ins/VST
	mkdir -p target/Components/Audio/Plug-Ins/Components
	cp -R ../../../LICENSE target/LICENSE.txt
	cp -RL ../../../Builds/MacOSX/build/Release/Dexed.app target/Applications
	cp -RL ../../../Builds/MacOSX/build/Release/Dexed.vst target/VST/Audio/Plug-Ins/VST
	cp -RL ../../../Builds/MacOSX/build/Release/Dexed.component target/Components/Audio/Plug-Ins/Components

clean:
	rm -f -r target Dexed.mpkg

