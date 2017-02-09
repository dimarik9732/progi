/* Отец пишет в канал по строке.
Сын читает, реверсирует, пишет в канал. 
Отец читает, выводит в файл.*/
#include <stdio.h> // fopen, getline, fwrite, fseek
#include <stdlib.h> // exit, getline, realloc
#include <unistd.h> // dup2, write, fork, exit, pipe
#include <sys/types.h> // fork, wait
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h> // wait
#include <string.h>
#define _GNU_SOURCE // getline

int main(int argc, char **argv)
{
	pid_t pid;
	int fd1[2], fd2[2]; // Каналы

	pipe(fd1);
	pipe(fd2);

	char *cur_line = NULL; // Текущая строка
	size_t size_cur_line = 0; // Ее размер
	ssize_t size_read; // Сколько считали

	if((pid = fork()) != 0)
	{ // Процесс Отец
		// Открыть файл
		FILE* f1, *f2;
		f1 = fopen(argv[1], "r");
		f2 = fopen("tmp.txt", "w");
		if(f1 == NULL) 
		{
			printf("Ошибка открытия файла\n"); 
			exit(1);
		}

		// Работа с файлом и каналом
		close(fd1[0]); // Закрыть канал на чтение
		close(fd2[1]); // Закрыть канал на запись

		// Читаем из файла строками пока он не закончился
		while((size_read = getline(&cur_line, &size_cur_line, f1)) != -1)
		{	
		//	fseek(f1, 0 - size_read, SEEK_CUR);
			static char *chr;
			if (chr = strchr(cur_line,'\n')) *chr = '\0';
			else size_read++;

			if(write(fd1[1], cur_line, size_read) != size_read) // Запись в канал
			{
				printf("Ошибка записи в канал\n");
				exit(1);
			}

			if(read(fd2[0], cur_line, size_read) != size_read) // Читаем из канала
/*			{	
				printf("Ошибка чтения из канала\n");
				exit(1);
			}
*/;
			if(fprintf(f2, "%s\n", cur_line))// Запись в файл
/*			{
				printf("Ошибка записи в файл\n");
				exit(1);
			}
*/;
		}

		fclose(f1);
		fclose(f2);
		f1 = fopen(argv[1], "w");
		f2 = fopen("tmp.txt", "r");

		int c;
		while((c = fgetc(f2)) != EOF){
			fputc(c, f1);
		}

		fclose(f1);
		fclose(f2);

		close(fd1[1]);
		close(fd2[0]);
		
		exit(0);
	}
	else
	{ // Процесс Сын
		close(fd1[1]);
		close(fd2[0]);

		int size_rev = 8;
		char *inp_line = (char *) malloc(size_rev*sizeof(char));
		int i = 0;

		while ( read(fd1[0], inp_line + i, 1) == 1 )
		{
			if(inp_line[i] == '\0')
			{
				write(fd2[1], inp_line, 1);
				continue;
			}

			i++;

			while( read(fd1[0], inp_line + i, 1) == 1 ) // Читаем из канала
			{	
				if (inp_line[i] == '\0') 
				{
					i = 0;
					break;
				}
				else 
				{
					if (i >= size_rev ) 
					{
							size_rev*=2;
							inp_line = realloc(inp_line, size_rev);
					}
					i++;

				}
			}

			size_read = strlen(inp_line);
			char *rev_line = (char*) malloc(sizeof(char)*(size_read+1));

			rev_line[size_read] = '\n'; // -1

			//rev_line[size_read - 1] = inp_line[size_read - 1];
			for(i = 0; i < size_read; i++)
				rev_line[i] = inp_line[size_read - i - 1];

			if(write(fd2[1], rev_line, size_read) != size_read) // Запись в канал
			{
				printf("Ошибка записи в канал\n");
				exit(1);
			}
			i = 0;
		}

		exit(0);
	}

	return 0;
}
