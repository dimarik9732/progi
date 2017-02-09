// Сортировка файлов sort [-r +n –n –o] файлы
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Описание списка данных строк
typedef struct Node 
{
	int offset; // Смещение от начала файла
	int size; // Длина строки
	struct Node *next; // Ссылка на следующий элемент	

} Node;
// Конец описания

// Возвращение нового звена списка списка элемента
Node * getNode(int elem1, int elem2)
{
	// Создание элемента списка
	Node *tmp = (Node*)malloc(sizeof(Node));
	
	// Копирование значений elem1, elem2 в поля данных нового элемента списка
	tmp->offset = elem1; // (*tmp).offset
 	tmp->size = elem2;
	tmp->next = NULL;

	return tmp;

}
// Конец функции

// Освобождение списка
int free_list(Node *list)
{
	if (list == NULL ) return 0;
	else
	{
		Node *cur = list;
		Node *next;

		while (cur->next != NULL)
		{
			next = cur->next;
			free(cur);
			cur = next;				
				
		}
		
		free(cur);

		return 0;

	}
}
// Конец функции

// Возвращение ссылки на n-ый элемент списка
Node * get_nth(Node *list, int n)
{
		Node * tmp = list;
		int i = 1;
		if(tmp == NULL) return tmp;
		else
		{
			while (i < n)
			{
				if(tmp->next == NULL)return tmp;
				else 
				{ 
						tmp = tmp->next;
						i++;
				}
			}

			return tmp;
		}
}
// Конец функции

// Сортировка пузырьком
int bubble_sort(FILE *f1, Node *list, int r, int n){ 
// f1 - кого сорт, f3 - куда сорт, list - стр-ра строк(смещ, длина), r - порядок сортировки, n - числов сравнение
		
		char *s1, *s2; // Две строки для сравнения
		int sort_flag = 1; // Флаг сортировки(1 - не отсорт-но,0 - отсорт-но)
		
		Node * to_sort = list;
		
		if (to_sort == NULL) return 1;

		while (sort_flag)
		{
			sort_flag = 0;

			while (to_sort->next != NULL)
			{
				fseek(f1, to_sort->offset, SEEK_SET);
				s1 = (char *) malloc(sizeof(char) * (to_sort->size));
				fread(s1, sizeof(char), to_sort->size, f1);
				fseek(f1, to_sort->next->offset, SEEK_SET);
				s2 = (char *) malloc(sizeof(char) * (to_sort->next->size));
				fread(s2, sizeof(char), to_sort->next->size, f1);

				int x;
				
				if(n) 
				{
					// Числ сортировка
					int x1, x2;
					if(sscanf(s1, "%d", &x1) < 1) x1 = 0;
					if(sscanf(s2, "%d", &x2) < 1) x2 = 0;
					if(x1>x2) x = 1;
					else x = -1 ;
					if(x1 == 0 && x2 == 0) x = strcmp(s1, s2);
				}
				else 
				{
					// Строковая сортировка
					x = strcmp(s1, s2);
				}

				if (r ? x <= 0 : x > 0)
				{
					Node *tmp;
					tmp = (Node *) malloc(sizeof(Node));
					tmp->offset = to_sort->offset;
					tmp->size = to_sort->size;
					to_sort->offset = to_sort->next->offset;
					to_sort->size = to_sort->next->size;
					to_sort->next->offset = tmp->offset;
					to_sort->next->size = tmp->size;
					free(tmp);
					sort_flag = 1;
				}

				to_sort = to_sort->next;
				free(s1);
				free(s2);
			}
					
			to_sort = list;

		}
		return 0;
}
// Конец функции



/*
// Описание дополнительной структуры
typedef struct line
{
	char *s; // Строка
	int i; // Длина строки

} line;
// Конец описания 

// Чтение строки
line readline(FILE *f1){

	int c; // Текущий символ
	int size_s = 16; // Размер текущей строки
	
	line x;// Текущая строка
		x.s = malloc((size_s + 1)*sizeof(char));
		x.i = 0;
	
	c = fgetc(f1);
	while(c != EOF && c != '\n') 
	{
		// Пока файл или строка не закончились
		if(x.i >= size_s) 
		{ 
			size_s *= 2;
			x.s = (char *) realloc(x.s, (size_s + 1)*sizeof(char));
		}
			
		x.s[x.i] = c;
		x.i++; 
			
		c = fgetc(f1);			
	}

	return x;
}
// Конец функции

*/



int main(int argc, char **argv) 
{
		FILE *f1;
		char c;

// Считывание параметров
	int r = 0, n_plus = 1, n = 0, o = 0; // Параметры +n, -n, -o

	int size_input_files = 16;
	char **input_files = (char **) malloc(sizeof(char *)*size_input_files); // Входные файлы
	int count_input_files = 0; // Число входных файлов
	
	char *output_file; // Выходной файл
	FILE *f2 = tmpfile();

	int i;
	for(i = 1; i < argc; i++) 
	{
		switch(argv[i][0]) 
		{
			case '-':
			
				switch (argv[i][1]) 
				{
					case 'r':
						r++;					
						break;
					case 'n':
						n++;
						break;
					case 'o':
						o++;
						i++;
						output_file = (char *) malloc(sizeof(char)*strlen(argv[i])); 
						output_file = argv[i];
						break;
					default:
						break;
				}
				break;

			case '+':
				n_plus = atoi(argv[i]);
				break;

			default:

				if(count_input_files >= size_input_files) 
				{
					size_input_files *=2;
					input_files = (char **) realloc(input_files, sizeof(char *)*size_input_files);
				}

				input_files[count_input_files] = argv[i];
				count_input_files++;
				break;

		}
	}
// Конец считывания параметров

// Цикл по входным файлам
	int j;
	for(j = 0; j < count_input_files; j++) 
	{
		f1 = fopen(input_files[j], "r");
		if(f1 == NULL) 
		{
			printf("Ошибка открытия файла\n"); 
			return 1;
		}

	// Чтение файла, заполнение списка смещение + длина
		Node *head = NULL;
		Node *list = NULL;
		int set = 0; // Номер текущей позиции в файле

		while(!feof(f1))
		{
		// Находим длину строки
			int i = 0; // Длина текущей строки
			while((c = fgetc(f1)) != EOF && c != '\n') 
			{// Пока строка не закончились
				i++;			
			}
		// Нашли длину строки

		// Добавляем новый эл-т в список смещений + длина
			if(list) 
			{
				list->next = getNode(set, i);
				list = list->next;
			}
			else 
			{
				list = getNode(set, i);
				head = list;
			}			
		
			set += i + 1;
		}
	// Заполнили список смещение + длина

		fseek(f1, 0, SEEK_SET);
	// Конец чтения файла

		// Сортировка с n-ой строки
		if(n_plus != 1) 
		{
			list = head;
			Node *tmp = get_nth(head, n_plus - 1);
			head = tmp->next;
			tmp->next = NULL;

			bubble_sort(f1, head, r, n);

			tmp = get_nth(list, n_plus - 1);
			tmp->next = head;
			head = list;
		}

		else 
		{	
			bubble_sort(f1, head, r, n);
		}
		// Конец сортировки

		// Печать
		if(o == 0) f2 = stdout; // В стандартный поток вывода
		
		Node *to_write = head;
		while(to_write)
		{
			char *s1;
			fseek(f1, to_write->offset, SEEK_SET);
			s1 = (char *) malloc(sizeof(char) * (to_write->size));
			fread(s1, sizeof(char), to_write->size, f1);
			fwrite(s1, sizeof(char), to_write->size, f2);
			if (to_write->next) fputc('\n', f2);
			to_write = to_write->next;
			free(s1);
		}
		// Конец печати
		
		fclose(f1);
		free_list(head);

	}	
// Конец цикла по входным файлам

	// Печать в выходной файл
	if(o != 0) 
	{
		f1 = fopen(output_file, "w+");
		if(f1 == NULL) 
		{
			printf("Ошибка записи в файл\n"); 
			return 1;
		}
		fseek(f2, 0, SEEK_SET);
		while ((c = getc(f2)) != EOF) putc(c,f1);

		fclose(f1);
		fclose(f2);
	}
	// Конец печати в выходной файл

	return 0;
}
