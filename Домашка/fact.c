#include <stdio.h>
int fact_recoursive(int n){
	if (n == 1){
		return 1;
	}
	else {
		return n*fact_recoursive(n-1);
	}

}



int main(void){
	int n;

	printf("Please, input n:\n");
	scanf("%d",&n);
	int f=fact_recoursive(n);
	printf("n! = %d\n",f); 
}	
