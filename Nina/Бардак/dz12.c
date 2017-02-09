/* Реализовать команду UNIX 
cp [-ip] исходный_файл целевой_файл (копирование файла) */
#include <stdio.h>
#include <fcntl.h>
#include <sys/file.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

char * readlink_malloc (char *filename) {
        int size = 100;
        while (1){
            char *buffer = (char *) malloc (size * sizeof(char));
            int nchars= readlink(filename,buffer,size);
            if (nchars < size)
            return buffer;
            free (buffer);
            size *= 2;
        }
}

int main(int argc, char **argv) {

// Считывание параметров
	int i = 0; // Параметр -i
    int p = 0; // Параметр -p
    char **input_files; // Входные файлы
	input_files = NULL;	
	int count_input_files = 0; // Число входных файлов
	
	static const char *opt_string = "ip";

	int opt = 0;
    opt = getopt( argc, argv, opt_string);

    while( opt != -1 ) {
        switch( opt ) {
            case 'i':
                i++;
                break;
            case 'p':
                p++;
                break;
            default:
				break;
        }
        
        opt = getopt(argc, argv, opt_string);
    }
    
    input_files = argv + optind;
    count_input_files = argc - optind;
// Конец считывания параметров

    if(count_input_files != 2) {printf("Ошибка"); return 1;}

	struct stat buf1, buf2;
	lstat(input_files[0], &buf1);
	lstat(input_files[1], &buf2);
   
    if((buf1.st_mode & S_IFMT) == S_IFLNK){
        input_files[0] = readlink_malloc(input_files[0]);
        printf("%s\n", input_files[0]);
    }

	if(buf1.st_ino == buf2.st_ino && buf1.st_dev == buf2.st_dev) {
		printf("Ошибка: копирование в себя\n"); return 1;}

	int fd1 = open(input_files[0], O_RDONLY, 0);
    if(fd1 == -1) {printf("Ошибка чтения файла\n"); return 1;}
 
	int fd2 = open(input_files[1], O_EXCL | O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);
    if(fd2 == -1 && i == 1) {
    		printf("Заменить существующий файл? (Y/n)\n");
    		if(getchar() == 'n') return 0;
            fd2 = open(input_files[1], O_WRONLY, 0);
            if (fd2 == -1){
                {printf("Ошибка записи файла\n");}
            }
    }

    if(p == 1) {
    	chmod(input_files[1], buf1.st_mode);
        chown(input_files[1], buf1.st_uid, buf1.st_gid);
    }

	char buffer[512]; 
	int n;
    while((n = read(fd1, buffer, sizeof(buffer))) > 0) {
    	printf("%d\n", n);
               write(fd2, buffer, n);

}

	close(fd1);
	close(fd2);

	return 0;
}
