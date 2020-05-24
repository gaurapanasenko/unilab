CC=clang
IDIR=.
LDIR=.

DEPS=aes.h
LIBS=-L$(LDIR) -laes

CFLAGS=-I$(IDIR) -O2

all: main-128 main-192 main-256

main-128: main.c aes.c
	$(CC) -o $@ $^ $(CFLAGS)
	rm $@d
	ln -s $@ $@d

main-192: main.c aes.c
	$(CC) -o $@ $^ $(CFLAGS) -DAES192=1
	rm $@d
	ln -s $@ $@d

main-256: main.c aes.c
	$(CC) -o $@ $^ $(CFLAGS) -DAES256=1
	rm $@d
	ln -s $@ $@d

.PHONY: clean

clean:
	rm -f *.o *.a *.so *main-*
