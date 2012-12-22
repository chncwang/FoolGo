VPATH = src
CXX := clang++
CXXFLAGS := -Weverything -std=c++0x -O3 -march=core2 -g \
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
    -Wno-non-virtual-dtor \
    -Wno-overloaded-virtual
objects = foolishgo.o rand.o vector_util.o position.o brd_change.o
position_h = position.h def.h
pos_cal_h = pos_cal.h def.h pos_cal-TLT.h pos_cal_TEST.h
board_h = board.h def.h $(position_h) board-TLT.h board_TEST.h $(pos_cal_h)
chain_set_h = chain_set.h $(position_h) $(pos_cal_h) chain_set_TEST.h \
	      chain_set-TLT.h
board_in_gm_h = board_in_gm.h $(board_h) $(chain_set_h) board_in_gm-TLT.h \
		board_in_gm_TEST.h bitset_util.h $(eye_set_h) $(vector_util_h)
mc_simulator_h = mc_simulator.h mc_simulator_TEST.h mc_simulator-TLT.h def.h \
		 $(position_h) $(board_in_gm_h) rand.h
zob_hasher_h = zob_hasher.h zob_hasher-TLT.h zob_hasher_TEST.h def.h \
	       $(position_h) def.h rand.h $(board_in_gm_h)
engine_h = engine.h engine-TLT.h def.h $(board_in_gm_h) $(mc_simulator_h) \
	   bitset_util.h engine_TEST.h
eye_set_h = def.h $(position_h) eye_set.h eye_set-TLT.h
vector_util_h = vector_util.h $(position_h)

edit : $(objects)
	clang++ -stdlib=libc++ -g -o a.out $(objects)

foolishgo.o : def.h $(board_h) $(pos_cal_h) $(board_in_gm_h) $(chain_set_h) \
    $(mc_simulator_h) $(zob_hasher_h) $(engine_h) $(vector_util_h)
rand.o : rand.h
vector_util.o : $(vector_util_h) rand.h
position.o : $(position_h)
brd_change.o : $(board_in_gm_h)

.PHONY : clean
clean :
	-rm a.out *.o
