#DEBUG=-DDEBUG=0
BUNDLE=dexed.lv2
INSTALL_DIR=/home/pi/zynthian/zynthian-plugins/mod-lv2
TARGET=dexed.so
OBJ=ringbuffer.o fm_core.o env.o lfo.o dx7note.o sin.o pitchenv.o fm_op_kernel.o freqlut.o exp2.o EngineMkI.o EngineOpl.o PluginFx.o trace.o
CFLAGS=-fPIC -DPIC $(DEBUG) -std=c++11 -I. -I/usr/local/include/lvtk-2
LDFLAGS=-L/usr/local/lib -llvtk_plugin2

all: $(BUNDLE) Makefile

clean:
	rm -f *.o *~ *.bak *.gch dexed.peg *.so *.gch msfa/*.gch
	rm -rf $(BUNDLE)

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	rm -rf ${HOME}/.lv2/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

$(BUNDLE): manifest.ttl Dexed.ttl dexed.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)
#	cp -R modgui $(BUNDLE)

dexed.so: $(OBJ) dexed.o
	$(CXX) -shared dexed.o $(OBJ) $(LDFLAGS) -o dexed.so

dexed.o: dexed.cpp dexed.peg
	$(CXX) $(CFLAGS) -Wall -c dexed.cpp

trace.o: trace.c trace.h
	$(CXX) $(CFLAGS) -Wall -c trace.c

ringbuffer.o: msfa/ringbuffer.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/ringbuffer.cc
 
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

dexed.peg: Dexed.ttl
	lv2peg Dexed.ttl dexed.peg

