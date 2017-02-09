#include <stdio.h>
#include <unistd.h>

struct args {

	int flag_e;
	int flag_E;
	int flag_n;

};

struct args global_args = {0, 0, 0};

void print_with_mods(char * line){

	if (global_args.flag_n){
		printf("%s", line);
	}



}



int main (int argc, char **argv){

	opterr = 0;
	int opt;
	int err = 0;
	while ( (opt = getopt(argc, argv, "eEn")) != -1 || err) {
		switch (opt){
			case 'e': 
				global_args.flag_e = 1;
				global_args.flag_E = 0;
				break;
			case 'E': 
				global_args.flag_e = 0;
				global_args.flag_E = 1;
				break;
			case 'n': 
				global_args.flag_n = 1;
				break;
			default:
				print_with_mods(argv[optind]);
				err = 1;
				break; 

		}


	}
	printf("%s\n", argv[1]); 

}