CC = gcc
DEPS = 
OBJ = part1.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $<

part1: $(OBJ)
		$(CC) -o $@ $^


clean: rm -f $(OBJ) part1