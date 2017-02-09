#include "fun.h"
#include <stdlib.h>
int main(void)
{
	Node *P1 = NULL;
	P1 = input();
	print(P1);
	P1 = delete(P1);
	
	return 0;
}
