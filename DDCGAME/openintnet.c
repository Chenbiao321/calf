#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <stdio.h>

typedef struct node
{
	char ip[20];
	int port;
	struct node *piror;
	struct node *next;
}NODE, *PNODE;

struct msgbuf
{
	long mtype;
	char mtext[20];
};

union senun
{
	int val;
	struct semmid_ds *buf;
	unsigned short *array;
}agc;

int P(int id)
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


PNODE CreateInetList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	strcpy(head->ip, "192.168.0.122");
	head->port = 8888;
    
    head->next = head;
    head->piror = head;

    return head;
}

void InsertInetList(PNODE head, char *ip, int port)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	strcpy(pnew->ip, ip);
	pnew->port = port;
    
    pnew->piror = head->piror;
    head->piror->next = pnew;
    pnew->next = head;
    head->piror = pnew;
}

void DelectInetList(PNODE head, int port)
{
	PNODE p = head->next;

	while(p != head)
	{
		if(p->port == port)
		{
			p->next->piror = p->piror;
			p->piror->next = p->next;
			free(p);
			break;
		}
		else
			p = p->next;
	}
}

int FindInetList(PNODE head, int port)
{
	PNODE p = head->next;

	while(p != head)
	{
		if(p->port == port)
		{
			return 1;
			break;
		}
		else
			p = p->next;
	}
	return 0;
}

void DisplayInetList(PNODE head)
{
	printf("服务器ip = %s\n", head->ip);
	printf("服务器port = %d\n", head->port);
	printf("\n");
	PNODE p = head->next;
    
    while(p != head)
    {
    	printf("客户机ip = %s\n", p->ip);
		printf("客户机port = %d\n", p->port);
		printf("\n");
		p = p->next;
    }
}

int analyse(char *buff)
{
	if(strcmp(buff, "ready") == 0)
		return 1;
	else if(strcmp(buff, "help") == 0)
		return 2;
	else if(strcmp(buff, "quit") == 0)
		return 3;
	else if(strcmp(buff, "load") == 0)
		return 4;
	else
		return 0;	
}

void *Sendscreen(void *arg)
{
	int id;
	key_t key = ftok("/home/chenbiao/mywork/", 1);
	if((id = shmget(key, 1024, IPC_EXCL)) < 0)
	{
		id = shmget(key, 1024, IPC_CREAT | 0666);
	}
	void *shmaddr = shmat(id, NULL, 0);
    char (*screen)[50] = (char (*)[50])shmaddr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	int ONOFF = 1;
	PNODE p = (PNODE)arg;

	while(1)
    {
    	PNODE q = p->next;
    	while(q != p)
    	{
    		struct sockaddr_in sddr;
			sddr.sin_family = AF_INET;
			sddr.sin_port = htons(q->port);
			inet_pton(AF_INET, q->ip, &sddr.sin_addr);
            if(ONOFF == 1)
            {
    			sendto(sock, "start", 6, 0, (struct sockaddr *)&sddr, sizeof(sddr));
    			sleep(1);
    			sendto(sock, "3", 2, 0, (struct sockaddr *)&sddr, sizeof(sddr));
    			sleep(1);
    			sendto(sock, "2", 2, 0, (struct sockaddr *)&sddr, sizeof(sddr));
    			sleep(1);
    			sendto(sock, "1", 2, 0, (struct sockaddr *)&sddr, sizeof(sddr));
    					
            }
    		P(id);
			sendto(sock, shmaddr, 35*50, 0, (struct sockaddr *)&sddr, sizeof(sddr));
            v(id);
			q = q->next;
    	}
    	sleep(1);
    	--ONOFF;
    	printf("send success\n");
    }
}

void *fun(void *arg)
{
	

	PNODE p = (PNODE)arg;
    char buff[20];

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	//for(int i = 0;i < 35;i++)
	//{
		//write(1, screen[i], 50);
		//printf("\n");
	//}

    while(1)
    {
    	int set;
    	printf("ifwatch: put list\n");
    	set = read(0, buff, sizeof(buff));
    	if(strcmp(buff, "list\n") == 0)
    	{
    		DisplayInetList(p);
    	}
    	else if(strcmp(buff, "start\n") == 0)
    	{	
    		pthread_t tid2;
    		pthread_create(&tid2, NULL, Sendscreen, p);	
    	}
    }

    close(sock);
}


int main()
{

	int id;
	key_t key1 = ftok("/home/chenbiao/mywork/", 2);

	if((id = semget(key1, 0, IPC_EXCL)) < 0)
	{
		id = semget(key1, 1, IPC_CREAT | 0666);
		agc.val = 1;
		semctl(id, 0, SETVAL, agc);
	}
	printf("id = %d", id); 

	int id1;
	key_t key2 = ftok("/home/chenbiao/mywork/", 3);

	if((id1 = msgget(key2, IPC_EXCL)) < 0)
	{
		id1 = msgget(key2, IPC_CREAT | 0666);
	}

	int flag = fcntl(0, F_GETFL);
	flag = flag&(~O_NONBLOCK);
	//flag = flag|O_NONBLOCK;
	if(flag&O_NONBLOCK)
		printf("O_NONBLOCK\n");
	fcntl(0, F_SETFL, flag);
	
	PNODE head = CreateInetList();

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	int opt;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		printf("set success\n");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.0.122", &addr.sin_addr);

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("bind error\n");
		return 0;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, fun, head);

    struct sockaddr_in rddr;
    int len = sizeof(rddr);
    int set;
    char buff[20];
	while((set = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&rddr, &len)) >= 0)
	{
		char ip[20];
		int port;
		inet_ntop(AF_INET, &rddr.sin_addr, ip, sizeof(ip));
		port = ntohs(rddr.sin_port);

		struct sockaddr_in sddr;
		sddr.sin_family = AF_INET;
		sddr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &sddr.sin_addr);

		switch(analyse(buff))
		{
			case 1:
			{
				if(FindInetList(head, port))
				{
                    sendto(sock, "you have ready", 15, 0, (struct sockaddr *)&sddr, sizeof(sddr));
				}
				else
				{
					InsertInetList(head, ip, port);
					sendto(sock, "ready success", 14, 0, (struct sockaddr *)&sddr, sizeof(sddr));
				}
			}
			break;
			case 2:
			{
				sendto(sock, "准备:ready", 13, 0, (struct sockaddr *)&sddr, sizeof(sddr));
				sendto(sock, "重进:ready + load", 20, 0, (struct sockaddr *)&sddr, sizeof(sddr));
				sendto(sock, "退出:quit", 12, 0, (struct sockaddr *)&sddr, sizeof(sddr));
			}
			break;
			case 3:
			{
				DelectInetList(head, port);
				sendto(sock, "quit success", 13, 0, (struct sockaddr *)&sddr, sizeof(sddr));
			}
			break;
			case 0:
			{
				struct msgbuf msg;
				msg.mtype = 1;
				strcpy(msg.mtext, buff);
				msgsnd(id1, &msg, 20, MSG_NOERROR);
			}
			break;
			case 4:
			{
				sendto(sock, "1", 2, 0, (struct sockaddr *)&sddr, sizeof(sddr));
			}
		}
		memset(buff, 0, sizeof(buff));
	}

	close(sock);
	return 0;


}