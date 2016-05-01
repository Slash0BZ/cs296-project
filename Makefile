CC = clang
EXE = remove
WARNINGS = -Wall -Wextra
CFLAGS_DEBUG	= -O0 $(WARNINGS) -g -pthread -std=c99
CFLAGS	= -O2 $(WARNINGS) -g -pthread -std=gnu99

all:
	$(CC) $(CFLAGS) remove.c
	mv a.out remove
