CC = g++
CFLAGS = -Wall
DEPS = tclasses.h tclasses.tpp classes.h
LIBS = -lglut -lGL -lGLU
OBJ = main.o classes.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core main
