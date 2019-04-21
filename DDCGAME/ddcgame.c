#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include "cccb.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int num = 0;
char DANGCHI[20] = "hello";

union senun
{
	int val;
	struct semmid_ds *buf;
	unsigned short *array;
}agc;

int p(int id)
{
	struct sembuf sem;
	sem.sem_num = 0;
	sem.sem_op = -1;
	sem.sem_flg = SEM_UNDO;
	return semop(id, &sem, 1);
}

int v(int id)
{
	struct sembuf sem;
	sem.sem_num = 0;
	sem.sem_op = +1;
	sem.sem_flg = SEM_UNDO;
	return semop(id, &sem, 1);
}



int main()
{

    srand(time(NULL));

    int id1;
    key_t key1 = ftok("/home/chenbiao/mywork/", 2);

	if((id1 = semget(key1, 0, IPC_EXCL)) < 0)
	{
		id1 = semget(key1, 1, IPC_CREAT | 0666);
		agc.val = 1;
		semctl(id1, 0, SETVAL, agc);
	}
	printf("id = %d", id1); 

	int id;
	key_t key = ftok("/home/chenbiao/mywork/", 1);
	if((id = shmget(key, 1024, IPC_EXCL)) < 0)
	{
		id = shmget(key, 1024, IPC_CREAT | 0666);
	}
	printf("id = %d", id);
	void *addr = shmat(id, NULL, 0);
	PNODE screen = DownloadScreenList();
	char str[35][50];
	memcpy(addr, str, size);
    SaveScreenList(addr, screen);

	pthread_t tid;
	pthread_create(&tid, NULL, RefreshScreen, screen);

   // if(access("fifo", F_OK) < 0)
    //{
  //  	mkfifo("fifo", 0666);
   // }
   // int fd = open("fifo", O_WRONLY);
   // printf("open success\n");
    pthread_t tid1;
    pthread_create(&tid1, NULL, DelectScreenDanci, screen);
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(num == 0)
		{	
			pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond, &mutex);
			pthread_mutex_unlock(&mutex);	
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			p(id1);
			SaveScreenList(addr, screen);
			v(id1);
			DisplayScreenList(screen);
			--num;
			pthread_cond_signal(&cond1);
		}
	}
	pthread_join(tid, NULL);
	shmdt(addr);

	return 0;
}