#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int pid1, pid2, fd[2];
char *prs1[4];
char *prs2[3];
prs1[0] = calloc(3, sizeof(char));
prs1[1] = calloc(3, sizeof(char));
prs1[0][0] = 'l';
prs1[0][1] = 's';
prs1[0][2] = '\0';
prs1[1][0] = '-';
prs1[1][1] = 'a';
prs1[1][2] = '\0';
prs1[2] = NULL;
prs1[3] = NULL;

prs2[0] = calloc(5, sizeof(char));
prs2[1] = calloc(3, sizeof(char));
prs2[0][0] = 's';
prs2[0][1] = 'o';
prs2[0][2] = 'r';
prs2[0][3] = 't';
prs2[0][4] = '\0';
prs2[1][0] = '-';
prs2[1][1] = 'r';
prs2[1][2] = '\0';
prs2[2] = NULL;

	pipe(fd);
	if ((pid1 = fork()) == 0){
		printf("1\n");
		dup2(fd[1],1);
		close(fd[1]);
		close(fd[0]);
		execvp(prs1[0], prs1);
		perror("exec1");
		_exit(1);
	}
	if ((pid2 = fork()) == 0){
		printf("2\n");
		dup2(fd[0],0);
		close(fd[0]);
		close(fd[1]);
		execvp(prs2[0], prs2);
		perror("exec2");
		_exit(1);
	}
	printf("3\n");
	close(fd[1]); close(fd[0]);
	wait(NULL); wait(NULL);
	return 0;
}


