CC = gcc
CFLAGS = -I.
DEPS = 
OBJ = wordcount.o errExit.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

p0: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

clean: rm -f $(OBJ) wordcount