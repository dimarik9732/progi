#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <unistd.h>


int main(int argc, char **argv)
{
key_t key;
char *shmaddr;
key = ftok("/tmp/ter", 'S');
if(key < 0) 
	printf("ERR1\n");
int shmid = shmget(key, 100, 0666 | IPC_CREAT | IPC_EXCL);
if(shmid == -1)
	printf("ERR2\n");
shmaddr = shmat(shmid, NULL, 0);
/* работаем с разделяемой памятью, как с обычной */
/*
putm(shmaddr);
waitprocess();
*/
shmctl(shmid, IPC_RMID, NULL);
return 0;
}