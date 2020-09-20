all:
	mkdir -p target/Applications
	mkdir -p target/VST/Audio/Plug-Ins/VST
	mkdir -p target/Components/Audio/Plug-Ins/Components
	cp -R ../../../LICENSE target/LICENSE.txt
	cp -RL Dexed.app target/Applications
	cp -RL Dexed.vst3 target/VST/Audio/Plug-Ins/VST
	cp -RL Dexed.component target/Components/Audio/Plug-Ins/Components

clean:
	rm -f -r target Dexed.mpkg

