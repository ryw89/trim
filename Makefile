DESTDIR = /usr/local/bin
GCC_CMD = gcc -std=c99 -O2 -Wall -Werror -Wextra -Wpedantic -pedantic-errors

trim: main.c
	$(GCC_CMD) -o trim main.c

trim-debug: main.c
	$(GCC_CMD) -gdwarf -o trim-debug main.c

install: trim
	install -m 755 trim $(DESTDIR)
