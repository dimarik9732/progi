#include <stdio.h>
#include <stdlib.h> //exit
#include <sys/types.h> //wait, fork
#include <unistd.h> //exit, execlp, fork
#include <sys/wait.h> //wait
#include <string.h> //strlen

typedef struct Node{
	pid_t pid;
	char *command;
} Node;

char *form_command(char *str1, char *str2, char *str3)
{
	char * tmp = (char *) malloc((strlen(str1) + strlen(str2) + strlen(str3) + 3)* sizeof(char));
	sprintf(tmp, "%s %s %s", str1, str2, str3);
	return tmp;
}

int main(int argc, char **argv) 
{
	int status;
	if (argc == 7){

	Node proc1, proc2, ended;
	pid_t pid1, pid2, pid_return;
	
	printf("Параллельный запуск\n");

		if ( !(pid1 = fork()) )
		{
			// Процесс Сын1
			printf("Сын1 запущен\n");
			execlp(argv[1], argv[1], argv[2], argv[3], NULL);
			printf("%s: Исполняемый файл не найден!\n", argv[1]);
			exit(5);
		}

		else 
		{ 
			// Процесс Отец
			proc1.pid = pid1;
			proc1.command = form_command(argv[1], argv[2], argv[3]);

			if ( !(pid2 = fork()) )
			{
				// Процесс Сын2
				printf("Сын2 запущен\n");
				execlp(argv[4], argv[4], argv[5], argv[6], NULL);
				printf("%s: Исполняемый файл не найден!\n", argv[4]);
				exit(5);
			}

			else 
			{
				// Процесс Отец
				proc2.pid = pid2;
				proc2.command = form_command(argv[4], argv[5], argv[6]);

				while ((pid_return = waitpid(-1, &status, WNOHANG | 0)) != -1 )
				{
					ended.pid = 0;
					if(pid_return == proc1.pid) ended = proc1;
					if(pid_return == proc2.pid) ended = proc2;

					if (ended.pid)
					{
						printf("Process with pid = %d, ran with cmd %s was exited with status %d\n", 
							ended.pid,
							ended.command, 
							WEXITSTATUS(status));
					}
				}
			}
		}

		printf("Последовательный запуск\n");

		if ( !(pid1 = fork()) )
		{
			// Процесс Сын1
			printf("Сын1 запущен\n");
			execlp(argv[1], argv[1], argv[2], argv[3], NULL);
			printf("%s: Исполняемый файл не найден!\n", argv[1]);
			exit(5);
		}

		else 
		{ 
			// Процесс Отец
			waitpid(pid1, &status, 0);
			printf("Process with pid = %d, ran with cmd %s was exited with status %d\n", pid1, proc1.command, WEXITSTATUS(status));

			if ( !(pid2 = fork()) )
			{
				// Процесс Сын2
				printf("Сын2 запущен\n");
				execlp(argv[4], argv[4], argv[5], argv[6], NULL);
				printf("%s: Исполняемый файл не найден!\n", argv[4]);
				exit(5);
			}

			else 
			{
				// Процесс Отец
				waitpid(pid2, &status, 0);
				printf("Process with pid = %d, ran with cmd %s was exited with status %d\n", pid2, proc2.command, WEXITSTATUS(status));				
			}
		}		
	}

	return 0;
}
