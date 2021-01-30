DESTDIR = /usr/local/bin
GCC_CMD = gcc -g3 -O2 -std=c99 -Wall -Werror -Wextra -Wpedantic -pedantic-errors

trim: main.c
	$(GCC_CMD) -o trim main.c

install: trim
	install -m 755 trim $(DESTDIR)
