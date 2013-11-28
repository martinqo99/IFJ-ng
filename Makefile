PRJ=ifj13


FILES=main.c errors.c strings.c scanner.c btree.c mmu.c stack.c list.c
CC=gcc
CFLAGS=-pedantic -W -Wall -Wextra -g -std=c99

PROGS=$(PRJ)

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
