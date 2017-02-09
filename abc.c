#include <stdio.h>

 int main(void){
	int a;


	printf("Please, input sequence of pairs of numbers: \n");
	while (scanf("%d%d",&a,&b)!=2) {
		printf("Input numbers,please!\n");
		scanf("%c%c%c",&a,&b,&c);
	}
	while (a!=0 ||  b!=0){
		if (a<b) {
			printf("%d %d\n",a,b);
		}
		while (scanf("%d%d",&a,&b)!=2){
			printf("Input numbers,please!\n");
			scanf("%c%c%c",&a,&b,&c);
		} 
	
	}
	return 0;
}
