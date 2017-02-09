/*
Игра «Крестики-нолики».
Размер поля по желанию.
Состояние доски хранится в разделяемой памяти.
Доступ к доске синхронизировать с помощью семафоров
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>

int flag = 55;
int n; // Размер поля
key_t key; // Ключи поля, массива семафоров
int *shmaddr = NULL; // Указатель на поле
int shmid = -1, semid = -1, msgq_id = -1; // Дескрипторы поля, массива семафоров
struct mesbuf{
long mtype;
char mtext[2];
};

// Ошибка
#define ERROR(x) \
	printf("Ошибка %s\n", x); \
	return 1	

// Ф-я, освобождающая все занятые ресурсы
int delete(void) 
{
	// Отключение поля (сегмента разд-й памяти) 
	if(shmaddr != NULL)
		shmdt(shmaddr); 

	// Удаление массива семафоров
	if(semid >= 0) 
		semctl(semid, 0, IPC_RMID);

	// Удаление поля
	if(shmid >= 0)
		shmctl(shmid, IPC_RMID, 0);
}

// Ф-я, устанавливаящая соответствие м/у 1, 0, 5 и X, O, ?
char interrelation(int x)
{
	switch(x)
	{
		case 0:
			return 'O';
			break;
		case 1:
			return 'X';
			break;
		default:
			return '?';
			break;
	}
}

// Ф-я, печатающая поле
int print(int *mas)
{
	int i, j;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
			printf("%c", interrelation(mas[i*n+j]));
		printf("\n");
	}

	return 0;
}

// Ф-я, анализируящая поле (возврат: 5 - выигрыш, 0 - нет выигрыша)
int analise(int *mas)
{
	int tmp;
	int count; // Счетчик совпадений
	int i, j;

	// Просмотр главной диагонали
	if((tmp = mas[0]) != 5)
	{
		count = 1;
	
		for(i = 1; i < n; i++)
			if(mas[i*n+i] == tmp)
				count++;
	
		if(count == n)
		{
			printf("Выиграл %d игрок\n", tmp);
			return 5;
		}
	}

	// Просмотр побочной диагонали
	if((tmp = mas[n-1]) != 5)
	{
		count = 1;
	
		for(i = 1; i < n; i++)
			if(mas[i*n+n-1-i] == tmp)
				count++;
	
		if(count == n)
		{
			printf("Выиграл %d игрок\n", tmp);
			return 5;
		}
	}

	// Просмотр строк
	for(i = 0; i < n; i++)
	{
		if((tmp = mas[i*n]) != 5)
		{
			count = 1;
	
			for(j = 1; j < n; j++)
				if(mas[i*n+j] == tmp)
					count++;
	
			if(count == n)
			{
				printf("Выиграл %d игрок\n", tmp);
				return 5;
			}
		}
	}

	// Просмотр столбцов
	for(j = 0; j < n; j++)
	{
		if((tmp = mas[j]) != 5)
		{
			count = 1;
	
			for(i = 1; i < n; i++)
				if(mas[i*n+j] == tmp)
					count++;
	
			if(count == n)
			{
				printf("Выиграл %d игрок\n", tmp);
				return 5;
			}
		}
	}

	return 0;
}

int play(int k)
{
	struct sembuf op_wait = {k, -1, 0};
	struct sembuf op_unlock = { (k + 1)%2, 1, 0};

	//struct sembuf op_wait[] = {{k, 0, 0}, {k, 1, 0}};
	//struct sembuf op_unlock = { (k + 1)%2, 1, 0};
	
	printf("Ждем противника\n");
	semop(semid, &op_wait, 1);

	if(analise(shmaddr) == 5)
		return 5;

	print(shmaddr);

	int x, y;
	printf("Сделайте ход: ");
	scanf("%d %d", &x, &y);

	x -= 1;
	y -= 1;

	if(k == 0)
		shmaddr[x*n+y] = 0;
	else 
		shmaddr[x*n+y] = 1;

	if(analise(shmaddr) == 5)
		return 5;

	semop(semid, &op_unlock, 1);

	return 0;
}


int main()
{
	delete();
	int k; // Номер игрока

	creat("tmp", 0666);
	key = ftok("tmp", 0);
	if(key == -1)
	{
		ERROR("ключ");
	}

/*
semid = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL);
	semctl(semid, 2, SETVAL, 1);
	if(semid > 0)
	{
		struct sembuf init1[] = {{2, -1, 0}};
		semop(semid, init1, 1);
		printf("Введите размер поля n*n, n = ");
		scanf("%d", &n);

		printf("Введите номер игрока (0 или 1), k = ");
		scanf("%d", &k);

		// Создание поля
		shmid = shmget(key, sizeof(int)*n*n, 0666 | IPC_CREAT | IPC_EXCL);
		if(shmid == -1)
			shmid = shmget(key, sizeof(int)*n*n, 0);

		// Подключение поля
		shmaddr = shmat(shmid, NULL, 0);

		// Обнуление поля
		int i, j;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				shmaddr[i*n+j] = 5;
			}

		semctl(semid, 2, SETVAL, 1);
	}
	else
	{
		struct sembuf init2[] = {{2, -1, 0}};
		semop(semid, init2, 1);
		struct sembuf init3[] = {{1, 1, 0}};
		semop(semid, init3, 1);
			
	}

*/

semid = semget(key, 3, 0666 | IPC_CREAT | IPC_EXCL);
	if(semid > 0)
	{
		unsigned short init[] = {1, 0, 1};
		semctl(semid, 0, SETALL, init);
}
else
{
	semid = semget(key, 0, 0);
	struct sembuf init2[] = {{2, -1, 0}, {2, 1, 0}};
	semop(semid, init2, 2);
}




msgq_id = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
if (msgq_id < 0){
msgq_id = msgget(key, 0);
struct mesbuf message;
msgrcv(msgq_id, &message, 2, 1, 0);
n = message.mtext[0];
k = message.mtext[1];
}
else {
printf("Введите размер поля n*n, n = ");
		scanf("%d", &n);
printf("Введите номер игрока (0 или 1), k = ");
		scanf("%d", &k);
struct mesbuf message = {1, {n, (k+1)%2}};
msgsnd(msgq_id, &message, 2, 0);
}

	int count = n*n/2; // Счетчик ходов	


// Создание поля
		shmid = shmget(key, sizeof(int)*n*n, 0666 | IPC_CREAT | IPC_EXCL);
		if(shmid == -1)
			shmid = shmget(key, sizeof(int)*n*n, 0);

		// Подключение поля
		shmaddr = shmat(shmid, NULL, 0);

		// Обнуление поля
		int i, j;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				shmaddr[i*n+j] = 5;
			}



/*

	// Семафор
	semid = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL);
	if(semid > 0)
	{
		unsigned short init[] = {1, 0, 1};
		semctl(semid, 0, SETALL, init);

		struct sembuf init1[] = {{0, -1, 0}, {2, -1, 0}};
		semop(semid, init1, 2);

		printf("Введите размер поля n*n, n = ");
		scanf("%d", &n);

		printf("Введите номер игрока (0 или 1), k = ");
		scanf("%d", &k);

		// Создание поля
		shmid = shmget(key, sizeof(int)*n*n, 0666 | IPC_CREAT | IPC_EXCL);
		if(shmid == -1)
			shmid = shmget(key, sizeof(int)*n*n, 0);

		// Подключение поля
		shmaddr = shmat(shmid, NULL, 0);

		// Обнуление поля
		int i, j;
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
			{
				shmaddr[i*n+j] = 5;
			}
		
		semctl(semid, 2, SETVAL, 1);
	}
	else
	{
		struct sembuf init2[] = {{2, -1, 0}, {0, 1, 0}};
		semop(semid, init2, 2);
	}
*/
	// Игра
	do
		count--;
	while (((flag = play(k) )== 0) && (count > 0));	
	
	if((flag == 0) && (count == 0))
		printf("Ничья\n");
 
	// Завершение игры
	delete(); 

	return 0;
}