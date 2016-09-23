CFLAGS := -DDEBUG=0 -std=c++11

all: \
	msfa/fm_core.o \
	msfa/env.o \
	msfa/lfo.o \
	msfa/dx7note.o \
	msfa/sin.o \
	msfa/pitchenv.o \
	msfa/fm_op_kernel.o \
	msfa/freqlut.o \
	msfa/exp2.o \
	EngineMkI.o \
	EngineOpl.o \
	PluginFx.o \
	PluginProcessor.o \
	SysexComm.o \

clean:
	rm -f *.o *~ *.bak

msfa/%.o: msfa/%.cc
	$(CXX) $(CFLAGS) -Wall -c $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -Wall -c $<
