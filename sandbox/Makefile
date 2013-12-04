PRJ=ifj13

PROGS=$(PRJ)
FILES=strings_test.c ../errors.c ../gc.c ../strings.c
CC=gcc
CFLAGS=-Wall -W -Wextra -g -std=c99 -pedantic

all: $(PROGS)

$(PRJ): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) -lm

clean:
	rm -f *.o *.out $(PROGS)