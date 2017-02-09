CC=gcc
CFLAGS= -g

%.o: %.c %.h sh_types.h
	$(CC) $(CFLAGS) -c $< -o $@

sh_tree.o: sh_tree.c sh_tree.h sh_list.h sh_types.h

all: sh_utils.o sh_list.o sh_tree.o sh_exec.o sh_main.c sh_globals.h
	$(CC) $(CFLAGS) sh_utils.o sh_list.o sh_tree.o sh_exec.o sh_main.c -o shell