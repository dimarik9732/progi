/* sh_list.c */

#include "sh_list.h"

int listSize(list l){
	int i = 0;
	while(l) i++, l = l->next;
	return i;
}

void printList(list l){
	while(l) printf("%c : %s \n", l->tp, l->word), l = l->next;
}

void clearList(list l){
	list p;
	while(l) p = l->next, free(l->word), free(l), l = p;
}

list buildList(char *s){
	static char *sn = ";()<";
	static char *db = "|&>";
	static char *dv = " \t";
	static char *ex = "#\n";
	char *w;
	
	list f = NULL, p;
	int i = 0, j, k, d;
	char l;
	int t = 1;
	if(!s) return NULL;
	
	do{
		while(find(s[i],dv)) i++;
		j = i; d = 0;
		if(s[i] == '\0' || find(s[i],ex)) t = 0;
		else
			if(find(s[i],sn)) {j++; l = 's';}
		else
			if(find(s[i],db)) {j ++; if(s[i] == s[i+1]){j++; l = 'd';} else l = 's'; }
		else
			if(s[i] == '"') {
			l = 'w';
			do { if(s[j] == '\\' && s[j+1] != '\0') {j+=2; d++;} else j++; }
			while (s[j] != '"' && s[j] != '\0');
			if(s[j] == '\0') { fprintf(stderr, "cant find quotes (l)\n"); clearList(f); return NULL; }
		}
		else {
			l = 'w';
			do  { if(s[j] == '\\' && s[j+1] != '\0') {j+=2; d++;} else j++; }
			while (s[j] != '\0' && s[j] != '"' && !find(s[j],ex) && !find(s[j],dv) && !find(s[j],sn) && !find(s[j],db));
		}
		
		if(t){
			if(f) p = p->next = (list) malloc(sizeof(struct node));
			else p = f = (list) malloc(sizeof(struct node));
			
			if(!p) { fprintf(stderr, "cant allocate memory (lm)\n"); clearList(f); return NULL; }
			
			p->next = NULL;
			p->tp = l;
			w = (char*) malloc( (j - i - d + 1) * sizeof(char));
			if(s[i] == '"'){i++; d = 1;} else d = 0;
			k = 0;
			while(i < j){				/* why does it works? */
				if(s[i] == '\\') i++;
				w[k++] = s[i++];
			}
			w[k] = '\0';
			i += d;
			p->word = w;
		}
	} while( t );
	
	return f;
}
