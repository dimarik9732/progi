#include <stdio.h>

int fib_recoursive(int n){
	if (n < 2) return 1;
	else return fib_recoursive(n-1)+fib_recoursive(n-2);		
}


int main(void){

	int n;
	int c;

	printf("Input n of Fibonacci Sequence:\n");
	scanf("%d",&n);

	c = fib_recoursive(n);

	printf("That number is %d\n",c);
}
