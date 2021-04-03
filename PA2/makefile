# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O3 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O3

all	: ./bin/mps
	@echo -n ""

# optimized version
./bin/mps: main_opt.o maximum_planar_subset.o 
			$(CC) $(OPTFLAGS) $^ -o $@ 
main_opt.o: src/main.cpp
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
maximum_planar_subset.o: src/maximum_planar_subset.cpp src/maximum_planar_subset.h 
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o bin/* output/*
