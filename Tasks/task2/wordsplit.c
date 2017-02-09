#include "util.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

	int opt = getopt(argc, argv, "l");
	char * line;
	char * temp;
	while(!feof(stdin)){

		line = getline_unlim();

		// Перевод в нижний регистр

		if (opt == 'l') {
			for (temp = line; *temp != '\0'; temp ++){
				if(*temp >= 'A' && *temp <= 'Z'){*temp = *temp - 'A' + 'a';}
			}

		}

		// Замена всех разделителей на '\n'

		for (temp = line; *temp != '\0'; temp++){
			if ( !((*temp >= 'a' && *temp <= 'z') || (*temp >= 'A' && *temp <= 'Z') || (*temp >='0' && *temp <='9')) ){
					
				*temp = '\n';

			}
		}

		// Печать line 
		printf("%s\n", line);
	}
	return 0;
}
