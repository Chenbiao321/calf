#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <pthread.h>
#include "cbmusic.h"

int i;

long Conversion(char *time)
{
	long t;
	long a, b, c;
	char *p;
	*(strchr(time, ':')) = ' ';
	if((p = strchr(time, '.')) != NULL)
		*p = ' ';
	else
		strcat(time, " 0");
	sscanf(time, "%ld%ld%ld", &a, &b, &c);
	t = b*1000+a*60*1000;
	return t;
}

PNODE DownloadIrc(char *bc)
{
	FILE *fp = fopen(bc, "r");
	if(fp == NULL)
	{
        printf("找不到歌词文件lrc\n");
		return NULL;
	}
    PNODE head = CreateList();
    char TIME[11];
    char lyrics[60];
    char buff[150];
    int i;
    fgets(buff, 150, fp);
    char *lp = strchr(buff, ']');

    while(*(lp+1) == '\r' || *(lp+1) == '\n')
    {
    	printf("%s", buff);
    	fgets(buff, 150, fp);
        lp = strchr(buff, ']');
    }
    fseek(fp, -(strlen(buff)), SEEK_CUR);
    while(fgets(buff, 150, fp) != NULL)
    {
    	if(buff[0] != '[')
    		continue;
    	char *pf = buff;
    	buff[strlen(buff) - 1] = '\0';
    	char *p;
    	while((p = strchr(buff, ']')) != NULL)
    	{
    		*p = '*';
    		if(*(p+1) != '[')
    			break;
    	}
    	strcpy(lyrics, p+1);
    	while((p = strchr(pf, '*')) != NULL)
    	{
    		*p = '\0';
    		strcpy(TIME, pf+1);
    		InsertList(head, Conversion(TIME), lyrics);
    		
    		pf = p+1;
        }
    }
    fclose(fp);
    return head;
}

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	head->time = 0;
	strcpy(head->lyrics, "lyrics");

	head->piror = head;
	head->next = head;
	return head;
}

void InsertList(PNODE head, long time, char *lyrics)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	pnew->time = time;
	strcpy(pnew->lyrics, lyrics);

	PNODE p = head->next;

	while(p != head)
	{
		if(pnew->time <= p->time)
		{
			pnew->piror = p->piror;
			p->piror->next = pnew;
			p->piror = pnew;
			pnew->next = p;
			break;
		}
		else
			p = p->next;
	}
	if(p == head)
	{
		pnew->piror = p->piror;
		p->piror->next = pnew;
		p->piror = pnew;
		pnew->next = p;
	}
}

char *FindList(PNODE head, long time)
{
	char *fp;
	PNODE p = head->next;

	while(p != head)
	{
		if(time == p->time)
		{
			fp = p->lyrics;
			return fp;
		}
		else
			p = p->next;
	}
	return NULL;
}

void ClearList(PNODE head)
{
	if(head == NULL)
		return;
	PNODE p = head->next;
	if(p != head)
	{
		p->next->piror = head;
		head->next = p->next;
		free(p);
		ClearList(head);
	}	
}

void displaylist(PNODE head)
{
	PNODE p = head->next;

	while(p != head)
	{
		printf("%s\n", p->lyrics);
		printf("%ld\n", p->time);
		p = p->next;
	}
}




















void SaveGedang(PLIST list)
{
	PLIST p = list->next;
	FILE *fd = fopen("/home/chenbiao/mywork/4.5/Gedang.txt", "w");
	while(p != list)
	{
		fprintf(fd, "%s\n", p->path);
		p = p->next;
	}
	fclose(fd);
}

PLIST CreateGedang()
{
	PLIST list = (PLIST)malloc(sizeof(LIST));
	assert(list != NULL);

    memset(list->path, 0, sizeof(list->path));

	list->piror = list;
	list->next = list; 
    
    return list;
}

void DelectGedang(PLIST list, int data)
{
	PLIST p = list->next;
	while(p != list)
	{
		if(p->data == data)
		{
			p->next->piror = p->piror;
			p->piror->next = p->next;
			free(p);
			break;
		}
		else
			p = p->next;
	}
	PLIST q = list->next;
	--list->data;
	int j = 0;
	while(q != list)
	{
		q->data = j;
		++j;
		q = q->next;
	}
}
int ChackGdang(PLIST list, char *path)
{
	PLIST p = list->next;
	while(p != list)
	{
		if(strcmp(p->path, path) == 0)
			return 1;
		else
			p = p->next;
	}
	return 0;
}

void InsertGedang(PLIST list, char *path)
{
	if(ChackGdang(list, path))
	{
		printf("已经存在\n");
		return;
	}
	PLIST pnew = (PLIST)malloc(sizeof(LIST));
	assert(pnew != NULL);

	strcpy(pnew->path, path);
	pnew->data = i;
	++i;

	pnew->piror = list->piror;
	list->piror->next = pnew;
	pnew->next = list;
	list->piror = pnew;

}

void InsertGdDIR(char *str, PLIST list)
{
	DIR *pf = opendir(str);
	if(pf == NULL)
		return;
	struct dirent *dir;
	char path[80];
	while((dir = readdir(pf)) != NULL)
	{
		strcpy(path, str);
		char *p = (dir->d_name)+strlen(dir->d_name)-3;
		if(strcmp(p, "mp3") == 0)
		{
			if(path[strlen(path)-1] != '/')
				strcat(path, "/");
			strcat(path, dir->d_name);
			//printf("%d\n", dir->d_type);
			InsertGedang(list, path);
		}
		else if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0) && (dir->d_type == 4))
		{
			if(path[strlen(path)-1] != '/')
				strcat(path, "/");
			strcat(path, dir->d_name);
			InsertGdDIR(path, list);
		} 
	}
	printf("insert success\n");
	closedir(pf);
}

PLIST DownloadGedang()
{
	FILE *fd = fopen("/home/chenbiao/mywork/4.5/Gedang.txt", "r");
	if(fd == NULL)
	{
		FILE *fd1 = fopen("/home/chenbiao/mywork/4.5/Gedang.txt", "w");
		fclose(fd1);
	}
	char buff[80];
	PLIST list = CreateGedang();

	while(fgets(buff, 80, fd) != NULL)
	{
		buff[strlen(buff)-1] = '\0';
		InsertGedang(list, buff);
	}
	list->data = i-1;

	fclose(fd);

	return list;
}

void Help()
{
	printf("添加歌曲：addmusic 文件绝对路径\n");
	printf("添加歌曲：addmusicdir 目录的绝对路径\n");
	printf("退出：quit\n");
	printf("查看歌单：list\n");
	printf("删除歌曲：delmusic\n");
	printf("清空歌单：clear\n");
	printf("下一首:next\n");
	printf("上一首:piror\n");
	printf("播放指定歌曲:play music_id\n");
	printf("单曲循环:mode 0\n");
	printf("列表循环:mode 1\n");
	printf("随机播放:mode 3\n");
	printf("快进:seek x\n");
	printf("后退:seek -x\n");
	printf("音量调节:volume x 1\n");
	printf("是否静音:mute 1/0\n");
	printf("查看目前模式， 正在播放的歌曲序号:which\n");
}

void DisplayGedang(PLIST list)
{
	PLIST p = list->next;
	if(p == NULL)
		return;
	while(p != list)
	{
		printf("%d, %s\n", p->data, p->path);
		p = p->next;
	}
}

void ClearGedang(PLIST list)
{
	i = 0;
	PLIST p = list->next;
	if(p != list)
	{
		p->next->piror = list;
		list->next = p->next;
		free(p);
		ClearGedang(list);
	}	
}
















//加载上一次的进度，模式等
PPZWJ Downloadpzwj()
{
	PPZWJ head = (PPZWJ)malloc(sizeof(PZWJ));
	assert(head != NULL);

	FILE *pf = fopen("/home/chenbiao/mywork/4.5/pzwj.txt", "r");
	if(pf == NULL)
	{
		FILE *pf = fopen("/home/chenbiao/mywork/4.5/pzwj.txt", "w");
		Firstpzwj(pf);
		fclose(pf);
	}
	char buf[100];
	char *p;
	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->music_id));

	fgets(buf, 100, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%s", head->musicname);

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%ld", &(head->time));

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->playmode));

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->OnOff));

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->OUTDO));

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->IRCDO));

	fgets(buf, 20, pf);
	p = strchr(buf, '=');
	sscanf(p+1, "%d", &(head->musicnumber));

	fclose(pf);

	return head;
}


void Sendcmd(char *buff, int set)
{
	int std = dup(1);
	int fd = open("fifo", O_WRONLY);
	//printf("Insert success\n");
	dup2(fd, 1);
	write(fd, buff, set);
	dup2(std, 1);
	close(fd);	
}



void FindMusicname(PLIST list, char *path, int *music_id)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(p->data == *music_id)
		{
			strcpy(path, p->path);
			return;
		}
		else
			p = p->next;
	}
	if(*music_id < 0)
	{
		*music_id = list->data;
        strcpy(path, list->piror->path);
	}
	else
	{
		*music_id = 0;
        strcpy(path, list->next->path);
	} 
}

void *GetmusicMSG(void *arg)
{
	char cmd[20];
    PPZWJ Chusi = (PPZWJ)arg;
    
	while(Chusi->OnOff)
	{
		if(Chusi->PAUSE)
		{
			sleep(1);
		}
		else
		{
			if(Chusi->OUTDO)
			{
				usleep(100000);
				sprintf(cmd, "get_time_length\n");
				int set = strlen(cmd);
				Sendcmd(cmd, set);
			}	
			sprintf(cmd, "get_time_pos\n");
			int set = strlen(cmd);
			Sendcmd(cmd, set);
			sleep(1);
		}
	}
	printf("off success\n");
}

void PlayChusimusic(PPZWJ Chusi)
{
	PPZWJ p = Chusi;
	char cmd[20];
	sprintf(cmd, "seek %ld\n", p->time);
	int set = strlen(cmd);
	Sendcmd(cmd, set);
}

void Savepzwj(PPZWJ Chusi)
{
	PPZWJ p = Chusi;
	FILE *fp = fopen("/home/chenbiao/mywork/4.5/pzwj.txt", "w");

	char buf[100];
	
	fprintf(fp, "num=%d\n", p->music_id);

	fprintf(fp, "musicname=%s\n", p->musicname);

	fprintf(fp, "time=%ld\n", p->time);

	fprintf(fp, "playmode=%d\n", p->playmode);

	fprintf(fp, "OnOff=%d\n", 1);

	fprintf(fp, "OUTDO=%d\n", 1);

	fprintf(fp, "IRCDO=%d\n", 1);

	fprintf(fp, "PAUSE=%d\n", 0);

	fprintf(fp, "musicnumber=%d\n", p->musicnumber);

	fclose(fp);

}

void Firstpzwj(FILE *fp)
{
	char buf[100];
	
	fprintf(fp, "num=%d\n", 0);

	printf("please give one musicname");
	scanf("%s", buf);

	fprintf(fp, "musicname=%s\n", buf);

	fprintf(fp, "time=%d\n", 0);

	fprintf(fp, "playmode=%d\n", 1);

	fprintf(fp, "OnOff=%d\n", 1);

	fprintf(fp, "OUTDO=%d\n", 1);

	fprintf(fp, "IRCDO=%d\n", 1);

	fprintf(fp, "PAUSE=%d\n", 1);

	fprintf(fp, "musicnumber=%d\n", 0);
	
}

void fun()
{
	int status;
	printf("pid = %d\n", wait(NULL));
	if(waitpid(1, &status, WEXITSTATUS(status)))
		printf("%d\n", status);
	else if(waitpid(1, &status, WIFSIGNALED(status)))
		printf("error\n");
	printf("child over\n");
}

void fun1()
{
	printf("1111111\n");
}

void *PlayLyrics(void *arg)
{
	int fd1 = open("timefifo", O_RDONLY);
	PPZWJ Chusi = (PPZWJ)arg;
    
    char time[20];
    long ltime = 0;
    PNODE head = NULL;
    char *p;
    while(1)
    {
    	if(LRCIO == 1)
    	{	
    		pthread_mutex_lock(&mutex);
    		pthread_cond_wait(&cond, &mutex);
    		pthread_mutex_unlock(&mutex);
    	}
    	else
    	{
    	if(Chusi->IRCDO)
    	{
    		--Chusi->IRCDO;
    		ClearList(head);
    		char path[100] = {0};
    		Changetolrc(Chusi, path);
    		printf("1111111\n");
    		head = DownloadIrc(path);
    		if(head == NULL)
    			head = CreateList();
    		p = FindList(head, ltime);
			if(p != NULL)
			printf("%s\n", p);
		}
    
    	if(read(fd1, time, sizeof(time)) > 0)
    	{
    		//time[strlen(time)-2] = time[strlen(time)-1];
    		//time[strlen(time)-1] = '\0';
    		ltime = (long)(atoi(time)*1000);
    		//printf("%ld\n" ,ltime);
		
	   		char *p = FindList(head, ltime);
	    	if(p != NULL)
	    	printf("%s\n", p);
		}
		}
	}
	ClearList(head);

    close(fd1);
        
}


char *Changetolrc(PPZWJ Chusi, char *path)
{
	int len = strlen(Chusi->musicname);
	memcpy(path, Chusi->musicname, len);
	

	char *p;
	char name[40] = {0};
    p = path+strlen("/home/chenbiao/mywork/4.5/mp3/")-1;
    *p = '\0';
    sprintf(name, "%s", p+1);
    name[strlen(name)-3] = '\0';
    strcat(name, "lrc");
   
    memset(path, 0, strlen(path));
	sprintf(path, "/home/chenbiao/mywork/4.5/gechi/%s", name);
	printf("%s\n", path);
    
	return path;
}


