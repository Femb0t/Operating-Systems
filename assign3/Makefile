CC = gcc
DEPS = 
OBJ = part1.o
OBJ2 = part2.o
OBJ3 = part3.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $<

part1: $(OBJ)
		$(CC) -o $@ $^

part2: $(OBJ2)
		$(CC) -o $@ $^

part3: $(OBJ3)
		$(CC) -o $@ $^

clean: rm -f $(OBJ) part1 part2 part3