CC = gcc
DEPS = 
OBJ = prog.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $<

prog: $(OBJ)
		$(CC) -o $@ $^


clean: rm -f $(OBJ) prog