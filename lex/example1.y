%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct cmd_inf 
{
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile_w;; // переназначенный файл стандартного вывода
	char *outfile_rw;
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
	cmd_inf *psubcmd; // команды для запуска в дочернем shell
	cmd_inf *pipe; // следующая команда после “|”
	cmd_inf *next; // следующая после “;” (или после “&”)
	cmd_inf *ornext; // ||
	cmd_inf *andnext; // &&
} cmd_inf;

cmd_inf tree, tmp;
%}

%start simpleCommand

%token OBRACKET CBRACKET NEWLINE WORD

%left PIPE FORCEOUTADD FORCEOUT FORCEIN
%left AND OR
%left BACKGROUND NEXT
%% /*rules*/
/*
shellCommand: ifCommand 
			| shellCommand NEXT ifCommand
			| shellCommand BACKGROUND ifCommand
			;
ifCommand:	command
		 |	ifCommand AND command 
		 |	ifCommand OR command
		 ;
command:	force pipe
	   |	pipe force
	   |	OBRACKET shellCommand CBRACKET
	   ;
force:  forceout forcein
	 |  forcein forceout
	 ;
forcein: /*empty
	   | FORCEIN WORD
	   ;
forceout: /*empty
		| FORCEOUT WORD
		| FORCEOUTADD WORD
		;
pipe: simpleCommand
	| pipe PIPE simpleCommand
	; */
simpleCommand: /*empty*/
			 | simpleCommand WORD
			 {

			 }
			 | WORD 
			 { 
			   printf("%s\n",$1);
			   //tmp = tree = (cmd_inf*)calloc(1, sizeof(cmd_inf)); 
			   //tmp->argv = (char**) calloc(2, sizeof(char *)); 
			   //tmp->argv[0] = strdup($1);
			   //i++;
			 }
//			 {
//			 	printf("simpleCommand \n");
//			 }	
			 ;

%%
void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
  
main()
{
		tree = NULL;
        yyparse();
} 
