VPATH = src
CXX := g++
CXXFLAGS := -Wall
objects = foolishgo.o
pos_cal_h = pos_cal.h def.h pos_cal-TLT.h pos_cal_TEST.h
board_h = board.h def.h position.h board-TLT.h board_TEST.h $(pos_cal_h)
chain_set_h = chain_set.h position.h $(pos_cal_h) chain_set_TEST.h \
	      chain_set-TLT.h airset_util.h
board_in_gm_h = board_in_gm.h $(board_h) $(chain_set_h)
edit : $(objects)
	g++ -o a.out $(objects)
foolishgo.o : def.h $(board_h) $(pos_cal_h) $(board_in_gm_h) $(chain_set_h)
.PHONY : clean
clean :
	-rm a *.o
