#include "util.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{

	char *line1 = getline_unlim();
	char *line2;
	int count = 1;
	int flag_begin = 1; // beginning flag
	while (!feof(stdin)){
		line2 = getline_unlim();
		
		while ((strcmp(line1,line2) == 0)&&!feof(stdin)){

			count++;
			line1 = line2;
			line2 = getline_unlim();
		}
		
		if (flag_begin == 0){
			printf("\n");
		}		
		else flag_begin = 0;
		printf("%7d %s", count, line1);
		line1 = line2;
		count = 1;
	}

	printf("\n%4d %s\n",count, line1);
	return 0;
}
