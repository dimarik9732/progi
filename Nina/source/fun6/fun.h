#ifndef _FUN_H_
#define  _FUN_H_

// Описание компоненты связности
typedef struct Node {
		int ai;
		int degree;
		struct Node *next;
	} Node;

Node *new(Node *P, int a, int n);
int print(Node *P);
Node *input(void);
Node *delete(Node *P);
Node *search(Node *P, int n);
Node *sub(Node *P, Node *Q);

#endif