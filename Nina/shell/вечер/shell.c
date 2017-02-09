#include "mod1.h"
// Ошибка
#define ERROR(x) \
	printf("Ошибка %s\n", x)

/*typedef struct cmd_inf 
{
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile_w;; // переназначенный файл стандартного вывода
	char *outfile_rw;
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
	cmd_inf *psubcmd; // команды для запуска в дочернем shell
	cmd_inf *pipe; // следующая команда после “|”
	cmd_inf *next; // следующая после “;” (или после “&”)
	cmd_inf *ornext; // ||
	cmd_inf *andnext; // &&
} cmd_inf;
*/

typedef struct proc_list
{
	pid_t pid;
	int status;
	int pipe_num;
	int is_last;
	struct proc_list *next;
} proc_list;

proc_list *run_proc;
int *pipes;

int find_free(void)
{
	int i = 0;
	while(pipes[i] != -1 && pipes[i])
	{
		i++;
	}

	if (pipes[i] == -1)
	{
		pipes = realloc(pipes, (i + 5)*sizeof(int));
		pipes[i+4] = -1;
		int j;
		for (j = i; j < i + 4; j++)
			pipes[j] = 0;
	}
	return i;
}

proc_list *find_in_list(proc_list *tmp, pid_t pid)
{
	while(tmp)
	{
		if(tmp->pid == pid)
			return tmp;
		else
			tmp = tmp->next;
	}
	return NULL;
}

int find_last_status(int pipe_num)
{
	proc_list *tmp = run_proc;
	while(tmp)
	{
		if(tmp->pipe_num == pipe_num && tmp->is_last == 1)
			return tmp->status;
		tmp = tmp->next;
	}

	return -1;
}

// Ф-я прохода по дереву
int go_tree(cmd_inf *tree, int old_status)
{
	if (!tree){
		return 0;
	}
	
	int pipe_num = find_free();
	printf("[--%d--]\n", pipe_num);
	int i = 0, size = 2, fdo, fdi, last_last_status, j;
	int **fd_pipe = calloc(size, sizeof(int*)); // Массив каналов
	cmd_inf *cur_pipe = tree; 
	pid_t pid_new, pid_ret;
	proc_list *proc_ret;
	if (old_status){

		if (tree->psubcmd){
			if ( (pid_new = fork()) == 0 )
			{
				// Сын
				int last_last_status;
				last_last_status = go_tree(tree->psubcmd, 1);
				exit(last_last_status);
			}
			else {
				// Отец
				proc_list* tmp = calloc(1, sizeof(proc_list));
				tmp-> pid = pid_new;
				tmp->pipe_num = pipe_num;
				if (!cur_pipe->pipe){
				tmp->is_last = 1;
			}
		tmp->next = run_proc;
		run_proc = tmp;
			}
		}
			do
			{
				if(i >= size)
				{
					size*=2;
					fd_pipe = (int **) realloc(fd_pipe, size*sizeof(int*));
				}

				else 
				{
					fd_pipe[i] = (int*) calloc(2, sizeof(int));
					pipe(fd_pipe[i]);
		
					// Если первый то канал ввода не нужен
					if (i == 0)
					{
						//close(fd_pipe[i][0]);
						//fd_pipe[i][0] = 0;
					}

					// Если последний то канал вывода не нужен
					if (!cur_pipe->pipe)
					{
						close(fd_pipe[i][1]);
						close(fd_pipe[i][0]);
						//fd_pipe[i][1] = 1;
					}
				}

				fdo = -1;
				fdi = -1;
		
				if(cur_pipe->outfile_w != NULL)
					fdo = open(cur_pipe->outfile_w, O_CREAT | O_TRUNC | O_WRONLY, 0660 );

				if(cur_pipe->outfile_rw != NULL)
					fdo = open(cur_pipe->outfile_rw, O_CREAT | O_WRONLY | O_APPEND, 0660 );
		
				if(cur_pipe->infile != NULL || cur_pipe->backgrnd)
				{
					if (cur_pipe->backgrnd)
						fdi = open("/dev/null", O_RDONLY, 0);	
				else 
					if ((fdi = open(cur_pipe->infile, O_RDONLY, 0)) < 0)
						ERROR("нет файла");
				}

				if(!(pid_new = fork()))
				{
					// Сын
					if(i > 0)
					{
						dup2(fd_pipe[i-1][0], 0);
						close(fd_pipe[i-1][0]);
					}

					if (cur_pipe->pipe)
					{
						dup2(fd_pipe[i][1], 1);
						close(fd_pipe[i][1]);
					}

					if (fdo > 0)
					{
						dup2(fdo, 1);
						close(fdo);
					}

					if (fdi > 0)
					{
						dup2(fdi, 0);
						close(fdi);
					}

					execvp(cur_pipe->argv[0], cur_pipe->argv);
				} 
					// Отец
				if (i > 0)
					close(fd_pipe[i-1][0]);
				if (cur_pipe->pipe && fd_pipe[i][1] != 1)
					close(fd_pipe[i][1]);
				if (fdo > 0)
					close(fdo);
				if (fdi > 0)
					close(fdi);

				proc_list* tmp = calloc(1, sizeof(proc_list));
				tmp-> pid = pid_new;
				tmp->pipe_num = pipe_num;
				if (!cur_pipe->pipe){
					tmp->is_last = 1;
				}
				tmp->next = run_proc;
				run_proc = tmp;

				pipes[pipe_num]++;		
				i++;
				cur_pipe = cur_pipe->pipe;
		} while (cur_pipe != NULL);
		for (j = 0; j < i; j++)
			free(fd_pipe[j]);
		free(fd_pipe);

		if (tree->backgrnd){
			if(tree->next) 
				last_last_status = go_tree(tree->next, 1);
		}
		else
		{
			while (pipes[pipe_num])
			{
				int status;
				pid_ret = waitpid(-1, &status, WNOHANG);
				if (pid_ret == -1 || pid_ret == 0){
					continue;
				}
				proc_ret = find_in_list(run_proc, pid_ret);
				if (WIFEXITED(status))
					proc_ret->status = WEXITSTATUS(status);
				else
					if (WIFSIGNALED(status))
						proc_ret->status = -1;
					else proc_ret->status = -5;
				
				//if(proc_ret->pipe_num != -1){
					pipes[proc_ret->pipe_num]--;
					if(pipes[proc_ret->pipe_num] == 0)
						printf("Готово [--%d--]. Статус = %d\n", proc_ret->pipe_num, find_last_status(proc_ret->pipe_num));
				//}
			}
		}

		// Команды с условным выполнением
		int last_status = find_last_status(pipe_num); 
		if( tree->andnext)
			if (last_status == 0)
				last_last_status = go_tree(tree->andnext, 1);
			else 
				last_last_status = go_tree(tree->andnext, 0);

		if(tree->ornext)
			if(last_status)
				last_last_status = go_tree(tree->ornext, 1);
			else
				last_last_status = go_tree(tree->ornext, 0);
		if (tree->next){
			last_last_status = go_tree(tree->next, 1);
		}

		return last_last_status;
	}
	else {
		if (tree->andnext)
			last_last_status = go_tree(tree->andnext, 0);
		if (tree->ornext)
			last_last_status = go_tree(tree->ornext, 1);
		if (tree->next)
			last_last_status = go_tree(tree->next, 1);	
		return last_last_status;			
	}
}
/*
cmd_inf *parse2(cmd_inf *tree, string_list **list)
{
	if(!*list)
		return tree;
	int size = 3, flag;
	cmd_inf *tmp = tree, *cur_pipe;
	if(!tmp)
	{
		i = 0;
		tmp = calloc(1, sizeof(cmd_inf));
		tree = tmp;
	}

		// Спец символ
		while (*list){

			switch(list->word[0])
			{

				case 1:
					*list = *list->next;
					tmp->outfile_w = (*list)->word;
					break;
				case 2: 
					*list = *list->next;
					tmp->outfile_rw = (*list)->word;
					break;
				case 3:
					*list = *list->next;
					tmp->infile = (*list)->word;
					break;
				case 4:
					tmp->backgrnd = 1;
					parse2(tmp->next, &list->next);
					break;
				case 5:
					tmp->andnext = parse2(tmp->andnext, &list->next);
					break;
				case 6:
					int flag = 1;
					while(*list && flag) 
					{
						if((*list)->word[0] > 10)
						{
							if(!tmp->argv)
								tmp->argv = (char **)calloc(size, sizeof(char*));
				
							if(i >= size)
							{
								size *= 2;
								tmp->argv = (char **)realloc(tmp->argv, size*sizeof(char*));
							}

							tmp->argv[i] = *list->word;
							i++;
							}
						}
						else
							if((*list)->word[0] == 6)
							{
								i = 0;
								tmp = calloc(1, sizeof(cmd_inf));
								tree = tmp;
							}
							else
								flag = 0;
						(*list) = (*list)->next;
					}
					continue;
					break;
				case 7:
					parse2(tmp->ornext, &list->next);
					break;
				case 8:
					parse2(tmp->psubcmd, &list->next);
					break;
				case 9:
					return tree;
					break;
				case 10:
					parse2(tmp->next, &list->next);
					break;
			default:
				// Не спец символ
				if(!tmp->argv)
					tmp->argv = (char **)calloc(size, sizeof(char*));
				
				if(i >= size)
				{
					size *= 2;
					tmp->argv = (char **)realloc(tmp->argv, size*sizeof(char*));
				}

				tmp->argv[i] = *list->word;
				i++;
				break;
		}

		*list = *list->next;
}
*/

int main(int argc, char **argv)
{
	pipes = calloc(3, sizeof(int));
	while(1)
	{
		printf("home$ ");
		
		string_list *lexems = parse();

		cmd_inf *tree = NULL;
		tree = parse2(tree, lexems);
		
		pipes[2] = -1;

		int last_last_status = go_tree(tree, 1);

		int status;
		pid_t pid_return;
		proc_list *proc_ret;
		// Фон. Сбор зомби
		// Пока у меня есть завершившиеся пр-сы и пока у меня вообще есть проц-сы
		while ((pid_return = waitpid(-1, &status, WNOHANG)) != 0 && pid_return != -1)
		{
			proc_ret = find_in_list(run_proc, pid_return);
			proc_ret->pid = -1;
			
			if (WIFEXITED(status))
					proc_ret->status = WEXITSTATUS(status);
				else
					if (WIFSIGNALED(status))
						proc_ret->status = -1;
					else proc_ret->status = -5;

			//if(return_proc->pipe_num != -1)
				if(!(--pipes[proc_ret->pipe_num]))
				{
					printf("Готово [--%d--]. Status is %d\n", proc_ret->pipe_num, find_last_status(proc_ret->pipe_num));
				}
		}
		print_list(lexems);
		free_tree(tree);
	}
}