CC=clang
IDIR=.
LDIR=.

DEPS=sumy.h
LIBS=-L$(LDIR) -lsumy

CFLAGS=-I$(IDIR) -O2

all: main-static main-shared

%-shared.o: %.c $(DEPS)
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

%-static.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib%.so: %-shared.o $(DEPS)
	$(CC) -shared -o $@ $< $(CFLAGS)

lib%.a: %-static.o $(DEPS)
	ar cr $@ $<

main-shared: main.c libsumy.so
	$(CC) -o $@ $< $(CFLAGS) $(LIBS)

main-static: main.c libsumy.a
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *.a *.so main-static main-shared
