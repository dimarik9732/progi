/* sh_main */

#include "sh_exec.h"
#include "sh_globals.h"
#include "sh_tree.h"
#include "sh_utils.h"
#include "sh_list.h"

int main(int argc, char **argv){
	char *s;
	list l = NULL;
	tree t = NULL;
	char fl_l = 0, fl_t = 0, fl_p = 0, fl_i = 0, fl_e = 0;
	
	if(argc > 1){
		int i,j;
		for(i = 1; i < argc; i++)
		  if(argv[i][0] == '-')
		    for(j = 1; argv[i][j]; j++)
		    switch(argv[i][j]){
		      case 't': case 'T': fl_t = 1; break;
		      case 'l': case 'L': fl_l = 1; break;
		      case 'p': case 'P': fl_p = 1; break;
		      case 'i': case 'I': fl_i = 1; break;
		      case 'e': case 'E': fl_e = 1; break;
		    }
		  else break;
	}
	
	if(!fl_i) signal(SIGINT,SIG_IGN);
	if(!fl_e) printf("=>");
	while( s = readStr() ){
		
		l = buildList(s);
		if(fl_l){
		printf(" %d \n",listSize(l));
		printList(l);
		printf("\n");
		}

		err = 0;
		curlist = l;
		t = buildT3();
		if(curlist) fprintf(stderr,"unexpected operands\n");
		
		if(fl_t){
		printTree(t);
		printf("\n");
		}

		clearList(l);
		if(!err)
			exeT(t);
		else fprintf(stderr,"cant execute this\n");
		if(fake){fprintf(stderr,"unexpected error\n"); exit(0);}
		
		clearTree(t);
		free(s);

		if(fl_p){
		printf("\n");
		printzt();
		printf("\n");
		}

		clrzt();
		if(!fl_e) printf("=>");
	}
	
	return 0;
}
