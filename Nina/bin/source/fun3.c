#include <stdio.h>
#include <string.h>
#include <malloc.h>

// Описание компоненты связности
typedef struct Node {
		int chislo;
		struct Node *next;
	} Node;

// Инициализация списка

Node * init(int a) { // а- значение первого узла
  Node *lst;
  // выделение памяти под корень списка
  lst = (Node*)malloc(sizeof(Node));
  lst->chislo = a;
  lst->next = NULL; // указатель на следующий узел
  return(lst);
}

// Добавление узла
Node *addelem(Node *lst, int number) {
  
  Node *elem, *temp;
  elem = (Node*)malloc(sizeof(Node));
  elem -> chislo = number;
  elem -> next = lst;

  return elem;
}

int bin_search_in_mas(Node * mas, int v_count, int b){

	int right = 0,left = v_count;
	int center = (right+left)/2;

	if (mas[right].chislo == b) return right;
	if (mas[left].chislo == b) return left;

	while (right != center){
		if (mas[center].chislo == b) return center;
		if (mas[center].chislo < b) {
			right = center;
			center = (right + left)/2;
			}
		if (mas[center].chislo > b) {
			left = center;
			center = (right + left)/2;
		} 
	}

	return -1;
}

void shift_mas_1 (Node * mas, int v_count, int from){

	int i;

	for (i = v_count; i >= from; i-- ){
		mas[i+1].chislo = mas[i].chislo;
		mas[i+1].next = mas[i].next;
	}

	return;
}

Node * bin_add_in_mas(Node * mas, Node * elem, int v_count){

	int right = 0, left = v_count,center = (right+left)/2;

	while (right != center){
		if (mas[center].chislo == (elem->chislo) ) return mas;
		if (mas[center].chislo < (elem->chislo)) {
			right = center;
			center = (right + left)/2;
			}
		if (mas[center].chislo > (elem->chislo)) {
			left = center;
			center = (right + left)/2;
		} 
	}

	shift_mas_1(mas, v_count, right + 1);
	mas[right].chislo = elem->chislo;
	mas[right].next = elem->next;

	return mas; 
}

Node * resize (Node * mas, int v_count, int * size_mas){

	if (v_count == *size_mas - 1){
		*size_mas *= 2;
		Node *new_mas = (Node *)realloc(mas, *size_mas);
		return new_mas;
	} 
	else return mas;
}

// Создание/Добавление
int create(Node* mas, int *v_count, int * size_mas, int a, int b) {
	int exist_a = bin_search_in_mas (mas, *v_count, a);
	int exist_b = bin_search_in_mas (mas, *v_count, b);

	if (exist_a != -1 && exist_b == -1){
		mas[exist_a].next = addelem(mas[exist_a].next, b);
		mas = resize(mas, *v_count, size_mas);
		mas = bin_add_in_mas(mas, init(b), *v_count);
		(*v_count)++;
		return 0;
	} else  if (exist_a == -1 && exist_b != -1){
				mas[exist_b].next = addelem(mas[exist_b].next, a);
				mas = resize(mas, *v_count, size_mas);
				mas = bin_add_in_mas(mas, init(a), *v_count);
				(*v_count)++;
				return 0;	
			} else if (exist_a == -1 && exist_b == -1){
				
				mas = resize(mas, *v_count, size_mas);
				mas = bin_add_in_mas(mas, init(a), *v_count);

				(*v_count)++;

				mas = resize(mas, *v_count, size_mas);
				mas = bin_add_in_mas(mas, init(b), *v_count);

				(*v_count)++;
				return 0;

			}






}


// Основная программа
int main(void) {
	
	int size_mas = 2;	
	Node *mas = (Node*)malloc(size_mas*sizeof(Node));

	int a,b;
	char c;
	int f;
	int v_count = 0;

	while((f = scanf("%d%c%d", &a,&c,&b)) != -1){
		if (f == 3)
		create(mas,&v_count, &size_mas, a, b);

	} 

	return 0;
}