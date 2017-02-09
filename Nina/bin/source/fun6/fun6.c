#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "fun.h"

// Добавление одночлена в многочлен
Node *new(Node *P, int a, int n) {
	Node *elem = (Node *) malloc(sizeof(Node));
	Node *temp = P;

	if(a == 0) return P;
	elem->ai = a;
	elem->degree = n;
	elem->next = NULL;

	if(temp == NULL) 
		return elem;
	else {
		while(temp->next != NULL) 
			temp = temp->next;
		temp->next = elem;
	}

	return P;
}

// Печать многочлена
int print(Node *P) {
	Node *temp = P;
	
	printf("P = ");
	if(temp == NULL) {printf("0\n"); return 0;}

	while(temp != NULL) {
		printf("%d*x^%d", temp->ai, temp->degree); // Вывод значения элемента p
		temp = temp->next; // Переход к следующему узлу
		if(temp != NULL) printf(" + ");
		else printf("\n");
	}
	return 0;
}

// Создание многочлена
Node *input(void) {
	printf("Введите многочлен a0*x^0 + a1*x^1 + a2*x^2 + ... + an*x^n (напр: 5*x^3 + 8*x^4)\n");
	
	Node *temp = NULL;
	int a, n; // Коэф-т, степень

	char c; // Промежуточный символ
	int size_s = 2; // Размер строки-числа
	char *s = (char *) malloc((size_s + 1)*sizeof(char)); // Строка-число
	s[0] = '\0';
	int flag1 = 0; // Флаг число(>0)/не число
	int i = 0; // Счетчик цифр в строке-числе
	int flag2 = 0; // Флаг коэф-т(0)/степень(1)
	int flag3 = 1; // Флаг отрицательный коэф-т(-1)

	do {

		c = getchar();

		if(isdigit(c)) {
			if( (i + 1) == size_s) {size_s *= 2; s = (char *) realloc(s, size_s*sizeof(char) + 1);}	
			s[i] = c;
			s[i+1] = '\0';
			flag1++;
			i++;
		}
		else {
			if(c == '-') flag3 = -1;
			if(flag1 >= 1) {
				if(flag2 == 0) { 
					a = flag3 * atoi(s);
					flag2++;
					flag3 = 1;
				}
				else {
					n = atoi(s);
					flag2--;
					temp = new(temp, a, n);
				}
				// Подготовка к новой строке-числу
				s[0] = '\0';
				i = 0;
				flag1 = 0;
			}
		}
		
	} while (c != '\n');
	return temp;
}

// Удаление многочлена
Node *delete(Node *P) {
	Node *temp = P;

	while(P != NULL) {
		P = P->next;
		free(temp);
		if(temp->next != NULL) temp = P;
	}
	return P;
}
// Поиск одночлена
Node *search(Node *P, int n) {
	Node *temp = P;
	while(temp != NULL) {
		if(temp->degree == n) 
			return temp;
		temp = temp->next;
	}
	return NULL;
}


// Вычитание одночлена
Node *sub(Node *P, Node *Q) {
	Node *elem = search(P, Q->degree);
	Node *temp = P;
	elem->ai -= Q->ai;
	if(elem->ai == 0) {
		if(temp->ai == 0) {
			P = P->next;
			free(temp);
		}
		else {
			if(temp->next->ai != 0) 
				temp = temp->next;
			elem = temp->next;
			temp->next = elem->next;
			free(elem);
		}
	}
	return P;
}

