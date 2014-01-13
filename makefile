######################################
# CS211 sample makefile
######################################

COMPILER = gcc
CCFLAGS  = -ansi -pedantic -Wall -std=c99 -lm

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

all: cache-sim

debug:
	make DEBUG=TRUE

cache-sim: cache-sim.o
	$(COMPILER) $(CCFLAGS) -o cache-sim cache-sim.o
cache-sim.o: cache-sim.c
	$(COMPILER) $(CCFLAGS) -c cache-sim.c

clean:
	rm -f cache-sim
	rm -f *.o
