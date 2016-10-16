set -x
make
g++ -std=c++11 -c dexed-test.cpp
g++ -std=c++11 dexed-test.o fm_core.o env.o lfo.o dx7note.o sin.o pitchenv.o fm_op_kernel.o freqlut.o exp2.o EngineMkI.o EngineOpl.o -lm -o dexed-test
./dexed-test

