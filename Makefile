CC=g++
STANDARD=c++14
CFLAGS=-Wall -Wextra -pedantic

MAIN=main
ERROR=error
PARAMETERS=parameters

.PHONY: all clean

all:
	$(CC) -std=$(STANDARD) $(MAIN).cpp $(ERROR).cpp $(ERROR).hpp $(PARAMETERS).cpp $(PARAMETERS).h $(CFLAGS) -o main

clean:
	rm -f *.o main
