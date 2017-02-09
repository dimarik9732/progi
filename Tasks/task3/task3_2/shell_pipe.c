#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


typedef struct Node
{
	pid_t pid;
	char * cmd;
	int status;
	int if_exited;
	int pipe_num; 
	struct Node *next;	
} Node;

typedef struct cmd_Tree
{	
	char **argv;
//	char *infile;
	char *outfile;
	int background;
//	struct cmd_Tree *psubcmd;
	struct cmd_Tree *pipe;
//	struct cmd_Tree *next;
} cmd_Tree;

int pipes[20];
char *pipes_cmd[20];

int find_free(void){

	int j= 0;
	while (j < 20 && pipes[j] != 0)
		j++;

	return j;
}


char*  getline_unlim(){

		enum {
			FGETS_BUFSIZE=30
		};

		int flag_newline = 0;	// флаг конца строки
		
		unsigned long block_size = FGETS_BUFSIZE;	// размер блока считывания
		
		// Создание пустой строки

		char * line = (char *) malloc(block_size*sizeof(char));
		
		// Считывание строки

		char * character; // указатель на элемент строки
		char * result;
		
		while(!flag_newline && !feof(stdin)){
				
			// Считывание block_size элементов из ввода
			result = fgets(line + strlen(line),block_size,stdin);
			if (!result){
				if (!feof(stdin)) 
					{ 
						fprintf(stderr, "%s", strerror(errno));
						exit(EXIT_FAILURE);
					}
			}

			else {

				// Поиск символа перевода строки

				character = strchr(line,'\n');
				
				if (character)
				{
					*character = '\0';
					flag_newline = 1;
				}

				else {				

						// Выделение дополнительной памяти
					
						block_size *= 2;

						line = realloc(line,strlen(line)+block_size);
					}
			}
		
		}

			return line;
}


Node *find_in_list(pid_t pid, Node *list){
	while( list && (list->pid != pid) )
		list = list->next;
	return list;
}

Node *add_to_list(pid_t pid, char* cmd, Node *list){
	Node *tmp = (Node *) calloc(sizeof(Node),1);
	tmp->pid = pid;
	tmp->cmd = cmd;
	tmp->next = list;
	tmp->status = -1;
	tmp->if_exited = 0;
	tmp->pipe_num = 0;
	return tmp;
}

cmd_Tree *add_to_tree(char *command){

	cmd_Tree *tmp = (cmd_Tree *) calloc(sizeof(cmd_Tree),1);

	char *forward;
	char *output;
	forward = strchr(command, '>');

	if (forward){
		*forward = '\0';
		output = (char*) calloc(sizeof(char), 256);
		sscanf(forward + 1, "%256s", output);
		tmp->outfile = output;
	}
	else tmp->outfile = NULL;

	// Формирование списка аргументов

	char ** argv;
	char *cur_param;
	volatile int j = 0;
	int state = 0, end_f = 0;
	int size = 1;
	char *cur = command;

	
	tmp->argv = (char **) calloc(sizeof(char*),1);

	while ( !end_f ){
		switch(state){

			case 0: 
				if (*cur == '\0') {
					end_f = 1;
					break;
				}

				if (!isspace(*cur)){ 
					state = 1; 
					cur_param = cur;
				}

				cur++;
				break;

			case 1: 	
				if (!isspace(*cur) && *cur != '\0') cur++;
				
				else { 

					if (j + 1 > size){
						size *= 2;
						tmp->argv = (char **) realloc(tmp->argv, size*sizeof(char **));
					}
					if (*cur == '\0')
					{
						end_f = 1;
					}
					else *cur = '\0';
					
					(tmp->argv)[j] = cur_param;
					j++;
					state = 0;
					cur++;
				}
				break;
		}
	}

	(tmp->argv)[j] = NULL;

	return tmp;
}

cmd_Tree *parse (char *command){

	//Tree *tmp = (Tree *) malloc(sizeof(Tree));
	cmd_Tree *result = NULL;//= tmp;

	char *pipe;
	char *cmd;
	char *bg = NULL;
	int bg_flag = 0, end_f = 0; int state;
	char *next, *and, *or;

	if ((bg = strchr(command, '&')) && *(bg+1) != '&') {
		bg_flag = 1;
		*bg = '\0';
	}

	cmd_Tree *start = NULL;
	while (!end_f){

		while ((pipe = strchr(command, '|')) &&	(*(pipe + 1) != '|'))
		{			
			*pipe = '\0';
			if (!start){
				start = result = add_to_tree(command);
			}

			else { 
				result->pipe = add_to_tree(command);
				result = result->pipe;
			 }

			result->background = bg_flag;
			command = pipe + 1;
		}

		if ((and = strchr(command, '&')) && (*(and + 1) == '&'))
		{ 
			*and = '\0';
			
			if (!start){
				start = result = add_to_tree(command);
			}

			else { 
				result->pipe = add_to_tree(command);
			 }
			command = and + 2;
		}
		else if (next = strchr(command,';')) 
			{
				*next = '\0';
				if (!start){
					start = result = add_to_tree(command);
				}

				else { 
					result->pipe = add_to_tree(command);
			 	}

				command = next +1; 
			}
		else if (((or = strchr(command, '|')) && (*(or + 1) == '|')))
			{
				*or = '\0';
				
				if (!start){
					start = result = add_to_tree(command);
				}

				else { 
					result->pipe = add_to_tree(command);
			 	}
				command = or + 2;
			}
		else {
				if (!start){
					start = result = add_to_tree(command);
				}

				else { 
					result->pipe = add_to_tree(command);
			 	}
				end_f = 1;
			}
	}
	return start;
}

Node* go_pipe( cmd_Tree* command, int pipe_num){

	cmd_Tree *next;
	Node *shell_pipe = NULL;
	int size = 2, i = 0;
	int *fd_pipe = malloc(2*sizeof(int *));
	pid_t pid;
	int fd_forward;

	while (command != NULL){

		if (i + 1 > size){
			size *=2;
			fd_pipe = (int *) realloc(fd_pipe, size*sizeof(int*));
		}



		if (command->argv[0] == NULL){
			printf("Пустая команда\n");
			return NULL;
		}

		if (pipe((fd_pipe + 2*i))) perror("pipe");



		if ( !(pid = fork()) ){


			if (i != 0){
				dup2(fd_pipe[2*(i-1)],0);
				close(fd_pipe[2*(i-1)]);
			}

			if (command->pipe){
				dup2(fd_pipe[2*i+1],1);
				close(fd_pipe[2*i+1]);
			}

			if (command->outfile){
				fd_forward = open(command->outfile, O_WRONLY | O_CREAT, 0666);
				dup2(fd_forward, 1);
				close(fd_forward);
			}

			execvp(command->argv[0], command->argv);
		//	perror(command->argv[0]);
			exit(1);
		}

		close(fd_pipe[2*(i-1)]);
		close(fd_pipe[2*i+1]);
		shell_pipe = add_to_list(pid, command->argv[0], shell_pipe);
		shell_pipe->pipe_num = pipe_num;
		pipes[pipe_num]++;  	
		next = command->pipe;
		free(command);
		command = next;
		i++;
	}
	//close(fd_pipe[2*i]);
	//perror("close");
	//close(fd_pipe[2*i + 1]);
	//perror("close");
//	pipes[pipe_num] = i;
//	pipe_num++;
	return shell_pipe;
}


int main(int argc, char const *argv[])
{
	pid_t pid_return = 0;
	int status, pipe_num = 0;
	char *command, *bg;
	int if_wait = 0;
	Node *node_return = NULL, *shell_pipe = NULL;
	
	while(!feof(stdin)){

				while ( (pid_return = waitpid(0, &status, WNOHANG)) != 0 && pid_return != -1){
					node_return = find_in_list(pid_return, shell_pipe);
					if (node_return)
					{
						node_return->status = status;
						pipes[node_return->pipe_num]--;
						if (!pipes[node_return->pipe_num])
							printf("Pipe %d with cmd %s exited with status %d\n", node_return->pipe_num, pipes_cmd[node_return->pipe_num], status);
					//		free(pipes_cmd[node_return->pipe_num]);
					}
				}
			if (if_wait) {
				while ( (pid_return = waitpid(0, &status, WNOHANG)) != -1){
					node_return = find_in_list(pid_return, shell_pipe);
					if (node_return){
						pipes[node_return->pipe_num]--; 
					//	printf("Process %d with cmd %s exited with status %d\n", node_return->pid, node_return->cmd, status);	
					}
				}
			}

			printf(">");

			command = getline_unlim();

			if ((bg = strchr(command, '&')) && *(bg+1) != '&') if_wait = 0;
			else if_wait = 1;

			if (*command != '\0'){			
				if  ((pipe_num = find_free()) == 20){
					printf("Много каналов. Подождите  завершения хотя бы одного\n");
				}

				else {
					pipes_cmd[pipe_num] = malloc(sizeof(char) * strlen(command));
					strcpy(pipes_cmd[pipe_num], command);
					cmd_Tree *command_tree = parse(command);
					shell_pipe = go_pipe(command_tree, pipe_num);
				}
			
			}
	}
	return 0;
}
