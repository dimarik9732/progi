#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


char* getline_unlim(void){
	enum {FGETS_BUFSIZE = 50};
	int i, counter = 1, flag = 0;
	char *tmp, *c = (char*) calloc((FGETS_BUFSIZE-1)*counter + 1, sizeof(char*));
	if (fgets(c, 2, stdin) != NULL && !(feof(stdin))){
		if (feof(stdin)){
			return NULL;
		}
		if ( *c == '\n' ) { *c = '\0'; return c;} 
		tmp = fgets(c + 1, FGETS_BUFSIZE, stdin);
		if (tmp == NULL && !feof(stdin)){
			fprintf(stderr, "\n%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < (FGETS_BUFSIZE - 1) + 1; i++){
				if (c[i] == '\n' || c[i] == '\0'){
					c[i] = '\0';
					flag = 1;
				}
            }
		for(; flag == 0; counter++){ 
			c = realloc(c, counter*(FGETS_BUFSIZE - 1) + 1 + FGETS_BUFSIZE);
			tmp = fgets(c + counter*(FGETS_BUFSIZE - 1) + 1, FGETS_BUFSIZE, stdin);
			if (tmp == NULL && !feof(stdin)){
				fprintf(stderr, "\n%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			for(i = counter*(FGETS_BUFSIZE - 1); i < (counter + 1)*(FGETS_BUFSIZE - 1) + 1; i++){
				if (c[i] == '\n' || c[i] == '\0'){
					c[i] = '\0';
					flag = 1;
				}
			}
		}
	}
	else{
			c = NULL;
	}
	return c;
}
