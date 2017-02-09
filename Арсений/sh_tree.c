/* sh_tree.c */
#include "sh_tree.h"

void printTree(tree t){
	int k,i;
	while(t)
	switch(t->dp){
		case 3:
		case 2: 
		case 1: 
			if(t->down && t->down->dp == 3){
				printf("(");
				printTree(t->down);
				printf(")"); 
				if(t->down->fi) printf(" < %s",t->down->fi);
				if(t->down->fo){printf(" >");	if(t->down->tp == 'd') printf(">"); printf("%s",t->down->fo);}
			} else printTree(t->down);
			
			if(t->c)printf("%c",t->c);
		        if(t->c && t->dp == 2) printf("%c",t->c);
			t = t->next; break;
		case 0:
			printf("%s",t->argv[0]);
			if(t->c) printf(" [%c] ",t->c);
			if(t->fi) printf(" < %s",t->fi);
			if(t->fo){printf(" >");	if(t->tp == 'd') printf("> "); printf(" %s",t->fo);}
			if((k = t->argc) > 1) for(i = 1; i < k; i++) printf(" %s",t->argv[i]);
			t = NULL;
			break;
	}
}

void clearTree(tree t){
	tree p;
	while(t){
		clearTree(t->down);
		if(t->fi) free(t->fi);
		if(t->fo) free(t->fo);
		if(t->argv){
		char **s = t->argv;
		while(*s) free(*s++);
			free(t->argv);
		}
		p = t->next;
		free(t);
		t = p;
	}
}

void getnext(){
	if(curlist) curlist = curlist->next;
}

tree buildT3(){
	static char *s = ";&";
	tree t,p;
	if (!curlist) return NULL;
	t = p = (tree) malloc(sizeof(struct tnode));
	if(!p) { fprintf(stderr, "cant allocate memory (t3)\n"); err = 1; return NULL; }
	p->dp = 3;
	p->next = NULL;
	p->fi = p->fo = NULL;
	p->argv = NULL;
	p->c = ';';
	//getnext();
	p->down = buildT2();

	while(curlist && curlist->tp == 's' && find(curlist->word[0],s)){
		p->c = curlist->word[0];
		getnext();
		if(!curlist || !(curlist->tp == 's' && curlist->word[0] == '(') && !(curlist->tp == 'w'))
			break;
		p = p->next = (tree) malloc(sizeof(struct tnode));
		if(!p) { fprintf(stderr, "cant allocate memory (t3)\n"); err = 1; return t; }
		
		p->dp = 3;
		p->next = NULL;
		p->fi = p->fo = NULL;
		p->argv = NULL;
		p->c = ';';
		p->down = buildT2();
	}
	
	return t;
}

tree buildT2(){
	static char *s = "|&";
	tree t,p;
	if (!curlist) return NULL;
	t = p = (tree) malloc(sizeof(struct tnode));
	if(!p) { fprintf(stderr, "cant allocate memory (t2)\n"); err = 1; return NULL; }
	p->dp = 2;
	p->next = NULL;
	p->fi = p->fo = NULL;
	p->argv = NULL;
	p->c = '\0';
	//getnext();
	p->down = buildT1();
	
	while(curlist && curlist->tp == 'd' && find(curlist->word[0],s)){
		p->c = curlist->word[0];
		getnext();
		if(!curlist || !(curlist->tp == 's' && curlist->word[0] == '(') && !(curlist->tp == 'w'))
			{p->c = '\0'; fprintf(stderr, "cant find operand (t2)\n"); err = 1; break;}
		p = p->next = (tree) malloc(sizeof(struct tnode));
		if(!p) { fprintf(stderr, "cant allocate memory (t2)\n"); err = 1; return t; }
		
		p->dp = 2;
		p->next = NULL;
		p->fi = p->fo = NULL;
		p->argv = NULL;
		p->c = '\0';
		p->down = buildT1();
	}
	
	return t;
}

tree buildT1(){
	tree t,p;
	if (!curlist) return NULL;
	t = p = (tree) malloc(sizeof(struct tnode));
	if(!p) { fprintf(stderr, "cant allocate memory (t1)\n"); err = 1; return NULL; }
	p->dp = 1;
	p->next = NULL;
	p->fi = p->fo = NULL;
	p->argv = NULL;
	p->c = '\0';
	//getnext();
	p->down = buildT0();
	
	while(curlist && curlist->tp == 's' && curlist->word[0] == '|'){
		p->c = '|';
		getnext();
		if(!curlist || !(curlist->tp == 's' && curlist->word[0] == '(') && !(curlist->tp == 'w'))
			{p->c = '\0'; fprintf(stderr, "cant find operand (t1)\n"); err = 1; break;}
		p = p->next = (tree) malloc(sizeof(struct tnode));
		if(!p) { fprintf(stderr, "cant allocate memory (t1)\n"); err = 1; return t; }
		
		p->dp = 1;
		p->next = NULL;
		p->fi = p->fo = NULL;
		p->argv = NULL;
		p->c = '\0';
		p->down = buildT0();
	}
	
	return t;
}

tree buildT0(){
	static char *s = "<>";
	tree t;
	if (!curlist) return NULL;
/*	t = (tree) malloc(sizeof(struct tnode));
	if(!t) { fprintf(stderr, "cant allocate memory (t0)\n"); err = 1; return NULL; } */
	
	if(curlist->tp == 's' && curlist->word[0] == '('){
		
		getnext();
		t = buildT3();
		if(!curlist || curlist->tp != 's' || curlist->word[0] != ')') 
			{ fprintf(stderr, "cant find bracket (t0)\n"); err = 1; return t; } /* ??? */
		getnext();
		
		while(curlist && curlist->tp != 'w' && find(curlist->word[0],s))
		if(curlist->word[0] == '<'){
			getnext();
			if(curlist && curlist->tp == 'w'){ 
			if(!t->fi) t->fi = strcopy(curlist->word);
			else fprintf(stderr, "warning: too many input files (t03)\n"), err = 1;
			getnext(); 
			}
			else fprintf(stderr, "warning: cant find input file (t03)\n"), err = 1;
			}
		else
		if(curlist->word[0] == '>'){
			char c = curlist->tp;
			getnext();
			if(curlist && curlist->tp == 'w'){ 
			if(!t->fo){t->fo = strcopy(curlist->word); t->tp = c;}
			else fprintf(stderr, "warning: too many output files (t03)\n"), err = 1;
			getnext(); 
			}
			else fprintf(stderr, "warning: cant find output file (t03)\n"), err = 1;
			}
		
	} else if(curlist->tp == 'w'){
		char *name;
		list l;
		int k = 1;
		
		t = (tree) malloc(sizeof(struct tnode));
		if(!t) { fprintf(stderr,"cant allocate memory (t0)\n"); err = 1; return NULL; }

		t->dp = 0;
		t->next = NULL;
		t->down = NULL;
		t->fi = t->fo = NULL;
		t->c = '\0';
		name = strcopy(curlist->word);

		if(!cmp(name,"cd")) t->c = 'c';
		else if(!cmp(name,"pwd")) t->c = 'p';
		else if(!cmp(name,"exit")) t->c = 'e';

		getnext();
		
		l = curlist;
		
		while(curlist && (curlist->tp == 'w' || find(curlist->word[0],s)) )
		if(curlist->tp == 'w'){k++; getnext();}
		else
		if(curlist->word[0] == '<'){
			getnext();
			if(curlist && curlist->tp == 'w'){ 
			if(!t->fi) t->fi = strcopy(curlist->word);
			else fprintf(stderr, "warning: too many input files (t00)\n"), err = 1;
			getnext(); 
			}
			else fprintf(stderr, "warning: cant find input file (t00)\n"), err = 1;
			}
		else
		if(curlist->word[0] == '>'){
			char c = curlist->tp;
			getnext();
			if(curlist && curlist->tp == 'w'){ 
			if(!t->fo){t->fo = strcopy(curlist->word); t->tp = c;}
			else fprintf(stderr, "warning: too many output files (t00)\n"), err = 1;
			getnext(); 
			}
			else fprintf(stderr, "warning: cant find output file (t00)\n"), err = 1;
			}
		
		t->argc = k;
		t->argv = (char**) malloc((k + 1) * sizeof(char*));
		t->argv[0] = name;
		t->argv[k] = NULL;
		k = 1;
		while(l && (l->tp == 'w' || find(l->word[0],s)) )
			if(l->tp == 'w'){t->argv[k++] = strcopy(l->word); l = l->next;}
			else{l = l->next; if(l && l->tp == 'w') l = l->next;}
	} else { fprintf(stderr, "wrong input (t0)\n"); err = 1; /* free(t)*/; return NULL; }
	
	return t;
}
