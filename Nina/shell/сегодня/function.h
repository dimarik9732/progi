#ifndef _FUNCTION_H_
#define _FUNCTION_H_

typedef struct string_list
{
	char *word;
	struct string_list *next;

}string_list;

// Ф-я, разбивающая строку на список лексем
string_list *parse();
// Ф-я, печатающая и затем удаляющая список
int print_list(string_list *list);



#endif