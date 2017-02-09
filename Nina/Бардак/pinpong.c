#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


pid_t pid;

void sighndlr1(int sig){
	kill(pid, SIGKILL);
	wait(NULL);
	while (1){
		printf("father\n");
		sleep(1);
	}
}

void sighndlr2(int sig){
	printf("father\n");
	kill(pid, SIGUSR1);
	return;
}

void sighndlr3(int sig){
	printf("son\n");
	kill(getppid(), SIGUSR1);
	return;
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGALRM, sighndlr1);
	signal(SIGUSR1, sighndlr2);

	if ((pid = fork()) > 0){
		alarm(3);
		while(1)
			pause();
	//	while(1)
	//		printf("father\n");
	}
	else { 
		if(!pid) {
			signal(SIGUSR1, sighndlr3);
			kill(getppid(),SIGUSR1);
			while(1)
				pause();
		}
	}
	return 0;
}