#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>

#define FTOK_PATH "reader.c"
#define FTOK_PROJ 10
#define SIZE_BLOCK 400

key_t ipc_key = 0;

int shm_id = -1;
int sem_id = -1;
int msgq_id = -1;

void *mem_ptr = (void *)-1;

struct msgbuf{
	long mtype;
	char mtext[1];
};

void cleanup(void){

	if (mem_ptr != (void*)-1) shmdt (mem_ptr);
	if (sem_id >= 0) semctl (sem_id, 0, IPC_RMID);
	if (shm_id >= 0) shmctl (shm_id, IPC_RMID, 0);
	if (msgq_id >= 0) msgctl (msgq_id, IPC_RMID, 0);	
	return;
}

void pexit(char const *str){
	perror(str);
	cleanup();
	exit(errno);
}

void read_and_write(void){
	
	printf("W&R\n");
	int first = 1, end_flag = 0;

	char *path;
	char buf[SIZE_BLOCK]; // SIZE_BLOCK гарантированно больше максимального размера имени файла + 2 * sizeof(unsigned int)
	char read_buf[SIZE_BLOCK];

	sigset_t s, os;
	sigfillset(&s);

	struct sembuf wait_opt[2] = {{1, 0, 0},{1, 1, 0}};
	struct sembuf unlock_opt = {0, -1, 0};

//	wait_opt[0].sem_num = sem_id;
//	wait_opt[1].sem_num = sem_id;
//	unlock_opt.sem_num = sem_id;

	unsigned int n;
	unsigned int m;
	int fd;

	size_t size = sizeof(unsigned int);
	size_t block = SIZE_BLOCK;

	while (!end_flag){

		struct msgbuf err;	
		if ( msgrcv(msgq_id, &err, 1, 2, 0) < 0) pexit("msgsnd");
		if (err.mtext[0] == -1) return;
	//	sigprocmask(SIG_BLOCK, &s, &os); // Блокируем сигналы
		if (semop(sem_id, wait_opt, 2) < 0) pexit("semopwr1"); // Блокируем память на чтение и запись
		memcpy(buf, mem_ptr, SIZE_BLOCK); // Принимаем блок
		if (semop(sem_id, &unlock_opt, 1) < 0) pexit("semopwr2"); // Разблокируем память на чтение
	//	sigprocmask(SIG_SETMASK, &os, NULL); // Разблокируем сигналы
		
		block = SIZE_BLOCK;

		if (first){	

			memcpy(&n, buf, size);
			block -= size;
			path = malloc(sizeof(char)* (n + 5));
			memcpy(path, buf + size, n);
			path[n] = '\0';
			strcat(path, ".out");
			block -= n;
			memcpy(&m, buf + size + n, size);
			block -= size;

			fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0660);
			if (fd < 0) {
				perror("open");
				return;
			}
		}


		if (block > 0){
			if (block > m){
				if (first){
					if (write(fd, buf + size + n + size, m) < m) pexit("write");
				}
				else
					if (write(fd, buf, m) < m) pexit("write");
				end_flag = 1;
			}

			else {

				if (first){
					if (write(fd, buf + size + n + size, block) < block) pexit("write");
					first = 0;
				}
				else 
					if (write(fd, buf, block) < block) pexit("write");	
				m -= block;
			}
		}
	}

	close(fd);
	return;
}

int main(int argc, char const *argv[]) {

	int i;
	ipc_key = ftok(FTOK_PATH,FTOK_PROJ);
	printf("KEY = %d\n", ipc_key);

	unsigned short sem_init[3] = {0,1,1}, sem[3];

	if ( (msgq_id=msgget(ipc_key, IPC_CREAT | IPC_EXCL |  0660)) < 0 )
			if ( (msgq_id=msgget(ipc_key, 0)) < 0 ) pexit("msgget");

	struct msgbuf message;

	message.mtype = 1;
	message.mtext[0] = 0;

	if ( msgrcv(msgq_id, &message, 1, 1, 0) < 0) pexit("msgsnd");

	int count = message.mtext[0];

	if ( (shm_id=shmget(ipc_key, sizeof(char) * SIZE_BLOCK, IPC_CREAT | 0660)) < 0)
			pexit("shmget");
	
	mem_ptr = shmat (shm_id, NULL, 0);
	
	if (mem_ptr == (void *) -1)
		pexit("shmat");

	struct sembuf init = {2, -1, 0};

	if ((sem_id = semget (ipc_key, 3, IPC_CREAT | IPC_EXCL | 0660)) < 0 ){
			if ((sem_id = semget (ipc_key, 0, 0)) < 0) pexit("semget");
	//		init.sem_num = sem_id;
			semctl(sem_id, 3, GETALL, sem);
			if (semop(sem_id, &init, 1) < 0) pexit("semopwr3");
	}
	else {
		semctl(sem_id, 3, SETALL, sem_init);
	}


	for (i = 0; i < count - 1; i++){
		read_and_write();
	}

	cleanup();
	return 0;
}
