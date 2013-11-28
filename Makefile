PRJ=ifj13


FILES=main.c errors.c strings.c scanner.c gc.c stack.c list.c  symbol_table.c # btree.c interpret.c 
CC=gcc
CFLAGS=-pedantic -W -Wall -Wextra -g -std=c99
CFLAGS-NOWALL=-pedantic -W -Wextra -g -std=c99

PROGS=$(PRJ)

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)
	
nowall: $(FILES)
	$(CC) $(CFLAGS-NOWALL) -o $(PROGS) $(FILES) -lm