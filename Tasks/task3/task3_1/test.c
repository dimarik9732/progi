#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _STDOUT 1
#define _STDERR 2

int main(int argc, char const *argv[]){
	int status, off, len, total = 0, suc = 0;
	pid_t pid;
	int fd_pipe[2];
	char result[8];
	char * razd[3] = {"||", "&&", ";" };
	const char * testp[2] = {"success", "false"};
	const char * testc[2] = {"echo", "false"};	
	char *command;
	char *n;
	int fd;
	const char *path = "./shell_andor";

/*	fd = open("./success", O_CREAT | O_TRUNC | O_RDWR | O_EXCL, 0666);
	
//	printf ("%d\n", fd);
	if (fd > 0){
		write(fd, "success\n", 8);
		close(fd);
	}	
*/
	if ((pid = fork()) == 0){
		// Потомок
		execlp("gcc", "gcc", "-g", "./shell_andor.c", "-o", path, NULL);
	} 
	else {
		// Родитель
		//system("ps af");

		wait(&status);

		int i, j, k, u;
		
		for (i = 0; i < 3; i++)	{
			for (j = 0; j < 2; j++) {
				for ( k = 0; k < 2; k++) {

					total ++;
					command = malloc(sizeof(char)*(strlen(testc[j]) + strlen(testp[j]) + strlen(testc[k]) + strlen(testp[k]) + strlen(razd[i]) + 5));
					command[0] = '\0';

					strcat(command, testc[j]);
					strcat(command, " ");
					strcat(command, testp[j]);
					strcat(command, " ");
					strcat(command, razd[i]);
					strcat(command, " ");
					strcat(command, testc[k]);
					strcat(command, " ");
					strcat(command, testp[k]);
					strcat(command, "\n");
					write(_STDOUT, command, strlen(command));
					
					pipe(fd_pipe);

					if ((pid = fork()) == 0){
					//	dup2(fd_pipe[1],_STDOUT);
					//	dup2(fd_pipe[1],_STDERR);
					//	close(fd_pipe[1]);
					//	close(fd_pipe[0]);
						execlp(path, path, testc[j], testp[j], razd[i], testc[k], testp[k], NULL);
					}

					else{
					//	close(fd_pipe[1]);
						wait(&status);
					//	off = 0;
					/*	while(read(fd_pipe[0],result, 8)){

							n = strchr(result, '\n');
							
							if (n) {
								*n = '\0';
								len = strlen(result);
								*n = '\n';
								write(_STDOUT, result, len + 1);
								result[0] = '\0';
								off = 0;
								break;
							}

							off += 20;
						}
					*/
						switch(i){
							case 0:
								if (j == 0 || k == 0) {if (status == 0) suc++;}
								else if (status != 0) suc++;
								break;
							case 1: 
								if ( j == 0 && k == 0) { if (status == 0) suc++; }
								else if (status != 0) suc++;
								break;
							case 2:
								if (k == 0) {if (status == 0) suc++;}
								else if (status != 0) suc++;
								break;
						}		
					//	close(fd_pipe[0]);
					}						
				}							
			}
		}
			
		time_t time_sec_until;
		time_t time_sec_after;

		total++;
		printf("sleep 5 && sleep 3\n");				
		time(&time_sec_until);

		if ((pid = fork()) == 0){
			execlp(path, path, "sleep", "5", "&&", "sleep", "3", NULL);
		}

		else{
			wait4(pid, &status, 0, NULL);
			time(&time_sec_after);
			unsigned int time_dif;
			if ((time_sec_after - time_sec_until) >= 8) suc++;
			time_dif = time_sec_after - time_sec_until;
			printf("%u sec\n", time_dif);
		}
		
		total++;
		printf("false abc && sleep 5");
		time(&time_sec_until);

		if ((pid = fork()) == 0){
			execlp(path, path, "false", "abc", "&&", "sleep", "5", NULL);
		}

		else{
						wait4(pid, &status, 0, NULL);
						time(&time_sec_after);
						unsigned int time_dif;
						if ((time_sec_after - time_sec_until) < 5) suc++;
						time_dif = time_sec_after - time_sec_until;
						printf("%u sec\n", time_dif);
		}

		total++;
		printf("sleep 5 || sleep 3\n");		
		time(&time_sec_until);
		if ((pid = fork()) == 0){
			execlp(path, path, "sleep", "5", "||", "sleep", "3", NULL);
		}
		else{

			wait4(pid, &status, 0, NULL);
			time(&time_sec_after);
			unsigned int time_dif;
			if ((time_sec_after - time_sec_until) < 8) suc++;
			time_dif = time_sec_after - time_sec_until;
			printf("%u sec\n", time_dif);
		}

		total++;
		printf("true true || sleep 100\n");		
		time(&time_sec_until);
		if ((pid = fork()) == 0){
			execlp(path, path, "true", "true", "||", "sleep", "100", NULL);
		}
		else{

			wait4(pid, &status, 0, NULL);
			time(&time_sec_after);
			unsigned int time_dif;
			if ((time_sec_after - time_sec_until) < 100) suc++;
			time_dif = time_sec_after - time_sec_until;
			printf("%u sec\n", time_dif);
		}

		total++;
		printf("sleep 3 ; sleep 5\n");		
		time(&time_sec_until);
		if ((pid = fork()) == 0){
			execlp(path, path, "sleep", "3", ";", "sleep", "5", NULL);
		}
		else{

			wait4(pid, &status, 0, NULL);
			time(&time_sec_after);
			unsigned int time_dif;
			if ((time_sec_after - time_sec_until) <= 7) suc++;
			time_dif = time_sec_after - time_sec_until;
			printf("%u sec\n", time_dif);
		}

		printf("sucsess / total : %d / %d\n", suc, total);
		return 0;
	}
}
