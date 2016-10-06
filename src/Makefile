BUNDLE = dexed.lv2
INSTALL_DIR = /usr/local/lib/lv2
CFLAGS := -fPIC -DPIC -DDEBUG=0 -std=c++11 -I/usr/include/lv2-c++-tools
OBJ = dexed.o fm_core.o env.o lfo.o dx7note.o sin.o pitchenv.o fm_op_kernel.o freqlut.o exp2.o EngineMkI.o EngineOpl.o
#       SysexComm.o PluginFx.o

$(BUNDLE): manifest.ttl Dexed.ttl dexed.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

clean:
	rm -f *.o *~ *.bak *.gch *.peg

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

dexed.so: $(OBJ)
	g++ -shared -fPIC -DPIC $(OBJ) `pkg-config --cflags --libs lv2-plugin` -o dexed.so

dexed.o: dexed.cpp dexed.peg
	$(CXX) $(CFLAGS) -Wall -c dexed.cpp

fm_core.o: msfa/fm_core.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/fm_core.cc
 
env.o: msfa/env.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/env.cc

lfo.o: msfa/lfo.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/lfo.cc

dx7note.o: msfa/dx7note.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/dx7note.cc

sin.o: msfa/sin.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/sin.cc

pitchenv.o: msfa/pitchenv.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/pitchenv.cc

fm_op_kernel.o: msfa/fm_op_kernel.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/fm_op_kernel.cc

freqlut.o: msfa/freqlut.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/freqlut.cc

exp2.o: msfa/exp2.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/exp2.cc

EngineMkI.o: EngineMkI.cpp
	$(CXX) $(CFLAGS) -Wall -c EngineMkI.cpp

EngineOpl.o: EngineOpl.cpp
	$(CXX) $(CFLAGS) -Wall -c EngineOpl.cpp

PluginFx.o: PluginFx.cpp
	$(CXX) $(CFLAGS) -Wall -c PluginFx.cpp

PluginProcessor.o: PluginProcessor.cpp
	$(CXX) $(CFLAGS) -Wall -c PluginProcessor.cpp

SysexComm.o: SysexComm.cpp
	$(CXX) $(CFLAGS) -Wall -c SysexComm.cpp

#msfa/%.o: msfa/%.cc
#	$(CXX) $(CFLAGS) -Wall -c $<
#
#%.o: %.cpp
#	$(CXX) $(CFLAGS) -Wall -c $<

dexed.peg: Dexed.ttl
	lv2peg Dexed.ttl dexed.peg

