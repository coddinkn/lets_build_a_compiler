CC=gcc
CFLAGS = -Wall -g

all: clean knc test

test: test.out

.PHONY: test clean

knc: main.c
	$(CC) $(CFLAGS) $^ -o $@

test.s: knc
	cat test.k
	./knc < test.k >> test.s

test.out: test.s
	$(CC) -nostdlib $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f test.s
	rm -f test.out
	rm -f knc

