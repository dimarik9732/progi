#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

//pr1|prp2|pr3

int main(int argc, char *argv[]){
	int fd1[2], fd2[2];
	int pid1,pid2,pid3/*,pid4*/;	

	if (pipe(fd1) < 0){
		perror("pipe");
		exit(1);
	}

	if ((pid1 = fork()) < 0){
		perror("fork");
		exit(1);
	}else if (pid1 == 0){//son1
		dup2(fd1[1], 1);
		close(fd1[0]);
		close(fd1[1]);
		execlp(argv[1], argv[1], NULL);
		perror("execlp");
		_exit(1);
	}

		if (pipe(fd2) < 0){
		perror("pipe");
		exit(1);
	}

	if ((pid2 = fork()) < 0){
		perror("fork");
		exit(1);

	} else if (pid2 == 0){//son2
		dup2(fd1[0], 0);
		dup2(fd2[1], 1);
		close(fd2[1]);
		close(fd2[0]);
		close(fd1[0]);
		close(fd1[1]);
		execlp(argv[2], argv[2], NULL);
		perror("execlp");
		_exit(1);
	}

	if ((pid3 = fork()) < 0){
		perror("fork\n");
		exit(1);
	}else if (pid3 == 0){//son3
		dup2(fd2[0], 0);
		close(fd2[0]);
		close(fd2[1]);
		execlp(argv[3], argv[3], NULL);
		perror("execlp\n");
		_exit(1);
	}

	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	return 0;
}