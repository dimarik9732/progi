#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

// Описание дерева

typedef struct Node {
	
	char *line; // строка
	struct Node *right;// ссылка на правое поддерево
	struct Node *left;// ссылка на левое поддерево	
} Node;
// Конец описания

// Возвращение нового узла
Node * get_node(char *elem){
	

	// Создание элемента списка

	Node *tmp = (Node*)malloc(sizeof(Node));
	
	// Копирование значения elem в поле данных нового элемента списка

	tmp->line = (char *)malloc((strlen(elem)+1)*sizeof(char));
	memcpy(tmp->line,elem,strlen(elem)+1); 

	
	tmp->right = NULL;
	tmp->left = NULL;

	return tmp;
}
// Конец функции

// Вставка нового звена

Node * insert_node(Node * root, char * elem){

		if (root == NULL) {
				return get_node(elem);
		}
		
		if (strcmp(root->line, elem) < 0) 
		{
			root->left = insert_node(root->left, elem);
			return root;
		}

		else 
			{ 
				root->right = insert_node(root->right, elem);
				return root;
			}
}
// Конец функции 

// Печать дерева

int print_tree(Node * root){
		if (root == NULL) return 0;
		else {
				print_tree(root->right);
				if (root->line != NULL){
					printf("%s\n", root->line);
				}
				print_tree(root->left);
				return 0;
		}
}
			
// Чтение строки
char*  getline_unlim(int * end_flag){
		enum {
			FGETS_BUFSIZE=300
		};
		int flag_newline = 0;	// флаг конца строки
		
		unsigned long block_size = FGETS_BUFSIZE;	// размер блока считывания
		
		// Создание пустой строки

		char * line = (char *) malloc(block_size*sizeof(char));
		*line = '\0';

		// Считывание строки

		char * character; // указатель на элемент строки
		unsigned long oldsize; // длина строки до чтения нового блока
		char * result;
		
		while(!flag_newline && !*end_flag){
				
			// Считывание block_size элементов из ввода
			result = fgets(line + strlen(line),block_size,stdin);
			if (!result){
				if(feof(stdin)){*end_flag=1;}
				else { fprintf(stderr, "%s", strerror(errno)); exit(EXIT_FAILURE);}
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
			
					oldsize = strlen(line);
					// Выделение дополнительной памяти
					
					block_size *= 2;

					line = realloc(line,oldsize+block_size);
					}
			}
		
		}

			return line;
}
// Конец функции

// Основная программа
int main(void){
			
			Node *root = NULL;
			
			int end_flag = 0;

			while(!end_flag){
				root = insert_node(root,(getline_unlim(&end_flag)));
			}
			
			print_tree(root);
			return 0;
}
