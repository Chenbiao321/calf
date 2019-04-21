//歌曲播放器模拟
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <signal.h>
#include <sys/shm.h>
#include <wait.h>
#include "bcbc.h"

enum mode{MODE1, MODE2} mode;

int analyse(char *buff, char *str)
{
	printf("%d\n", mode);
	char *p;
	if((p = strchr(buff, ' ')) != NULL)
	{
		*p = '\0';
		strcpy(str, p+1);
		if(strcmp(buff, "play") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 2;
		}
		else if(strcmp(buff, "seek") == 0)
		{
			*p = ' ';
			return 4;
		}
		else if(strcmp(buff, "mode") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 5;
		}
	}
	else if(strcmp(buff, "next\n") == 0)
		return 3;
	else if(strcmp(buff, "piror\n") == 0)
		return 1;
	else if(strcmp(buff, "quit\n") == 0)
		return 6;
	else if(strcmp(buff, "which\n") == 0)
		return 7;
	else
		return 0;
}

//加载上一次的进度，模式等
PPZWJ Downloadpzwj()
{
	PPZWJ head = (PPZWJ)malloc(sizeof(PZWJ));
	assert(head != NULL);

	FILE *pf = fopen("/home/chenbiao/mywork/4.5/pzwj.txt", "r");
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

	return head;
}


void Sendcmd(char *buff, int set)
{
	int std = dup(1);
	int fd = open("fifo", O_WRONLY);
	printf("Insert success\n");
	dup2(fd, 1);
	write(1, buff, set);
	dup2(std, 1);
	close(fd);	
}



void FindMusicname(PLIST list, char *path, int music_id)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(p->data == music_id)
		{
			strcpy(path, p->path);
			return;
		}
		else
			p = p->next;
	}
	printf("not have this music\n");
}

void *GetmusicMSG(void *arg)
{
	char cmd[20];
    PPZWJ Chusi = (PPZWJ)arg;
    
	while(Chusi->OnOff)
	{
		if(Chusi->OUTDO)
		{
			sprintf(cmd, "get_time_length\n");
			int set = strlen(cmd);
			Sendcmd(cmd, set);
		}	
		sprintf(cmd, "get_time_pos\n");
		int set = strlen(cmd);
		Sendcmd(cmd, set);
		sleep(1);
	}
	printf("off success\n");
}

void PlayChusimusic()
{
	;
}

void fun()
{
	wait(NULL);
	printf("child over\n");
}


int main()
{
    signal(SIGCHLD, fun);

	int id;
	key_t key = ftok("/home/chenbiao/mywork/4.5/", 1);
	if((id = shmget(key, sizeof(PZWJ)+5, IPC_EXCL)) < 0)
	{
		id = shmget(key, sizeof(PZWJ)+5, IPC_CREAT | 0666);
	}
	void *addr = shmat(id, NULL, 0);

	PLIST list = DownloadGedang();
	printf("Gedang insert success\n");

	PPZWJ ChuSi = Downloadpzwj();
	printf("%ld\n", sizeof(PZWJ));
	memcpy(addr, (void *)ChuSi, sizeof(PZWJ));
	PPZWJ chuSi = (PPZWJ)addr;
	printf("即将播放：\n");
	printf("music_id = %d\n", ChuSi->music_id);
	printf("musicname = %s\n", ChuSi->musicname);
	printf("time = %ld\n", ChuSi->time);
	printf("playmode = %d\n", ChuSi->playmode);
	

	printf("press a key to goon\n");
	getchar();

	char buff[20];
	char str[20];
	if(access("fifo", F_OK) < 0)
	{
		mkfifo("fifo", 0666);
	}

	int df[2];
	pipe(df);

	pid_t pid = vfork();
	if(pid < 0)
		return 0;

	else if(pid == 0)
	{
		sleep(2);
		close(df[0]);
        dup2(df[1], 1);
		execl("/usr/bin/mplayer", "mplayer", "-slave", "-quiet", "-input", "file=/home/chenbiao/mywork/4.5/gubf/fifo", ChuSi->musicname, NULL);
	    close(df[1]);
	}

	else
	{
		PlayChusimusic();
        pid_t pid = fork();
        if(pid < 0)
        	return 0;
        //接受mplayer传回的信息
        else if(pid == 0)
        {
        	void *addr = shmat(id, NULL, 0);
        	PPZWJ Chusi = (PPZWJ)addr;
        	close(df[1]);
        	char MSG[40];
        	char *p;
        	int set;
        	char length[10] = {0};
        	
        	while((set = read(df[0], MSG, sizeof(MSG))) > 0)
        	{ 		
        		//printf("%s", MSG);
        		MSG[strlen(MSG)-1] = '\0';
        		if(Chusi->OUTDO)
        		{

        		if(((p = strchr(MSG, '=')) != NULL)&&(strchr(MSG, '.') != NULL))
                {
                	if(p[strlen(p+1)-1] == '0')
                	{
                	    printf("%s, %d\n", p+1, Chusi->OUTDO);
                	    strcpy(length, p+1);
                	    Chusi->OUTDO = 0;
                	}
                }
                memset(MSG, 0, sizeof(MSG));
                printf("11111\n");
                continue;
                }
                if((p = strchr(MSG, '=')) != NULL)
                {
                	printf("%s\n", p+1);
                	printf("%s\n", length);
                    if(atoi(p+1) > (atoi(length)-2))
                	{
                		Chusi->OUTDO = 1;
                		char cmd[100] = {0};
						char path[80] = {0};
						Chusi->music_id = (Chusi->music_id)+(Chusi->playmode);
						FindMusicname(list, path, Chusi->music_id);
						printf("%d\n", Chusi->music_id);
						sprintf(cmd, "loadfile %s\n", path);
						set = strlen(cmd);
						Sendcmd(cmd, set);
						//sleep(1);
						
                	}
                }
        		memset(MSG, 0, sizeof(MSG));
        		
        	}
        	
        	close(df[0]);
        	shmdt(addr);

             
        }	

        else
        {
 
        	int set;
        	pthread_t tid;
			pthread_create(&tid, NULL, GetmusicMSG, chuSi);
		while(1)
		{
			memset(buff, 0, sizeof(buff));
			set = read(0, buff, sizeof(buff));
			//printf("3333333\n");
			printf("set = %d, buff = %s", set, buff);
			char cmd[100] = {0};
			char path[80] = {0};
		    switch(analyse(buff, str))
		    {
				case 0:Sendcmd(buff, set);
				break;
				case 1:
				{
					--(chuSi->music_id);
                    FindMusicname(list, path, chuSi->music_id);
					sprintf(cmd, "loadfile %s\n", path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
				}
				break;
				case 2:
				{
					chuSi->music_id = atoi(str);
					FindMusicname(list, path, atoi(str));
					sprintf(cmd, "loadfile %s\n", path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
				}
				break;
				case 3:
				{
					++(chuSi->music_id);
					FindMusicname(list, path, chuSi->music_id);
					sprintf(cmd, "loadfile %s\n", path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
				}
				break;
				case 4:
				{
					set = strlen(buff);
                    Sendcmd(buff, set);
				}
				break;
				case 5:
				{
					if(atoi(str) == 0)
						chuSi->playmode = MODE1;
					else
						chuSi->playmode = MODE2;
				}
				break;
				case 6:
				{
					chuSi->OnOff = 0;
					sleep(1);

					printf("save success\n");
					close(df[0]);
					Sendcmd("q\n", 2);
					return 0;
					
				}
				break;
				case 7:
				{

					printf("MODE = %d\n", chuSi->playmode);
					printf("music_id = %d\n", chuSi->music_id);
					//printf("music_id = %d\n", Chusi->music_id);
				}
		    }
		    
		}
        }
		
	}
	return 0;
}