#include <stdio.h>

int main(void){
	int a=1;
	int b=1;
	int c;
	int n;

	printf("Input n of Fibonacci Sequence:\n");
	scanf("%d",&n);

	for (int i; i <= n-2; i++){
		c = a+b;
		a = b;
		b = c;
	}

	printf("That number is %d\n",c);
}
