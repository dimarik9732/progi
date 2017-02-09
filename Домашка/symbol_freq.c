#include <stdio.h>

int main(void){
	
	int freq[256] = {0};
	int c;
	int count=0;
	
	printf("Input sequence of symbols:\n");
	
	while((c = getchar()) != EOF){
	
		freq[c]++;
		count++;
	}
	
	int i;
	double rel_f;
	printf("\n");	
	for (i = 0; i <256; i++){
		
		if (freq[i] > 0){
			rel_f = freq[i]/(double)count;
			printf("'%c' : %4d %7.3f\n",(char)i,freq[i],rel_f);
		}
		
	}

	return 0;
} 
