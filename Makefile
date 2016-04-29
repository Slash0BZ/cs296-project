CC = clang
EXE = remove
WARNINGS = -Wall -Wextra
CFLAGS_DEBUG	= -O0 $(WARNINGS) -g -std=c99
CFLAGS	= -O2 $(WARNINGS) -g -std=gnu99

all:
	$(CC) $(CFLAGS) remove.c
	mv a.out remove
