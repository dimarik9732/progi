// Удаление строк, длиной меньше n, в файле 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Node {
	char* line;
	struct Node* next; 
} Node;

// Ввод строки из файла
char*  fgetline_unlim(FILE * input){

		const int FGETS_BUFSIZE = 15;

		int flag_newline = 0;	// флаг конца строки
		
		unsigned long block_size = FGETS_BUFSIZE;	// размер блока считывания
		
		// Создание пустой строки

		char * line = (char *) malloc(block_size*sizeof(char));
		
		// Считывание строки

		char * character; // указатель на элемент строки
		char * result;
		
		while(!flag_newline) {
				
			// Считывание block_size элементов из ввода
			result = fgets(line + strlen(line),block_size, input);
			if (!result){

				// если конец файла вернуть строку
				if (feof(input))
					{
						return line;
					}

				// иначе - ошибка
				else 
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

				else 
					{				
						// Выделение дополнительной памяти
						block_size *= 2;
						line = realloc(line,strlen(line)+block_size);
					}
			}
		
		}

			return line;
}

// Вставка в список
Node * push(Node * list, char* line){

	Node *tmp = (Node*) malloc(sizeof(Node));
	tmp->next = list;
	tmp->line = line;
	return tmp;

}

// Печать списка в файл
int fprint_list(FILE *output, Node* list){

	if(list) {
		fprintf(output, "%s\n", list->line);
		fprint_list(output, list->next);
	}
	else return 0;		
}

int main(int argc, char const *argv[])
{
	int n = atoi(argv[2]);

	FILE *f1, *f2;
	f1 = fopen(argv[1], "r");
	if(f1 == NULL) {printf("Ошибка открытия файла\n"); return 1;}

	Node * list = NULL;

	int c; // Текущий символ
	char *s = fgetline_unlim(f1); // Текущая строка

	while(feof(f1) == 0) {
	// Пока файл не закончился

		if ( strlen(s) <= n ){
			list = push(list, s); // Если строка нужной длины, заносим в список
		}

		s = fgetline_unlim(f1); // Считываем следующую
				
	}

	// Закрыть, затем открыть в режиме записи
	fclose(f1);

	f2 = fopen(argv[1], "w");

	if(f2 == NULL) {printf("Ошибка открытия файла\n"); return 1;}
	
	fprint_list(f2, list);
	
	fclose(f2);

	return 0;
}