#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>

// Ошибка
#define ERROR(x) \
	printf("Ошибка %s\n", x); \
	return 1	

struct cmd_inf 
{
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile; // переназначенный файл стандартного вывода
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
	cmd_inf* psubcmd; // команды для запуска в дочернем shell
	cmd_inf* pipe; // следующая команда после “|”
	cmd_inf* next; // следующая после “;” (или после “&”)
};

struct proc_list
{
	pid_t pid;
	int status;
	int pipe_num;
	struct proc_list *next;
};

int main(int argc, char **argv)
{
	char *cmd;
	struct cmd_inf *cmd_tree = NULL;
	struct proc_list *return_proc;
	int bckgrnd;
	char *alam;

	while(1)
	{
		printf(">");
		cmd = getline();

		if (strcmp(cmd, "exit") == 0){
			return 0;
		}

		if ( (alam = strchr(cmd, '&')) && *(alam + 1)!='&')
			backgrnd = 1;
		else backgrnd = 0;

		cmd_tree = parse(cmd);
		work(cmd_tree);

		int status;
		pid_t pid_return;
		// Фон. Сбор зомби
		// Пока у меня есть завершившиеся пр-сы и пока у меня вообще есть проц-сы
		while ((pid_return = waitpid(-1, &status, WNOHANG)) != 0 && pid_return != -1)
		{
			return_proc = find(pid_return);
			return_proc->pid = -1;
			return_proc->status = status;
			if(return_proc->pipe_num != -1)
				if(!(--proc_in_pipe_left[return_proc->pipe_num]))
				{
					printf("Ready %s. Status is %d\n", pipes_in_bckgrnd[return_proc->pipe_num],return_proc->status);
				}
		}

		if (backgrnd && pid_return != -1){
			while ((pid_return = waitpid(-1, &status, WNOHANG)) != -1)
			{
				return_proc = find(pid_return);
				return_proc->pid = -1;
				return_proc->status = status;
				if(return_proc->pipe_num != -1)
					if(!(--proc_in_pipe_left[return_proc->pipe_num]))
					{
						printf("Ready %s. Status is %d\n", pipes_in_bckgrnd[return_proc->pipe_num],return_proc->status);
					}
			}			
		}
	}

}