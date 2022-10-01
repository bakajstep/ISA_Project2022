CC=gcc
CFLAGS=-Wall -Wextra -pedantic

MAIN=main

.PHONY: all clean

all:
	$(CC) $(MAIN).c $(CFLAGS) -o main

clean:
	rm -f *.o main
