CC=clang
IDIR=.
LDIR=.

DEPS=aes.h
LIBS=-L$(LDIR) -laes

CFLAGS=-I$(IDIR) -O2

all: main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib%.a: %.o $(DEPS)
	ar cr $@ $<

main: main.c libaes.a
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *.a *.so main
