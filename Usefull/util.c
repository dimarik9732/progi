#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include "util.h"

// Чтение строки
char*  getline_unlim(){

		enum {
			FGETS_BUFSIZE=300;
		}

		int flag_newline = 0;	// флаг конца строки
		
		unsigned long block_size = FGETS_BUFSIZE;	// размер блока считывания
		
		// Создание пустой строки

		char * line = (char *) malloc(block_size*sizeof(char));
		
		// Считывание строки

		char * character; // указатель на элемент строки
		char * result;
		
		while(!flag_newline && !feof(stdin)){
				
			// Считывание block_size элементов из ввода
			result = fgets(line + strlen(line),block_size,stdin);
			if (!result){
				if (!feof(stdin)) 
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

				else {				

						// Выделение дополнительной памяти
					
						block_size *= 2;

						line = realloc(line,strlen(line)+block_size);
					}
			}
		
		}

			return line;
}
// Конец функции