#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv){

	if (argc == 6){
		
		int status;
		int stat = 3;
		pid_t pid1, pid2;
		int exe;

		if (!strcmp(argv[3], ";")) stat = 0;
		else if (!strcmp(argv[3], "||")) stat = 1;
		else if (!strcmp(argv[3], "&&")) stat = 2;

		if ( (pid1 = fork()) == 0) {

			exe = execlp(argv[1], argv[1], argv [2], NULL);
			if (exe == -1) {
				printf("%s : Command not found\n", argv[1]);
				exit(5);
			}

		}

		else {
			waitpid(pid1,&status,0);
			switch (stat) {
			case 0:	
				if ( (pid2 = fork()) == 0) {
					
					exe = execlp(argv[4], argv[4], argv [5], NULL);
					
					if (exe == -1) { 
						printf("%s : Command not found\n", argv[4]);
						exit(5);
					}
				}
				else {
					waitpid(pid2,&status,0);
					if (WIFEXITED(status) != 0){
						exit(WEXITSTATUS(status));
						break;
					}	
				}
			case 1:
				if (status == 0) return 0;
				else {
					if ( (pid2 = fork()) == 0) {
						exe = execlp(argv[4], argv[4], argv [5], NULL);
						if (exe == -1) { 
							printf("%s : Command not found\n", argv[4]);
							exit(5);
						}
					}
					
					else {
						waitpid(pid2,&status,0);
						if (WIFEXITED(status) != 0){
							exit(WEXITSTATUS(status));
							break;
						}
					}	
				}
			case 2:
				if (WIFEXITED(status)){
					if(status != 0) exit(WEXITSTATUS(status));
					
					else {
						if ( (pid2 = fork()) == 0) {
							exe = execlp(argv[4], argv[4], argv [5], NULL);
							if (exe == -1) { 
								printf("%s : Command not found\n", argv[4]);
								exit(5);
							}
						}

						else { 
							waitpid(pid2,&status,0);
							if (WIFEXITED(status)){
								exit(WEXITSTATUS(status));
								break;
							}
						}
					}
				}
			default: 
				exit(1);
				break;
			}
		}
	}
	printf("Not allowed number of params\n");
	exit(3);
}
