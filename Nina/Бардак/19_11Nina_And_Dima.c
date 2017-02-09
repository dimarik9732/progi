#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h> // Для getwd
#include <string.h>

#define TRUE 1

typedef struct Node
{
	char * name;
//	char * path;
	struct Node * next;
} Node;

int a = 0; // Параметр -a (вывод со скрыт файлами)
int l = 0; // Параметр -l (вывод подробной инф-ии)
int R = 0; // Параметр -R (рекурсивный обход подкаталогов)

const int buf_size = 256;

// Функция добавления элемента в очередь

void add_elem(Node **first, Node **last, char * elem)
{

	Node *tmp = malloc(sizeof(Node));
	tmp->name = elem;
	tmp->next = NULL;

	if (*last == NULL)
	{
		*first = tmp;
		*last = tmp;
	}
	else
	{
		(*last)->next = tmp;
		*last = (*last)->next;
	}
	return;
}

// Конец функции

// Функция очистки очереди
void rm_elem(Node **first, Node **last)
{
	if(*first == *last)
	{
		free(*first);
		*first = *last = NULL;
	}

	else
	{
		Node * next_first;
		next_first = (*first)->next;
		free(*first);
		*first = next_first; 
	}

	return;
}
// Конец функции

// Ф-я возвращающая тип файла
int get_type_file(int st_mode)
{
	switch ( st_mode & S_IFMT )
    {
        case S_IFDIR: return 'd';
        case S_IFCHR: return 'c';
        case S_IFBLK: return 'b';
        case S_IFREG: return '-';
        case S_IFLNK: return 'l';
        case S_IFSOCK: return 's';
        case S_IFIFO: return 'p';

    	default: return 0;
    }
}
// Конец ф-ии

// Ф-я выдающая инф-ю о файле
int print_inf_file(struct stat *buf, char* path_name)
{
	if(!l)
	{
		printf("%s ", path_name);
		return 0;
	}

	char x[10] = "rwxrwxrwx\0";

	int i;
	int j;
	// Отсутствие права доступа
	for(i = 0, j = (1 << 8); i < 9; i++, j >>= 1)
		if((buf->st_mode & j) == 0) x[i] = '-';

	if (buf->st_mode & S_ISUID) x[2]= 's'; // Установка идентификатора пользователя при выполнении (изменяется eUID процесса)
  	if (buf->st_mode & S_ISGID) x[5]= 's'; // Установка идентификатора группы при выполнении (изменяется eGID процесса)
  	if (buf->st_mode & S_ISVTX) x[8]= 't'; // Для обычного файла - сохранить текст программы (машинный код) в ОП после завершения процесса

  	struct passwd *passwd_file = malloc(sizeof(struct passwd));
  	passwd_file = getpwuid (buf->st_uid); // Преобразование идентификатора пользователя в его имя

  	// Печать тип файла, права, кол-во жест ссылок, имя польз-ля, р-р, имя файла
  	printf("%c%s %3ld %s %8ld %s\n", 
  		get_type_file(buf->st_mode), 
  		x, 
  		buf->st_nlink, 
  		passwd_file->pw_name, 
	  	buf->st_size, path_name);
	
	return 0;
}
// Конец ф-ии

// Печать информации о файле и занесение директорий в очередь
void list_files(char * name, char *path, Node **first, Node **last, int Rec)
{
	char *full_name;
	if (*name){
		full_name = malloc(sizeof(char)*buf_size);
		sprintf(full_name, "%s/%s", path, name);
	}
	else full_name = path;

	struct stat *buf = malloc(sizeof(struct stat));

	if(lstat(full_name, buf) == -1)
	{
		printf("Нет такого файла или каталога\n");
		return;
	}

	if(*name != '.' || a)
	{
		if (Rec && get_type_file(buf->st_mode) == 'd')
		{	
			if( strcmp(name, ".") && strcmp(name, "..") )
				add_elem(first, last, full_name);
		}
		else print_inf_file(buf, name);
	}
	return;
}
// Конец функции

// Печать информации о файлах из директории и занесение директорий в очередь
void print_dir(char *name, Node **first, Node **last)
{

	printf("%s:\n", name);
	struct dirent *dp;
	DIR *dfd;
	char* name1 = name;
	if( !(dfd = opendir(name)) )
	{
		printf("Ошибка открытия каталога\n");
		return;
	}
		
	while(dp = readdir(dfd))
		list_files(dp->d_name, name, first, last, R);

	printf("\n\n");
	closedir(dfd);
}

int main(int argc, char **argv) 
{
    char **input_files; // Входные файлы
	input_files = NULL;	
	int count_input_files = 0; // Число входных файлов
	char *cur_path;
	static const char *opt_string = "alR";

	int opt = 0;
    opt = getopt( argc, argv, opt_string );

// Считывание параметров
    while(opt != -1) 
    {
        switch(opt) 
        {
            case 'a':
                a++;
                break;
            case 'l':
                l++;
                break;
            case 'R':
                R++;
                break;
            default:
				break;
        }
        
        opt = getopt(argc, argv, opt_string);
    }
    
    input_files = argv + optind;
    count_input_files = argc - optind;
// Конец считывания параметров

// Задание изначального пути
    cur_path = (char*) malloc(sizeof(char)*buf_size);
    getcwd(cur_path, buf_size);

// Случай по умолчанию
if(!count_input_files) 
    	{
			input_files[0] = (char*) malloc(sizeof(char));
    		input_files[0][0] = '\0';
    		count_input_files++;
    	}

// Цикл по входным данным
    	int i;
    	Node *first = NULL, *last = NULL;
    	for(i = 0; i < count_input_files; i++) 
   			list_files(input_files[i], cur_path, &first, &last , TRUE);
// Конец цикла по входным данным

// Цикл по очереди папок
   		do {
   			if (first){
   				print_dir(first->name, &first, &last);
   				rm_elem(&first, &last);
   			}

   		} while (first != NULL);
// Конец цикла

	return 0;
}