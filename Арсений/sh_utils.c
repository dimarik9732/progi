/* sh_utils.c */
#include "sh_utils.h"

int find(char c, char *s){
	while(*s) if (c == *s) return 1;
		else s++;
	return 0;
}

int cmp(char *s1, char *s2){
	while(*s1 && *s1 == *s2) s1++, s2++;
	return (unsigned char) *s1 - (unsigned char) *s2;
}

int leng(char *s){
	int l = 0;
	if(s) while(*s++) l++;
	return l;
}

char *strcopy(char *s){
	int l;
	char *r,*p;
	l = leng(s);
	r = p = (char*) malloc((1 + l) * sizeof(char));
	if(s) while(*s) *p++ = *s++;
	*p = '\0';
	return r;
}

char *readStr(){
	char *s;
	int size = blockSize;
	/*printf("=> ");*/
	s = (char*) malloc( (size + 1) * sizeof(char) );
	if(!s) { fprintf(stderr, "cant allocate memory (sm)\n"); return NULL; }
	s[size] = 'X';
	if( fgets(s, size + 1, stdin) )
		while(s[size] != 'X'){
			if(s[size - 1] == '\n') break; 
			s = (char*) realloc(s, (size + blockSize + 1) * sizeof(char));
			if(!s) { fprintf(stderr, "cant allocate memory (sr)\n"); free(s); return NULL; } /* ??? */
			s[size + blockSize] = 'X';
			fgets(s + size, blockSize + 1, stdin);
			size += blockSize;
		}
	else {free(s); return 0;} /* eof? */
	
	return s;
}
