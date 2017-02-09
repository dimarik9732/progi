#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char const *argv[])
{
	int fd1[2], status;
	pid_t pid;
	if (argc == 4){

		pipe(fd1);
		if(!fork()){
			//Сын
			close(fd1[0]);
			dup2(fd1[1], 1);

			if (!(pid = fork())){
				// Внук 1
				execlp(argv[1], argv[1], NULL);
				perror("exec");
				exit(3);
			}
			else{
				// Сын
				waitpid(pid, NULL, 0);
				if (!(pid = fork())){
					// Внук 2
					execlp(argv[2], argv[2], NULL);
					perror("exec");
					exit(3);
				}
				else{
				// Сын
					waitpid(pid, &status, 0);
					if(WIFEXITED(status)){
						return WEXITSTATUS(status);
					}
				}
			}
		}
		else {
			close(fd1[1]);
			if (!fork()){
				// Сын 2
				dup2(fd1[0], 0);
				close(fd1[0]);
				execlp(argv[3], argv[3], NULL);
				perror("exec");
				exit(3);
			}
			else {
				close(fd1[0]);
				while((pid = waitpid(-1, NULL, WNOHANG)) != -1) 
					if (pid != 0)
						printf("Process %d finaled\n", pid); 
				return 0;
			}	
		}
	}
}
