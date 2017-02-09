SH = /bin/bash

CC = gcc
CFLAGS = -g -Wall

all: shell_andor.c test.sh
	$(CC) $(CFLAGS) $< -o shell_andor
	./test.sh