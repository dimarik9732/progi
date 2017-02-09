// Вывод всех максимальных пересечений двух строк
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Ф-я поблокового ввода строки 
char *input(void){
	const int starting_size = 8*sizeof(char), size_of_block = 4*sizeof(char);
	char *str = (char *) malloc(starting_size + sizeof(char));
	char *ptr;
	int cur_size = starting_size;
	int i = 0;
    char *test;
    printf("Введите строку\n");
    do{
    	if(i == cur_size){
           	cur_size <<= 1;
           	str = (char *) realloc(str, cur_size + sizeof(char));
        }
        ptr = fgets(str + i, size_of_block + sizeof(char), stdin);

        test = strchr(str + i,'\n');
        if(test != NULL){
            *test = '\0';
            break;}
        i += size_of_block;
    } 
    while(ptr);
    stdin;
    return str;
}

// Ф-я вывода всех максимальных подстрок пересечений
int cross(char *s1, char *s2) {

	// Если пересечений нет
	if(strpbrk(s1, s2) == NULL) return 0;

	// В s1 меньшая строка
	int m = strlen(s1); // Длина s1
	int n = strlen(s2); // Длина s2
	if(m >= n) {
		char *temp = s2;
		s2 = s1;
		s1 = temp;
		m +=n;
		n = m - n;
		m -=n;
	}

	int mas[m][n];
	int i, j;
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			if(s1[i] == s2[j]) mas[i][j] = 1;
			else mas[i][j] = 0;
//			printf("%d ", mas[i][j]);
		}
//		printf("\n");
	}

	int k = n - 1;
	int l = 0;
	int flag = 0; // Флаг новая подстрока или продолжение старой

	char * strmas[n]; // Массив указателей на первые эл-ты подстрок пересечений
	int str_max = 0; // Максимальная длина подстроки пересечения
	int str_count = 0; // Кол-во подстрок макс длины

	char *cur_str;
	int cur_max = 0; // Длина текущей подстроки

	// Поиск макс подстрок и заполнение массива strmas
	// Чтение матрицы пересечений по диагоналям снизу вверх
	while(l != n) {
		i = k;
		j = l;
		
		while ( i!=n && j!=m ){
			if(mas[i][j] == 1) {
				if(flag == 0) {
					cur_str = (s2 + j); // Если новая подстрока
					flag = 1;
				}
				cur_max++;
			}
			if (mas[i][j] == 0 || i == n-1 || j == m-1) {
				if(cur_max > str_max) { 
					// Нашли подстроку большей длины
					str_max = cur_max;
					str_count = 0;
					strmas[str_count] = cur_str;
					str_count++;
				}
				else {
					if (cur_max == str_max){
						// Нашли еще одну подстроку длины str_max
						strmas[str_count] = cur_str;
						str_count++;
					}
				}

				cur_max = 0;
				flag = 0;
			}

			i++;
			j++;
		}

		cur_max = 0;
		flag = 0;
		
		if(k != 0) k--;
		else l++;
	}

	// Вывод макс подстрок, опираясь на массив strmas
	for(i = 0; i < str_count; i++) {
		for(j = 0; j < str_max; j++) {
			printf("%c", strmas[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int main(void){
	char *s1 = input();
	char *s2 = input();
	cross(s1,s2);
	return 0;
}