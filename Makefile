VPATH = src
objects = foolishgo.o
a : $(objects)
	g++ -o a $(objects)
.PHONY : clean
clean :
	-rm a *.o
