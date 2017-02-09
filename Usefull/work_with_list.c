#include <stdlib.h>
#include "work_with_list.h"

// Функция возвращает указатель на последний элемент списка
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

// Функция возвращает ссылку на n-ый элемент списка
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

// Функция создает новое звена списка и возвращает на него ссылку
Node * getNode(char *elem){
	
	// Создание элемента списка

	Node *tmp = (Node*)malloc(sizeof(Node));
	
	// Копирование значения elem в поле данных нового элемента списка

	tmp->data = elem;
	tmp->next = NULL;

	return tmp;
}


// Функция освобождает списка
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

// Функция печатает все элементы списка
int print_list(Node *list){
		
		Node * pr_lst = list;
		if(list == NULL){
				return 0;
		}

		else {
				printf("\n Printed list\n");
				while (pr_lst -> next != NULL){
						
						if (pr_lst->data != NULL){
							printf("%s\n",pr_lst->data);
						}
						pr_lst = pr_lst->next;
				}

				if (pr_lst->data != NULL){
					printf("%s\n",pr_lst->data);
				}

				return 0;
		}

}
// Конец функции