#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>

struct msgbuf
{
	long mtype;
	char mtext[100];
}msg;

void *fun(void *arg)
{
	int set;
	struct msgbuf ff;
	memset(ff.mtext, 0, sizeof(ff.mtext));
	while((set = msgrcv(*(int *)arg, &ff, 100, 1, MSG_NOERROR)) > 0)
	{
		printf("rcv:%s\n", ff.mtext);
		memset(ff.mtext, 0, sizeof(ff.mtext));
	}

	return NULL;
}

int main()
{
	int id;
	key_t key = ("/home/chenbiao/mywork/", 2);
	if((id = msgget(key, IPC_EXCL)) < 0)
	{
		id = msgget(key, IPC_CREAT | 0666);
	}
	printf("id = %d\n", id);

	pthread_t tid;
	pthread_create(&tid, NULL, fun, &id);
    msg.mtype = 2;
	while(1)
	{
		gets(msg.mtext);
    	msgsnd(id, &msg, strlen(msg.mtext), MSG_NOERROR);
    }


	pthread_join(tid, NULL);
}