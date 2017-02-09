%{
#include <stdio.h>
#include "y.tab.h"
%}
%%
[a-zA-Z0-9\.]+  printf("WORD "); return WORD;
[\n]			printf("\n"); return NEWLINE;
[\&][\&]		printf("AND "); return AND;
\&				printf("BACKGROUND "); return BACKGROUND;
;				printf("NEXT "); return NEXT;
\( 				printf("OBRACKET "); return OBRACKET;
\)				printf("CBRACKET "); return CBRACKET;
\|				printf("PIPE "); return PIPE;
[\|][\|]		printf("OR "); return OR;
[ \f\r\t\v]+ 			/*empty*/
\>\> 			printf("FORCEOUTADD "); return FORCEOUTADD;
\>				printf("FORCEOUT "); return FORCEOUT;
\<				printf("FORCEIN "); return FORCEIN;
%%
