#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include "util.h"
#include <stdlib.h>

typedef struct pr_list{
	char **pr_arr;
	pid_t pid;
	struct pr_list *next;
}pr_list;

typedef struct pipe_list{
	char *line;
	struct pipe_list *next;
	pr_list *prs;
}pipe_list;

void printf_prs(char *array[]){
	int i;
	for (i = 0; array[i] != NULL /*&& array[i] != NULL*/; i++){
		printf("%s ", array[i]);
	}
}

void add_pr(char *pr, char **array[]){
	int i, j, len;
	/*	count "non-NULL" cells	*/
	for (j = 0; (*array)[j] != NULL; j++);
	j += 2;//j "non-NULL" + 1 "NULL" + 1 extra for a new argument
	(*array) = realloc((*array), j*sizeof(char*));
	/*	MOVE PR INTO CELL	*/
	for (i = 0; ((*array)[i]) != NULL && (i < j) ; i++);
	len = strlen(pr);
	free((*array)[i]);
	((*array)[i]) = calloc(len + 1, sizeof(char));
	for (j = 0; j < len; j++){
		((*array)[i][j]) = pr[j];
	}
	((*array)[i][j]) = '\0';
	/*	NEXT CELL = NULL	*/
	((*array)[i+1]) = NULL;
}

	/*	MAIN	*/

int main(int argc, char *argv[]){
	int i, j, k, len, word_begin, word_end;
	int status;
	pid_t pid;
	int not_empty, back_mode, pr_flag, error_flag;
	int exited_flag = 1, next_pr = 0;
	pipe_list *c_pipe, *pipes, *p_pipe;
	pr_list *c_pr, *p_pr, *pr_pointer;
	char *new_line, *cur_word;

	printf("Please input pipe\n");
	//c_pipe = calloc(1, sizeof(*pipe_list));
	pipes = malloc(sizeof(pipe_list));
	pipes->next = NULL;
	pipes->line = NULL;
	pipes->prs = NULL;

	while ((new_line = getline_unlim()) != NULL){
		if (!strcmp(new_line,"")) continue;
		/*	WAIT BACK PROCESSES	*/
		while ((pid = wait4(-1, &status, WNOHANG, NULL)) > 0){
			p_pipe = pipes->next;
			while (p_pipe){
				p_pr = p_pipe->prs;
				while (p_pr){
					if (pid == p_pr->pid){
						p_pr->pid = -1;
						printf("program \"%s\" has been completed with exit status: %d\n", p_pr->pr_arr[0], WEXITSTATUS(status));
						pr_pointer = p_pr;
						exited_flag = 1;
						while (pr_pointer){
							if (pr_pointer->pid != -1){
								exited_flag = 0;
								break;
							}
							pr_pointer = pr_pointer->next;
						}
						if (exited_flag){
							printf("pipe \"%s\" has been completed with exit status: %d\n", p_pipe->line, WEXITSTATUS(status));
						}
					}
					p_pr = p_pr->next;
				}
				p_pipe = p_pipe->next;
			}
		}
		
		/*	DELETE PRS WITH PID == -1	*/
		p_pipe = pipes->next;
		while (p_pipe){
			p_pr = p_pipe->prs;
			while (p_pr){
				if (p_pr->pid == -1){
					if (p_pr == p_pipe->prs){
						for (i = 0; p_pr->pr_arr[i] != NULL; i++){
							if (p_pr->pr_arr)
								free((p_pr->pr_arr[i]));
						}
						if (p_pr->pr_arr)
							free(p_pr->pr_arr);
						p_pipe->prs = p_pr->next;
						if (p_pr)
							free(p_pr);
						p_pr = p_pipe->prs;
					}else{
						c_pr = p_pipe->prs;
						while (c_pr->next != p_pr && c_pr->next != NULL){
							c_pr = c_pr->next;
						}
						c_pr->next = p_pr->next;
						for (i = 0; p_pr->pr_arr[i] != NULL; i++){
							if (p_pr->pr_arr[i])
								free((p_pr->pr_arr[i]));
						}
						if (p_pr->pr_arr)
							free(p_pr->pr_arr);
						p_pipe->prs = p_pr->next;
						if (p_pr)
							free(p_pr);
						p_pr = c_pr;
					}
				}else{
					p_pr = p_pr->next;
				}
			}
			p_pipe = p_pipe->next;
		}
		/*	DELETE PIPES WITHOUT PRS	*/
		/*p_pipe = pipes->next;
		while (p_pipe){
			if (p_pipe->prs == NULL){
				if (pipes->next == p_pipe){
					if (p_pipe->line)
						free(p_pipe->line);
					pipes->next = p_pipe->next;
					if (p_pipe)
						free(p_pipe);
					p_pipe = pipes->next;
				}else{
					c_pipe = pipes->next;
					while (c_pipe->next != p_pipe && c_pipe != NULL){
						c_pipe = c_pipe->next;
					}
					c_pipe->next = p_pipe->next;
					if (p_pipe->line)
						free(p_pipe->line);
					if (p_pipe)
						free(p_pipe);
					p_pipe = c_pipe;
				}
			}else{
				p_pipe = p_pipe->next;
			}
		}*/

		if( !strcmp(new_line, "exit") || !strcmp(new_line, "EXIT") ){
			printf("exit\n");
			break;
		}

		/* processing line, creating pipe */

		c_pipe = pipes;
		while (c_pipe){
			c_pipe = c_pipe->next;
		}
		/*	allocate memory for current pipe	*/
		c_pipe = calloc(1, sizeof(pipe_list));
		c_pipe->next = NULL;
		c_pipe->prs = NULL;

		len = strlen(new_line);
		/*	copy new_line to current_pipe->line	*/
		c_pipe->line = calloc(len + 1, sizeof(char));
		for (i = 0; i < len + 1; i++){
			c_pipe->line[i] = new_line[i];
		}

		/*	allocate memory for first pr in current pipe	*/
		c_pipe->prs = calloc(1, sizeof(pr_list));
		c_pipe->prs->pr_arr = calloc(1, sizeof(char*));
		c_pipe->prs->pr_arr[0] = NULL;
		c_pipe->prs->next = NULL;
		c_pr = c_pipe->prs;

		p_pipe = pipes;
		while (p_pipe->next){
			p_pipe = p_pipe->next;
		}
		p_pipe->next = c_pipe;

		word_begin = 0; word_end = 0; next_pr = 0; error_flag = 0;
		not_empty = 0, back_mode = 0, pr_flag = 1;
		for (i = 0; i < len; i++){
			if (pr_flag){
				c_pipe->prs = c_pr;
				pr_flag = 0;
				not_empty = 0;
			}
			if (next_pr){
				c_pr->next = calloc(1, sizeof(pr_list));
				c_pr = c_pr->next;
				c_pr->pr_arr = calloc(1, sizeof(char*));
				c_pr->next = NULL;
				next_pr = 0;
				not_empty = 0;
			}

			if ( isspace(new_line[i]) || new_line[i] == '|' || new_line[i] == '&' ){
				if (new_line[i] == '|'){
					if (!not_empty) error_flag = 1;
					next_pr = 1;
					not_empty = 0;
				}
				if (new_line[i] == '&'){
					back_mode = 1;
				}
				new_line[i] = '\0';
				word_end = i;

				k = word_end - word_begin;
				if (k != 0){
					cur_word = calloc(k, sizeof(char));
					for (j = 0; j < k; j++){
						cur_word[j] = new_line[word_begin + j];
					}
					word_begin = i + 1;
				}
				if (word_begin != word_end){
					if (strcmp(cur_word,"")){
						if (!not_empty) not_empty = 1;
						add_pr(cur_word, &(c_pr->pr_arr));
						free(cur_word);
					}
				}
			}
		}
		k = i - word_begin;
		if (k != 0){
			cur_word = calloc(k, sizeof(char));
			for (j = 0; j < k; j++){
				cur_word[j] = new_line[word_begin + j];
			}
		}
		if (word_begin != i){
			if (strcmp(cur_word,"")){
				if (!not_empty) not_empty = 1;
				add_pr(cur_word, &(c_pr->pr_arr));
				free(cur_word);
			}
		}

		/*	executing pipe	*/
			if (error_flag){
				printf("pipe was written incorrectly\n");

				if (c_pipe){
					if (c_pipe->line)
						free(c_pipe->line);
					p_pr = c_pipe->prs;
					while (p_pr){
						for (i = 0; p_pr->pr_arr[i] != NULL; i++){
							if(p_pr->pr_arr)
								free((p_pr->pr_arr[i]));
						}
						if (p_pr->pr_arr)
							free(p_pr->pr_arr);
						c_pipe->prs = p_pr->next;
						if (p_pr)
							free(p_pr);
						p_pr = c_pipe->prs;
					}
					c_pipe = NULL;
				}

			}else if (!back_mode){
		/*	STANDART MODE	*/
				int cnt = 0;
				p_pr = c_pipe->prs;
				while (p_pr){
					cnt++;
					if (p_pr->next == NULL) break;
					else (p_pr = p_pr->next);
				}
				p_pr = c_pipe->prs;
				int fd[cnt-1][2];
				for (i = 0; i < cnt - 1; i++){
					pipe(fd[i]);
				}
				for (i = 0; p_pr && i < cnt; i++){
					if (p_pr->pr_arr[0] != NULL){
						if ((p_pr->pid = fork()) < 0){
							perror("fork ");
							exit(1);
						}else if (p_pr->pid == 0){
							if (i < cnt - 1){
								dup2(fd[i][1], 1);
							}
							if (i > 0){
								dup2(fd[i-1][0], 0);
							}
							for (j = 0; j < cnt - 1; j++){
								close(fd[j][0]);
								close(fd[j][1]);
							}
							execvp(p_pr->pr_arr[0], p_pr->pr_arr);
							perror("exec ");
							_exit(1);
						}
						p_pr = p_pr->next;
						if (p_pr == NULL) break;
					}else break;
				}
				for (i = 0; i < cnt-1; i++){
					close(fd[i][0]);
					close(fd[i][1]);
				}
//////////////////
				c_pr = c_pipe->prs;
				for (i = 0; i < cnt; i++){
					pid = waitpid(c_pr->pid, &status, 0);
					if (WIFEXITED(status)){
						p_pr = c_pipe->prs;
						while (p_pr){
							if (p_pr->pid == pid){
								printf("program \"%s\" has been completed with exit status: %d\n", p_pr->pr_arr[0], WEXITSTATUS(status));
								if (c_pr->next == NULL){
									printf("pipe \"%s\" has been completed with exit status: %d\n", c_pipe->line, WEXITSTATUS(status));
								}
								break;
							}else{
								p_pr = p_pr->next;
							}if (p_pr == NULL){
								printf("there aren't process with pid == %d\n", (int)pid);
								break;
							}
						}
					}else{
						p_pr = c_pipe->prs;
						while (p_pr){
							if (p_pr->pid == pid){
								printf("program \"%s\" hasn't been completed\n", p_pr->pr_arr[0]);
								if (c_pr->next == NULL){
									printf("pipe \"%s\" hasn't been completed\n", p_pipe->line);
								}
								break;
							}else{
								p_pr = p_pr->next;
							}
							if (p_pr == NULL){
								printf("there aren't process with pid == %d\n", (int)pid);
								break;
							}
						}
					}
					c_pr = c_pr->next;
				}
///////////////////
				/*	DELETE USED MEMORY	*/
				
				if (c_pipe){
					if (c_pipe->line)
						free(c_pipe->line);
					p_pr = c_pipe->prs;
					while (p_pr){
						for (i = 0; p_pr->pr_arr[i] != NULL; i++){
							if (p_pr->pr_arr[i])
								free((p_pr->pr_arr[i]));
						}
						if (p_pr->pr_arr)
							free(p_pr->pr_arr);
						c_pipe->prs = p_pr->next;
						if (p_pr)
							free(p_pr);
						p_pr = c_pipe->prs;
					}
					c_pipe = NULL;
				}
///////////////////////////////
			}else{
			/*	BACKGROUND MODE	*/
				printf("backgroung mode\n");
				int cnt = 0;
				p_pr = c_pipe->prs;
				while (p_pr){
					cnt++;
					if (p_pr->next == NULL) break;
					else (p_pr = p_pr->next);
				}
				p_pr = c_pipe->prs;
				int fd[cnt-1][2];
				for (i = 0; i < cnt - 1; i++){
					pipe(fd[i]);
				}
				for (i = 0; p_pr && i < cnt; i++){
					//printf_prs(p_pr->pr_arr);
					if (p_pr->pr_arr[0] != NULL){
						if ((p_pr->pid = fork()) < 0){
							perror("fork ");
							exit(1);
						}else if (p_pr->pid == 0){
							if (i < cnt - 1){
								dup2(fd[i][1], 1);
							}
							if (i > 0){
								dup2(fd[i-1][0], 0);
							}
							for (j = 0; j < cnt - 1; j++){
								close(fd[j][0]);
								close(fd[j][1]);
							}
							execvp(p_pr->pr_arr[0], p_pr->pr_arr);
							perror("exec ");
							_exit(1);
						}
						p_pr = p_pr->next;
						if (p_pr == NULL) break;
					}else break;
				}
				for (i = 0; i < cnt-1; i++){
					close(fd[i][0]);
					close(fd[i][1]);
				}
			}
	printf("Please input pipe\n");
	}
	/*	WAIT BACK PROCESSES AND FREE ALL PIPES	*/
	p_pipe = pipes->next;
	while (p_pipe){
		p_pr = p_pipe->prs;
		while (p_pr){
			pid = waitpid(p_pr->pid, &status, 0);
			if (WIFEXITED(status)){
				while (p_pr){
					if (p_pr->pid == pid){
						printf("program \"%s\" has been completed with exit status: %d\n", p_pr->pr_arr[0], WEXITSTATUS(status));
						if (p_pr->next == NULL){
							printf("pipe \"%s\" has been completed with exit status: %d\n", p_pipe->line, WEXITSTATUS(status));
						}
						break;
					}else{
						p_pr = p_pr->next;
					}if (p_pr == NULL){
						printf("there aren't process with pid == %d\n", (int)pid);
						break;
					}
				}
			}else{
				p_pr = c_pipe->prs;
				while (p_pr){
					if (p_pr->pid == pid){
						printf("program \"%s\" hasn't been completed\n", p_pr->pr_arr[0]);
						if (p_pr->next == NULL){
							printf("pipe \"%s\" hasn't been completed\n", p_pipe->line);
						}
						break;
					}else{
						p_pr = p_pr->next;
					}
					if (p_pr == NULL){
						printf("there aren't process with pid == %d\n", (int)pid);
						break;
					}
				}
			}
			p_pr = p_pr->next;
		}
		p_pipe = p_pipe->next;
	}
	/*	DELETE ALL PIPES	*/
	p_pipe = pipes;
	while (p_pipe->next){
		if (p_pipe->next->line)
			free(p_pipe->next->line);
		if (p_pipe->next->prs){
			p_pr = p_pipe->prs;
			while (p_pr){
				for (i = 0; p_pr->pr_arr[i] != NULL; i++){
					if (p_pr->pr_arr[i])
						free((p_pr->pr_arr[i]));
				}
				if (p_pr->pr_arr)
					free(p_pr->pr_arr);
				p_pipe->next->prs = p_pr->next;
				if (p_pr)
					free(p_pr);
				p_pr = p_pipe->next->prs;
			}			
		}
		p_pipe = p_pipe->next;
	}
	if (pipes)
		free(pipes);
	return 0;
}
