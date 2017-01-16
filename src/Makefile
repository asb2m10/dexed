BUNDLE=dexed.lv2
INSTALL_DIR=/zynthian/zynthian-plugins/lv2
INSTALL_MYPLUGINS_DIR=/zynthian/zynthian-my-plugins/lv2
TARGET=dexed.so
OBJ=fm_core.o env.o lfo.o dx7note.o sin.o pitchenv.o fm_op_kernel.o freqlut.o exp2.o EngineMkI.o EngineOpl.o PluginFx.o trace.o
#CFLAGS=-fPIC -DPIC -std=c++11 -I. -I/usr/local/include/lvtk-2 -DLVTK_DEBUG=false
CFLAGS_LVTK=$(shell pkg-config --cflags --libs lvtk-plugin-2)
CFLAGS=-fPIC -DPIC -std=c++11 -I. $(CFLAGS_LVTK) -DLVTK_DEBUG=false
#LDFLAGS=-L/usr/local/lib -llvtk_plugin2
LDFLAGS_LVTK=$(shell pkg-config --ldflags --libs lvtk-plugin-2)
LDFLAGS=-L/usr/local/lib $(LDFLAGS_LVTK)
#DEBUG=1
#FILETRACE=1

ifeq ($(ARCH),)
   ARCH := $(shell uname -m)
endif
machine = $(shell sh -c 'uname -m 2>/dev/null || echo unknown')

# Intel-PC
ifneq (,$(findstring x86,$(machine)))
  CPU = 
  FPU = 
endif

# Raspberry Pi B+, Zero, etc
ifneq (,$(findstring armv6l,$(machine)))
  CPU = -mcpu=arm1176jzf-s
  FPU = -mfpu=vfp
endif

# Raspberry Pi 2 and 3
ifneq (,$(findstring armv7l,$(machine)))
  model = $(shell sh -c 'cat /sys/firmware/devicetree/base/model 2>/dev/null || echo unknown')
ifneq (,$(findstring 3,$(model)))
  CPU = -mcpu=cortex-a53
  FPU = -mfpu=neon-fp-armv8
else
  CPU = -mcpu=cortex-a7 -mthumb
  FPU = -mfpu=neon-vfpv4
endif
  FPU += -mneon-for-64bits
endif

# ARM A64
ifneq (,$(findstring aarch64,$(machine)))
  CPU = -mcpu=cortex-a53
  PLATFORM += -mabi=lp64 -mcmodel=tiny
endif

ifneq ($(findstring arm,$(ARCH)),)
   CXXFLAGS += 
endif

ifeq ($(DEBUG), 1)
   ifeq ($(FILETRACE), 1)
      CXXFLAGS += -DDEBUG -DFILETRACE $(CFLAGS) $(CFLAGS_OPTIONS)
   else
      CXXFLAGS += -DDEBUG $(CFLAGS) $(CFLAGS_OPTIONS)
   endif
else
   #CXXFLAGS += $(CFLAGS) $(CFLAGS_OPTIONS) -O3 -mcpu=cortex-a7 -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -mvectorize-with-neon-quad
   CXXFLAGS += $(CFLAGS) $(CFLAGS_OPTIONS) -O3
endif

all: $(BUNDLE) Makefile

clean: Makefile
	rm -f *.o *~ *.bak *.gch dexed_ttl.h *.so *.gch msfa/*.gch
	rm -rf $(BUNDLE)

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_MYPLUGINS_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	rm -rf ${HOME}/.lv2/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)
	cp -R user-presets/* $(INSTALL_MYPLUGINS_DIR)

$(BUNDLE): manifest.ttl Dexed.ttl modgui.ttl presets.ttl dexed.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)
	cp -R modgui $(BUNDLE)

dexed.so: $(OBJ) dexed.o
	$(CXX) $(CXXFLAGS) -shared dexed.o $(OBJ) $(LDFLAGS) -o dexed.so

dexed.o: Makefile dexed.cpp dexed.h dexed_ttl.h
	$(CXX) $(CXXFLAGS) -Wall -c dexed.cpp

trace.o: Makefile trace.c trace.h
	$(CXX) $(CXXFLAGS) -Wall -c trace.c

fm_core.o: Makefile msfa/fm_core.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/fm_core.cc
 
env.o: Makefile msfa/env.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/env.cc

lfo.o: Makefile msfa/lfo.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/lfo.cc

dx7note.o: Makefile msfa/dx7note.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/dx7note.cc

sin.o: Makefile msfa/sin.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/sin.cc

pitchenv.o: Makefile msfa/pitchenv.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/pitchenv.cc

fm_op_kernel.o: Makefile msfa/fm_op_kernel.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/fm_op_kernel.cc

freqlut.o: Makefile msfa/freqlut.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/freqlut.cc

exp2.o: Makefile msfa/exp2.cc
	$(CXX) $(CXXFLAGS) -Wall -c msfa/exp2.cc

EngineMkI.o: Makefile EngineMkI.cpp
	$(CXX) $(CXXFLAGS) -Wall -c EngineMkI.cpp

EngineOpl.o: Makefile EngineOpl.cpp
	$(CXX) $(CXXFLAGS) -Wall -c EngineOpl.cpp

PluginFx.o: Makefile PluginFx.cpp
	$(CXX) $(CXXFLAGS) -Wall -c PluginFx.cpp

dexed_ttl.h: Dexed.ttl
	ttl2c Dexed.ttl dexed_ttl.h

