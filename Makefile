CC=gcc
CFLAGS = -Wall

all: knc

knc: part_two.c
	$(CC) $(CFLAGS) $^ -o $@
