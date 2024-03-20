CC = clang
Cflags = -Wall -Wpedantic -Werror -Wextra
LIBFLAGS = -lncurses

all = life

life: life.o universe.o
	$(CC) -o $@ $^ $(LIBFLAGS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f life *.o universe *.o
	
format:
	clang-format -i -style=file *.o [ch]
