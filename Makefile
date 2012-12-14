VPATH = src
CXX := clang++
CXXFLAGS := -Weverything -std=c++0x -O3 -g \
    -stdlib=libc++ \
    -Wno-c++98-compat-pedantic \
    -Wno-conversion \
    -Wno-pedantic \
    -Wno-exit-time-destructors \
    -Wno-padded \
    -Wno-unused-parameter \
    -Wno-char-subscripts \
    -Wno-unneeded-internal-declaration \
    -Wno-format \
    -Wno-sign-compare \
    -Wno-non-virtual-dtor
objects = foolishgo.o rand.o
pos_cal_h = pos_cal.h def.h pos_cal-TLT.h pos_cal_TEST.h
board_h = board.h def.h position.h board-TLT.h board_TEST.h $(pos_cal_h)
chain_set_h = chain_set.h position.h $(pos_cal_h) chain_set_TEST.h \
	      chain_set-TLT.h
board_in_gm_h = board_in_gm.h $(board_h) $(chain_set_h) board_in_gm-TLT.h \
		board_in_gm_TEST.h bitset_util.h
mc_simulator_h = mc_simulator.h mc_simulator_TEST.h mc_simulator-TLT.h def.h \
		 position.h $(board_in_gm_h) rand.h
zob_hasher_h = zob_hasher.h zob_hasher-TLT.h zob_hasher_TEST.h def.h \
	       position.h def.h rand.h $(board_in_gm_h)
engine_h = engine.h engine-TLT.h def.h $(board_in_gm_h) $(mc_simulator_h) \
	   bitset_util.h engine_TEST.h

edit : $(objects)
	clang++ -stdlib=libc++ -g -o a.out $(objects)

foolishgo.o : def.h $(board_h) $(pos_cal_h) $(board_in_gm_h) $(chain_set_h) \
    $(mc_simulator_h) $(zob_hasher_h) $(engine_h)
rand.o : rand.h

.PHONY : clean
clean :
	-rm a.out *.o
