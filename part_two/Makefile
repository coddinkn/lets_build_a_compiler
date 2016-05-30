CC=gcc
CFLAGS = -Wall -g

all: clean knc test

test: test.out

.PHONY: test clean

knc: main.c
	$(CC) $(CFLAGS) $^ -o $@

test.s: knc
	@echo ".section .data\n" >> test.s
	@echo ".section .text\n" >> test.s
	@echo ".global _start\n" >> test.s
	@echo "_start:" >> test.s
	@echo "enter test code:"
	@./knc >> test.s
	@echo "\tmov %rax, %rdi" >> test.s
	@echo "\tmov \$$60, %rax" >> test.s
	@echo "\tsyscall" >> test.s

test.out: test.s
	$(CC) -nostdlib $(CFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm -f test.*
	rm -f knc

