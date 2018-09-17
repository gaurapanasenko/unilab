LIBS =
CFLAGS = -Wall
CC = g++

all: main

main.o: main.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

superint.o: superint.cpp superint.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o libsuperint.so
	$(CC) -o $@ $@.o $(CFLAGS) $(LIBS) -L. -lsuperint

libsuperint.so: superint.o
	$(CC) -o $@ -shared $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core *~ main libsuperint.so
