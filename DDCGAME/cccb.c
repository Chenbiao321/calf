#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/msg.h>
#include <assert.h>
#include <sys/msg.h>
#include "cccb.h"

int i;




void *RefreshScreen(void *arg)
{
	int flag = fcntl(0, F_GETFL);
	flag = flag&(~O_NONBLOCK);
	//flag = flag|O_NONBLOCK;
	if(flag&O_NONBLOCK)
		printf("O_NONBLOCK\n");
	fcntl(0, F_SETFL, flag);
    
	PLIST head = Downloaddangchi();
	PNODE screen = (PNODE)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(num == 1)
		{
			pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond1, &mutex);
			pthread_mutex_unlock(&mutex);	
		}
		else
		{			
			pthread_mutex_unlock(&mutex);
            
			int WeiZhi = rand()%30+5;
			Finddangchi(head, rand()%i);
			//printf("%s\n", DANGCHI);
			pthread_rwlock_wrlock(&rwlock);
            InsertScreenList(screen->next->next, DANGCHI, WeiZhi); 
            DelectScreenList(screen);
            pthread_rwlock_unlock(&rwlock);
            *DANGCHI = '\0';
            ++num;
			
			pthread_cond_signal(&cond);
			sleep(1);
		}
	}	
	return NULL;
}

PLIST CreateList()
{
	PLIST head = (PLIST)malloc(sizeof(LIST));
	assert(head != NULL);

	head->dangchi_id = 0;
	strcpy(head->dangchi, "chenbiao");

	head->piror = head;
	head->next = head;

	return head;
}

void InsertList(PLIST head, char *dangchi)
{
	PLIST pnew = (PLIST)malloc(sizeof(LIST));
	assert(pnew != NULL);

	pnew->dangchi_id = i;
	++i;
	strcpy(pnew->dangchi, dangchi);

	pnew->piror = head->piror;
	head->piror->next = pnew;
	pnew->next = head;
	head->piror = pnew;
}

PLIST Downloaddangchi()
{
	PLIST head = CreateList();

	FILE *fp = fopen("dangchi.txt", "r");
	if(fp == NULL)
	{
		printf("缺失单词文件\n");
		return NULL;
	}

	char dangchi[20];
	while(fgets(dangchi, sizeof(dangchi), fp) > 0)
	{
		dangchi[strlen(dangchi)-1] = '\0';
		InsertList(head, dangchi);
		printf("%s\n", dangchi);
	}
	head->dangchi_id = i;
	fclose(fp);

	return head;
}

void Finddangchi(PLIST head, int dangchi_id)
{
	PLIST p = head->next;

	while(p != head)
	{
		if(p->dangchi_id == dangchi_id)
		{
			strcpy(DANGCHI, p->dangchi);
			//printf("%s\n", DANGCHI);
			break;
		}
		else
			p = p->next;
	}
}









PNODE CreateScreenList()
{
	PNODE screen = (PNODE)malloc(sizeof(NODE));
	assert(screen != NULL);

	for(int i = 1;i < 49;++i)
	{
		*(screen->hang+i) = ' ';
	}
	*(screen->hang) = '|';
	*(screen->hang+49) = '|';
	strcpy(screen->dangchi, "\0");

	screen->piror = screen;
	screen->next = screen;

	return screen;
}

void InsertScreenList(PNODE screen, char *dangchi, int WeiZhi)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	if(WeiZhi == 0)
	{
		for(int i = 1;i < 49;++i)
		{
			*(pnew->hang+i) = '=';
		}
		*(pnew->hang) = '|';
		*(pnew->hang+49) = '|';
		strcpy(pnew->dangchi, "11111111111");
	
	}

	else
	{
		strcpy(pnew->hang, screen->hang);
		strcpy(pnew->hang+WeiZhi, dangchi);
		*(pnew->hang+WeiZhi+strlen(dangchi)) = ' ';
		strcpy(pnew->dangchi, dangchi);
	}

	pnew->next = screen->next;
	screen->next->piror = pnew;
	pnew->piror = screen;
	screen->next = pnew;
}

PNODE DownloadScreenList()
{
	PNODE screen = CreateScreenList();

	FILE *fp = fopen("screen.txt", "r");
	if(fp == NULL)
	{
		InsertScreenList(screen, " ", 0);
		for(int i = 0;i < 33;++i)
		{
			InsertScreenList(screen, " ", 1);
		}
		InsertScreenList(screen, " ", 0);
		
		return screen;
	}

	char dangchi[20];
	InsertScreenList(screen, " ", 0);
	
	while(fgets(dangchi, sizeof(dangchi), fp) > 0)
	{
		dangchi[strlen(dangchi)-1] = '\0';
		char *p = strchr(dangchi, ' ');
		*p = '\0';
		InsertScreenList(screen, dangchi, atoi(p+1));
	}
	InsertScreenList(screen, " ", 0);
	fclose(fp);

	return screen;
}

void DelectScreenList(PNODE screen)
{
	PNODE p = screen->piror->piror;

	p->next->piror = p->piror;
	p->piror->next = p->next;
	
	free(p);
}

void DisplayScreenList(PNODE screen)
{
	PNODE p = screen->next;

	while(p != screen)
	{
		write(1, p->hang, 50);
		printf("\n");
		p = p->next;
	}
}

void ClearScreenList(PNODE screen)
{
	PNODE p = screen->next;

	if(p != screen)
	{
		screen->next = p->next;
		p->next->piror = screen;
		free(p);
		ClearScreenList(screen);
	}
}

void SaveScreenList(void *addr, PNODE screen)
{
	char (*p)[50] = (char (*)[50])addr;
	PNODE q = screen->next;

	for(int i = 0;i < 35;++i)
	{
		memcpy(p+i, q->hang, 50);
		q = q->next;
	}

}

void *DelectScreenDanci(void *arg)
{
	int id1;
	key_t key2 = ftok("/home/chenbiao/mywork/", 3);

	if((id1 = msgget(key2, IPC_EXCL)) < 0)
	{
		id1 = msgget(key2, IPC_CREAT | 0666);
	}


	PNODE screen = (PNODE)arg;
	char buff[20];
	while(1)
	{
		msgrcv(id1, &msg, 20, 1, MSG_NOERROR);
		strcpy(buff, msg.mtext);
		printf("%s\n", buff);
    	pthread_rwlock_wrlock(&rwlock);
    	PNODE p = screen->piror->piror;
    	while(p != screen->next)
    	{
    		if(strcmp(buff, p->dangchi) == 0)
    		{
    			strcpy(p->hang, screen->hang);
    			strcpy(p->dangchi, screen->dangchi);
    			break;
    		}

    		else
    		{
    			p = p->piror;
    		}
    	}
    	pthread_rwlock_unlock(&rwlock);
    	//DisplayScreenList(screen);
    	memset(buff, 0, sizeof(buff));
    	//sleep(1);
    }
}








