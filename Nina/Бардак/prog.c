#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <string.h>

typedef struct Node{
	pid_t pid;
	char *command;
	struct Node *next;	
} Node;


char * form_command(char *str1, char *str2, char *str3){

	char * tmp = (char *) malloc((strlen(str1) + strlen(str2) + strlen(str3) + 3)* sizeof(char));
	*tmp = '\0';
	strcat(tmp, str1);
	strcat(tmp, " ");
	strcat(tmp, str2);
	strcat(tmp, " ");
	strcat(tmp, str3);
	return tmp;
}

Node * add_elem(pid_t pid, char* command, Node * process){
	Node *tmp = (Node *)malloc(sizeof(Node));
	tmp->pid = pid;
	tmp->command = command;
	tmp->next = process;
	return tmp;
}

Node * rm_elem(pid_t pid, Node *process){

	Node *to_del;
	Node *start = process;
	if (start->pid == pid){
		to_del = start;
		start = start->next;
		free(to_del);
		return start;
	}

	while(start->next->pid != pid){
		start = start->next;
	}

	to_del = start->next;
	start->next = to_del->next;

	free(to_del);
	return process;
}

Node * find_in_list(pid_t pid, Node *process){

	Node *to_find = process;

	while (to_find != NULL && to_find->pid != pid ){
		to_find = to_find->next;
	}

	return to_find;

}

int main(int argc, char **argv) 
{
	char *command1, *command2;
	int status;
	if (argc == 7){

		Node *active_process = NULL, *ended;
		pid_t pid1, pid2, pid_return;
		printf("Параллельный запуск\n");

		if ( !(pid1 = fork()) )
		{
			// Процесс Сын (Вася)
			printf("Вася запущен\n");
			execlp(argv[1], argv[1], argv[2], argv[3], NULL);
			printf("%s: Исполняемый файл не найден!\n", argv[1]);
			exit(5);
		}

		else 
		{ 
			// Процесс Отец (Валерий Михайлович)
			command1 = form_command(argv[1], argv[2], argv[3]);
			active_process = add_elem(pid1, command1, active_process);

			if ( !(pid2 = fork()) )
			{
				// Процесс Сын (Иннокентий)
				printf("Иннокентий запущен\n");
				execlp(argv[4], argv[4], argv[5], argv[6], NULL);
				printf("%s: Исполняемый файл не найден!\n", argv[4]);
				exit(5);
			}

			else 
			{
				// Процесс Отец (Валерий Михайлович)
				command2 = form_command(argv[4], argv[5], argv[6]);
				active_process = add_elem(pid2, command2, active_process);
				while ((pid_return = waitpid(-1, &status, WNOHANG | 0)) != -1 ){
					ended = find_in_list(pid_return, active_process);
					if (ended){
						printf("Process with pid = %d, runned with cmd %s was exited with status %d\n", ended->pid, ended->command, WEXITSTATUS(status));
						active_process = rm_elem(pid_return, active_process);
					}
				}


			}
		}

		printf("Последовательный запуск\n");


		if ( !(pid1 = fork()) )
		{
			// Процесс Сын (Вася)
			printf("Вася запущен\n");
			execlp(argv[1], argv[1], argv[2], argv[3], NULL);
			printf("%s: Исполняемый файл не найден!\n", argv[1]);
			exit(5);
		}

		else 
		{ 
			// Процесс Отец (Валерий Михайлович)
			waitpid(pid1, &status, 0);
			printf("Process with pid = %d, runned with cmd %s was exited with status %d\n", pid1, command1, WEXITSTATUS(status));

			if ( !(pid2 = fork()) )
			{
				// Процесс Сын (Иннокентий)
				printf("Иннокентий запущен\n");
				execlp(argv[4], argv[4], argv[5], argv[6], NULL);
				printf("%s: Исполняемый файл не найден!\n", argv[4]);
				exit(5);
			}

			else 
			{
				// Процесс Отец (Валерий Михайлович)
				waitpid(pid2, &status, 0);
				printf("Process with pid = %d, runned with cmd %s was exited with status %d\n", pid2, command2, WEXITSTATUS(status));				
			}
		}		
	}

	return 0;
}
