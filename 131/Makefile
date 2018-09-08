OBJ = main.o args.o
DEPS = args.h
LIBS =
CFLAGS =
CC = g++
EXTENSION = .cpp

%.o: %$(EXTENSION) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core *~ main
