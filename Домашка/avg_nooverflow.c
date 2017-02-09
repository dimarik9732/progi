#include <stdio.h>

int main(void){

	printf("Input secuence of numbers:\n");
	long a;
	int f = scanf("%ld",&a);
	
	if (f == 1){
	long max = a;
	long min = a;
	double avg = 0;
	long count=0; 
	while( f == 1 ){
	
		if (a>max){
			max = a;
		}
		
		if (a<min){
			min = a;
		}
		
		avg += ((double) a - avg)/++count;
		
		f = scanf("%ld",&a);
	}
	printf("\nmax = %ld; \nmin = %ld; \naverage = %10.5f;\n",max,min,avg);
	}
	else{ printf("You didn`t input any number((\n");}
	
	return 0;
}
