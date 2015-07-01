CC=gcc
CFLAGS = -Wall

all: knc

knc: cradle.c
	$(CC) $(CFLAGS) $^ -o $@
