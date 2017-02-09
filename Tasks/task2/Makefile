
CFLAGS = -Wall -g
CC = gcc
SHELL = /bin/bash

clean: 
	rm -f *.o wordsplit

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

wordsplit: wordsplit.c util.o
	$(CC) $(CFLAGS) $^ -o $@

countd: countd.c util.o
	$(CC) $(CFLAGS) $^ -o $@

all: wordsplit.c countd.c util.o
	$(CC) $(CFLAGS) wordsplit.c util.o -o wordsplit
	$(CC) $(CFLAGS) countd.c util.o -o countd

linesort: ../task1/linesort.c
	$(CC) $(CFLAGS) $^ -o $@

data: books
	wget -o log -nc `cat $<`
	cat *txt > data

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: util.c hash.c
	$(CC) -MM $^ > $@

dict: data wordsplit countd linesort Makefile
	time cat $< | ./wordsplit -l | ./linesort -m | ./countd | sort -nr > $@

dict_std: data Makefile
	time cat $< |  tr -cs "[:alpha:]" "[\n*]" | sort  | uniq -ci | sort -nr > $@

freq_dict: freq_dict.c hash.o data
	$(CC) $(CFLAGS) freq_dict.c hash.o -o $@
	time cat data | ./$@ > freq_dict.txt

clean: rm -f *.o *.txt