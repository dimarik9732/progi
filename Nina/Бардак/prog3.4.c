#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){

	pid_t pid;
	int file_des[2];

	pipe(file_des);

	if ( !(pid = fork()) ){

		dup2(file_des[1],  1);
		close(file_des[1]);
		close(file_des[0]);
		execlp("bin/prog2", "bin/prog2", NULL);
	}
	else {
		close(file_des[1]);
		char result[14];
		read(file_des[0], result, 100);
		result[13] = '\0';
		printf("%s\n", result);
		close(file_des[0]);
	}
	return 0;
}