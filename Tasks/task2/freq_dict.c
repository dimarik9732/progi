#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#include "hash.h"

int main(int argc, char* argv[]){
	const size_t max_short_int = 65536;
	
	char * line;
	char *word, *temp;
	HASH hash_table;
	hash_table = (Node **) calloc(max_short_int, sizeof(Node *)); 
	int opt = getopt(argc, argv, "l");
	do {

		line = getline_unlim();
		if (*line == '\0') continue;

		while(*line != '\0'){
			if (opt == 'l') {
				for (temp = line; *temp != '\0'; temp ++){
					if(*temp >= 'A' && *temp <= 'Z'){*temp = *temp - 'A' + 'a';}
				}
			}

			word = line;
			while ((*line >= 'a' && *line <= 'z') || (*line >= 'A' && *line <= 'Z') || (*line >='0' && *line <='9') && *line != '\0'){
				line++;
			}

			if (*line == '\0') {
				add_to_hash(hash_table, word);
			}
			else { 
				*line = '\0';
			
				add_to_hash(hash_table,word);

				line++;
				word = line;
			}
		}

	} while(!feof(stdin));

	print_table(hash_table);


	return 0;
}