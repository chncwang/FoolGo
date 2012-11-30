VPATH = src
CXX := g++
CXXFLAGS := -Wall
objects = foolishgo.o
pos_cal_h = pos_cal.h def.h pos_cal-TLT.h pos_cal_TEST.h
board_h = def.h position.h board-TLT.h board_TEST.h $(pos_cal_h)
edit : $(objects)
	g++ -o a.out $(objects)
foolishgo.o : def.h $(board_h) $(pos_cal_h)
.PHONY : clean
clean :
	-rm a *.o
