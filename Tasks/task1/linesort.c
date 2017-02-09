#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
// Описание списка

typedef struct Node {
	
	char *data; //данные
	struct Node *next; //ссылка на следующий элемент
		
} Node;
// Конец описания

// Возвращение ссылки на  последний элемент списка
Node * get_last(Node *list){

		Node * tmp = list;

		if (tmp == NULL){return tmp;}
		else{
			while(tmp->next != NULL){
					tmp = tmp->next;
			}
			return tmp;
		}
}
// Конец функции

// Возвращение ссылки на n-ый элемент списка
Node * get_nth(Node *list, int n){
		Node * tmp = list;
		int i = 1;
		if(tmp == NULL){return tmp;}
		else{
			while (i < n){
				if(tmp->next == NULL){return tmp;}
				else { 
						tmp = tmp->next;
						i++;
				}
			}

			return tmp;
		}
}

// Возвращение нового звена списка списка элемента
Node * getNode(char *elem){
	

	// Создание элемента списка

	Node *tmp = (Node*)malloc(sizeof(Node));
	
	// Копирование значения elem в поле данных нового элемента списка

	tmp->data = elem;
	tmp->next = NULL;

	return tmp;
}

// Освобождение списка
int free_list(Node *list){
		
	if (list == NULL ) return 0;
	else{
		Node * cur = list;
		Node * next;

		while (cur->next != NULL){
				
			next = cur->next;
			free(cur->data);
			free(cur);
			cur = next;				
				
		}
		
		free(cur->data);
		free(cur);

		return 0;
	}
}
// Конец функции

// Чтение строки
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
		
		while(!flag_newline) {
				
			// Считывание block_size элементов из ввода
			result = fgets(line + strlen(line),block_size,stdin);
			if (!result){

				// если конец файла вернуть строку
				if (feof(stdin))
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
// Конец функции

// Печать всех элементов списка
int print_list(Node *list){
		
	Node * pr_lst = list;

	if(list != NULL)

		{
			printf("\nPrinted list\n");
			while (pr_lst -> next != NULL)
				{
					printf("%s\n",pr_lst->data);
					pr_lst = pr_lst->next;
				}
				
			printf("%s\n",pr_lst->data);
		}

	return 0;

}
// Конец функции

// Сортировка пузырьком
int bubble_sort(Node *list){
			
		int sort_flag = 1; // флаг сортировки(1-неотсортировано,0-отсортировано)
		
		Node * to_sort = list;
		
		if (to_sort == NULL){return 1;}

		while (sort_flag){
				
			sort_flag = 0;

			while (to_sort->next != NULL){
				
				if (strcmp(to_sort->data,to_sort->next->data)>0){
						
					char *tmp =	to_sort->data;
					to_sort->data = to_sort->next->data;
					to_sort->next->data = tmp;
					sort_flag = 1;

				}

				to_sort = to_sort->next;
			}
					
			to_sort = list;
		}

		return 0;

}
// Конец функции

// Сортировка слиянием
Node * merge_sort(Node *list,int count){
		
		// Условие выхода из рекурсии 
		if(count <= 2) 
		{
				
				if (count == 2)
				{
						
						if (strcmp(list->data, list->next->data) > 0) 
						{
								
								char * tmp = list->data;
								list->data = list->next->data;
								list->next->data = tmp;
						
						}
			
				}

				return list;
		
		}

		// Если больше 2-х элементов

		else {
			
			Node * part2 = get_nth(list, count/2);

			Node * part1 = list;
			
			Node * tmp2 = part2;

			part2 = part2->next;

			tmp2->next = NULL;

			part1 = merge_sort(part1, count/2);

			part2 = merge_sort(part2, count - count/2);
			
			int comp_flag; // Флаг сравнения
			
			Node * result = part1;
			Node * tmp;
			Node * tmp1 = part1;

			while(part1 != NULL && part2 != NULL){
					
					comp_flag = strcmp(part1->data,part2->data);

					if (comp_flag > 0) 
						{
							tmp = part1;
							part1 = part2;
							part2 = part1 ->next;
							part1->next = tmp;
							
							if (tmp1 == tmp){
									tmp1 = part1;
									result = tmp1;
							}
							else tmp1->next = part1;
						}
					else
						{
							if ( part1 == tmp1){
								part1 = part1->next;
							}
							else { 
									part1 = part1->next;
									tmp1 = tmp1->next;
							}
						}
		
			}
		
			if (part1 == NULL){

					tmp1->next  = part2;

			}

			return result;
		
	}
}
// Конец функции

// Основная программа
int main(int argc,char *argv[]){
			
			Node *head = NULL;

			int counter = 1;				

			Node *list = NULL;

			while(!feof(stdin)){
				
				if (list) {
					list->next = getNode(getline_unlim());
					list = list->next;
					counter++;
				}

				else {
					list = getNode(getline_unlim());
					head = list;
				}
				
			}

			int opt = getopt(argc,argv,"mb");		
		
			switch (opt)
			{
					case 'b':
						bubble_sort(head);
						break;

					case 'm':
						head = merge_sort(head,counter);
						break;
					default: 
						break;
			}
			
			print_list(head);
			free_list(head);
			return 0;
}
