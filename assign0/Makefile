CC = gcc
CFLAGS = -I.
DEPS = myio.h
OBJ = assign0.o myio.o errExit.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

p0: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean: rm -f $(OBJ) p0