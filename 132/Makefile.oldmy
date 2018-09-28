LIBS =
CFLAGS = -Wall
CC = g++

all: main

main.o: main.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

superint.o: superint.cpp superint.h
	$(CC) -c -o $@ $< $(CFLAGS)

tests.o: tests.cpp tests.h superint.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o tests.o libsuperint.so
	$(CC) -o $@ $@.o tests.o $(CFLAGS) $(LIBS) -L. -lsuperint

libsuperint.so: superint.o
	$(CC) -o $@ -shared $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core *~ main libsuperint.so
