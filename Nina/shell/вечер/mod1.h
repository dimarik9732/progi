#ifndef _MOD_1_H_
#define _MOD_1_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

typedef struct string_list
{
	char *word;
	struct string_list *next;

}string_list;

typedef struct cmd_inf 
{
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile_w;; // переназначенный файл стандартного вывода
	char *outfile_rw;
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
	struct cmd_inf *psubcmd; // команды для запуска в дочернем shell
	struct cmd_inf *pipe; // следующая команда после “|”
	struct cmd_inf *next; // следующая после “;” (или после “&”)
	struct cmd_inf *ornext; // ||
	struct cmd_inf *andnext; // &&
}cmd_inf;

// Ф-я, разбивающая строку на список лексем
string_list *parse();
// Ф-я, печатающая и затем удаляющая список
int print_list(string_list *list);

cmd_inf *parse2(cmd_inf *tree, string_list *list);

void free_tree(cmd_inf *tree);


#endif