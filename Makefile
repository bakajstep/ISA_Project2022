CC=g++
STANDARD=c++14
CFLAGS=-Wall -Wextra -pedantic

MAIN=main
ERROR=error
PARAMETERS=parameters
FLOW=flows
UDP_CLIENT=udp_client

.PHONY: all clean

all:
	$(CC) -std=$(STANDARD) $(MAIN).cpp $(ERROR).cpp $(PARAMETERS).cpp $(FLOW).cpp $(UDP_CLIENT).cpp $(CFLAGS) -lpcap -o main

clean:
	rm -f *.o main