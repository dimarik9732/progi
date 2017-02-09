#ifndef __SH_TYPES_H__
#define __SH_TYPES_H__
#include <sys/types.h>
#define blockSize 8

typedef struct node *list;
struct node{
	char *word;
	char  tp;
	list next;
};

typedef struct tnode *tree;
struct tnode{
	int dp;
	char tp,c;
	int argc;
	char **argv;
	char *fi, *fo;
	tree next,down;
};

struct ztable{
	int n;
	int tsize;
	pid_t *pids;
};


#endif