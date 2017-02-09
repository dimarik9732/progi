#include <stdio.h>
#include "util.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){

	char *command;
	command = getline_unlim();
		
/*
	int size = 2;
	int j = 0;
	char ** argv1 = (char **) malloc(size);
	*argv1 = NULL;
	char buf[256];
	int i = 0;

	//command = rm_space(command);

	while((sscanf(command + i, " %255s", buf)) == 1){
		int len = strlen(buf);
		i += len + 1;
		if (j+1 > size){
			size *= 2;
			argv1 = (char **) realloc(argv1, size);
		}
		argv1[j] = (char *) malloc((len+1)*sizeof(char));
		memcpy(argv1[j], buf, len);
		argv1[j][len+1] = '\0';
		argv1[j+1] = NULL;
		j++;
	}

*/

	char ** argv1;
	char *cur_param;
	int j = 0, state = 0, end_f = 0;
	int size = 1;
	char *cur = command;

	argv1 = (char **) malloc(sizeof(char*));

	while ( !end_f ){
		switch(state){

			case 0: 
				if (isspace(*cur)) cur++;
				else { state = 1; cur_param = cur; cur++;}
				break;
			case 1: 	
				if (!isspace(*cur) && *cur != '\0') cur++;
				else { 

					if (j + 1 > size){
						size *= 2;
						argv1 = (char **) realloc(argv1, size);
					}
					if (*cur == '\0')
					{
						end_f = 1;
					}
					else *cur = '\0';
					
					argv1[j] = cur_param;
					j++;
					state = 0;
					cur++;
				}
				break;
		}
	}


	printf("%s\n", argv1[2]);
	
	return 0;
}