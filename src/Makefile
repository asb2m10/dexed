#DEBUG=-DDEBUG
BUNDLE=dexed.lv2
INSTALL_DIR=/home/pi/zynthian/zynthian-plugins/mod-lv2
TARGET=dexed.so
OBJ=ringbuffer.o fm_core.o env.o lfo.o dx7note.o sin.o pitchenv.o fm_op_kernel.o freqlut.o exp2.o EngineMkI.o EngineOpl.o PluginFx.o trace.o
CFLAGS=-fPIC -DPIC $(DEBUG) -std=c++11 -I. -I/usr/local/include/lvtk-2
LDFLAGS=-L/usr/local/lib -llvtk_plugin2

all: $(BUNDLE) Makefile

clean: Makefile
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

dexed.o: Makefile dexed.cpp dexed.peg
	$(CXX) $(CFLAGS) -Wall -c dexed.cpp

trace.o: Makefile trace.c trace.h
	$(CXX) $(CFLAGS) -Wall -c trace.c

ringbuffer.o: Makefile msfa/ringbuffer.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/ringbuffer.cc
 
fm_core.o: Makefile msfa/fm_core.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/fm_core.cc
 
env.o: Makefile msfa/env.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/env.cc

lfo.o: Makefile msfa/lfo.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/lfo.cc

dx7note.o: Makefile msfa/dx7note.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/dx7note.cc

sin.o: Makefile msfa/sin.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/sin.cc

pitchenv.o: Makefile msfa/pitchenv.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/pitchenv.cc

fm_op_kernel.o: Makefile msfa/fm_op_kernel.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/fm_op_kernel.cc

freqlut.o: Makefile msfa/freqlut.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/freqlut.cc

exp2.o: Makefile msfa/exp2.cc
	$(CXX) $(CFLAGS) -Wall -c msfa/exp2.cc

EngineMkI.o: Makefile EngineMkI.cpp
	$(CXX) $(CFLAGS) -Wall -c EngineMkI.cpp

EngineOpl.o: Makefile EngineOpl.cpp
	$(CXX) $(CFLAGS) -Wall -c EngineOpl.cpp

PluginFx.o: Makefile PluginFx.cpp
	$(CXX) $(CFLAGS) -Wall -c PluginFx.cpp

dexed.peg: Dexed.ttl
	lv2peg Dexed.ttl dexed.peg

