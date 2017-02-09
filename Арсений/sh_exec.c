/* sh_exec.c */

#include "sh_exec.h"

void addzt(pid_t pid){
	if(ztable.n >= ztable.tsize){
	ztable.pids = (pid_t*) realloc(ztable.pids, (ztable.tsize + blockSize) * sizeof(pid_t));
	if(!ztable.pids) fprintf(stderr, "cant allocate memory (zt)\n");
	else {ztable.tsize += blockSize; ztable.pids[ztable.n++] = pid;}
	} else ztable.pids[ztable.n++] = pid;
}

void clrzt(){
	int i = 0;
	while(i < ztable.n)
		if(waitpid(ztable.pids[i],NULL,WNOHANG)) ztable.pids[i] = ztable.pids[--ztable.n];
		else i++;
	if(ztable.n < ztable.tsize - 2*blockSize)
		if( !( ztable.pids = (pid_t*) realloc(ztable.pids, (ztable.tsize - blockSize) * sizeof(pid_t)) ) )
					fprintf(stderr, "cant allocate memory (clrzt)\n");
		else ztable.tsize -= blockSize;
}

void printzt(){
	int i;
	for(i = 0; i < ztable.n; i++) printf("%3d: %d\n",i,(int) ztable.pids[i]);
	printf("size: %d; n: %d\n",ztable.tsize,ztable.n);
}

int e_exit(){
 exit(0);
 return -1;
}

int e_cd(int argc, char **argv){
 if(argc > 1) return chdir(argv[1]);
 else return chdir(getenv("HOME"));
}

int e_pwd(int fd){ /* ... */
static char buf[300];
static char eoln = '\n';
int l;
getcwd(buf,300);
l = leng(buf);
return (write(fd,buf,l) > 0) ? write(fd,&eoln,1) > 0 : 0;
}

int exeT(tree t){
	if(!t) return 0;
	
	switch(t->dp){
		
		case 3:{
			int r = 0;
			if(t && fake){
				if(t->fi){int f = open(t->fi,O_RDONLY); dup2(f,0); close(f);}
				if(t->fo){int f; if(t->tp =='s') f = open(t->fo,O_CREAT | O_TRUNC | O_WRONLY,S_IREAD | S_IWRITE);
				       		else f = open(t->fo, O_CREAT | O_APPEND | O_WRONLY, S_IREAD | S_IWRITE);
					dup2(f,1); close(f); }
			}
			while(t){
				if(t->c == '&'){
					pid_t p;
					p = fork();
					if(p){addzt(p);	t = t->next;}
					else{
						if(!fake){int f = open("/dev/null",O_RDONLY); dup2(f,0); close(f);} /* close(0);*/
						fake = 1;
						signal(SIGINT, SIG_IGN);
						exeT(t->down);
						exit(0); /* ??? */
					}
				} else if(t->c == ';'){
					r = exeT(t->down);
					t = t->next;
				} else return 0; /* ???? */
			}
			return r;
		}
		
		case 2:{
			int r = 0;
			while(t){
				r = exeT(t->down);
				if(t->c == '&' && !r || t->c == '|' && r) t = t->next;
					else return r;
			}
			return r;
		}
		
		case 1:{
			int i, n = 1, r = 0, f = 0, p[2];
			tree l; pid_t *tab;
			if(!t) return 0; /* ??? */
			l = t;
			while(l->next){if(!(l && l->down && l->down->dp == 0 && l->down->c != '\0') ) n++; l = l->next;}
			tab = (pid_t*) malloc(n * sizeof(pid_t));
			i = 0;
			while(t->next) if(t->down && t->down->dp == 0 && t->down->c != '\0'){
				char c;
				c = t->down->c;
				pipe(p);
				if(c == 'e') e_exit(); else
				if(c == 'c') e_cd(t->down->argc, t->down->argv); else
				if(c == 'p') {int z = p[1]; if(t->down->fo){
			       		if(t->down->tp == 's') z = open(t->down->fo,O_WRONLY | O_CREAT | O_TRUNC,S_IREAD | S_IWRITE);
					else z = open(t->down->fo,O_APPEND | O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);	
					}
				e_pwd(z);
				if(t->down->fo) close(z);
				}
				if(f) close(f); close(p[1]); f = p[0];
					t = t->next; /* ?? */
			} else {
				pipe(p);
				if(!(tab[i++] = fork())){
					if(!fake) signal(SIGINT,SIG_DFL);
					fake = 1;
					if(f) {dup2(f,0); close(f);}
					dup2(p[1],1);
					close(p[0]); close(p[1]);
					exeT(t->down);
					exit(0);
				}
				if(f) close(f); close(p[1]); f = p[0];
				t = t->next;
			}

			if(t && t->down && t->down->dp == 0 && t->down->c != '\0'){
				char c;
				c = t->down->c;
				if(f) close(f);
				if(c == 'e') e_exit(); else
				if(c == 'c') e_cd(t->down->argc,t->down->argv); else
				if(c == 'p') {int z = 1;
					if(t->down->fo){
						if(t->down->tp == 's') z = open(t->down->fo,O_WRONLY | O_CREAT | O_TRUNC,S_IREAD | S_IWRITE);
						else z = open(t->down->fo,O_WRONLY | O_CREAT | O_APPEND,S_IREAD | S_IWRITE);
					}
					e_pwd(z);
					if(t->down->fo) close(z);
				}
				
			} else
			if(!(tab[i] = fork())){
				if(!fake) signal(SIGINT,SIG_DFL);
				fake = 1;
				if(f) {dup2(f,0); close(f);}
				exeT(t->down);
				exit(0);
			}
			if(f) close(f);
			for(i = 0; i < n; i++){
				int s;
				waitpid(tab[i],&s,0);
				if(WIFEXITED(s)) r = WEXITSTATUS(s); else r = -1; /* ???????? */
				}
			free(tab);
			return r;
		}
		
		case 0:{
			if(t && fake){
				if(t->fi){int f = open(t->fi,O_RDONLY); dup2(f,0); close(f);}
				if(t->fo){int f; if(t->tp =='s') f = open(t->fo,O_CREAT | O_TRUNC | O_WRONLY,S_IREAD | S_IWRITE);
				       		else f = open(t->fo, O_CREAT | O_APPEND | O_WRONLY,S_IREAD | S_IWRITE);
					dup2(f,1); close(f); }
			}
			if(!t) exit(0);
			if(t->argv)
				execvp(t->argv[0], t->argv);
			exit(0);
		}
		
	}
}
