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
#include <time.h>
#include <wait.h>
#include "cbmusic.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int LRCIO;

enum mode{MODE1, MODE2, MODE3} mode;

int analyse(char *buff, char *str)
{
	//printf("%d\n", mode);
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
		if(strcmp(buff, "lrc") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 28;
		}
		else if(strcmp(buff, "seek") == 0)
		{
			*p = ' ';
			return 4;
		}
		else if(strcmp(buff, "volume") == 0)
		{
			*p = ' ';
			return 25;
		}
		else if(strcmp(buff, "mute") == 0)
		{
			*p = ' ';
			return 26;
		}
		else if(strcmp(buff, "mode") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 5;
		}
		else if(strcmp(buff, "addmusic") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 21;
		}
		else if(strcmp(buff, "addmusicdir") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 22;
		}
		else if(strcmp(buff, "delmusic") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 23;
		}
		else if(strcmp(buff, "play") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 24;
		}
		else if(strcmp(buff, "pause") == 0)
		{
			str[strlen(str)-1] = '\0';
			return 30;
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
	else if(strcmp(buff, "help\n") == 0)
		return 8;
	else if(strcmp(buff, "list\n") == 0)
		return 9;
	else if(strcmp(buff, "quit\n") == 0)
		return 10;
	else if(strcmp(buff, "clear\n") == 0)
		return 11;
	else if(strcmp(buff, "help\n") == 0)
		return 27;
	
	else
		return 0;
}



int main()
{
    signal(SIGCHLD, fun);

    srand(time(NULL));
    
	int id;
	key_t key = ftok("/home/chenbiao/mywork/4.5/", 1);
	if(ftok("/home/chenbiao/mywork/4.5/", 1) < 0)
	{
		printf("key chuang jian fail\n");
		return 0;
	}
	if((id = shmget(key, sizeof(PZWJ)+10, IPC_EXCL)) < 0)
	{
		id = shmget(key, sizeof(PZWJ)+10, IPC_CREAT | 0666);
	}
	void *addr = shmat(id, NULL, 0);

	PLIST list = DownloadGedang();
	printf("Gedang insert success\n");

	PPZWJ ChuSi = Downloadpzwj();
	printf("%ld\n", sizeof(PZWJ));
	memcpy(addr, (void *)ChuSi, sizeof(PZWJ));
	PPZWJ chuSi = (PPZWJ)addr;
	printf("wlecome use mplayer!\n");
	printf("即将播放：\n");
	printf("music_id = %d\n", ChuSi->music_id);
	printf("musicname = %s\n", ChuSi->musicname);
	printf("time = %ld\n", ChuSi->time);
	printf("playmode = %d\n", ChuSi->playmode);
	

	printf("press a key to goon\n");
	getchar();

	char buff[100];
	char str[80];

	if(access("fifo", F_OK) < 0)
	{
		mkfifo("fifo", 0666);
	}

	if(access("timefifo", F_OK) < 0)
	{
		mkfifo("timefifo", 0666);
	}

	int df[2];
	pipe(df);

	pid_t pid = vfork();
	if(pid < 0)
		return 0;

	else if(pid == 0)
	{
		signal(SIGPIPE, fun1);
		sleep(2);
		close(df[0]);
        dup2(df[1], 1);
		execl("/usr/bin/mplayer", "mplayer", "-slave", "-quiet", "-input", "file=/home/chenbiao/mywork/4.5/fifo", ChuSi->musicname, NULL);
	    close(df[1]);

	    exit(12);
	}

	else
	{
		PlayChusimusic(chuSi);
        pid_t pid = fork();
        if(pid < 0)
        	return 0;
        //接受mplayer传回的信息
        else if(pid == 0)
        {
        	void *addr = shmat(id, NULL, 0);
        	PPZWJ Chusi = (PPZWJ)addr;
        	Chusi->musicnumber = list->data;
        	close(df[1]);
        	char MSG[40];
        	char *p;
        	int set;
        	char length[10] = {0};
        	int fd1 = open("timefifo", O_WRONLY);
        	
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
                	    printf("歌曲总时长为:%s, %d\n", p+1, Chusi->OUTDO);
                	    strcpy(length, p+1);
                	    Chusi->OUTDO = 0;
                	}
                }
                memset(MSG, 0, sizeof(MSG));
                //printf("11111\n");
                continue;
                }
                if((p = strchr(MSG, '=')) != NULL)
                {
                	//printf("%s\n", p+1);
                	//printf("%s\n", length);
                	Chusi->time = atoi(p+1);
                    write(fd1, p+1, strlen(p+1));
                    if(atoi(p+1) > (atoi(length)-5))
                	{
                		Chusi->OUTDO = 1;
                        Chusi->IRCDO = 1;
                		char cmd[100] = {0};
						char path[80] = {0};
						if(Chusi->playmode == 2)
						{
                            Chusi->music_id = rand()%(Chusi->musicnumber);
						}
						else
						{
							Chusi->music_id = (Chusi->music_id)+(Chusi->playmode);
						}
						FindMusicname(list, path, &(Chusi->music_id));
						printf("%d\n", Chusi->music_id);
						sprintf(cmd, "loadfile %s\n", path);
						printf("%s\n", path);
						strcpy(Chusi->musicname, path);
						set = strlen(cmd);
						Sendcmd(cmd, set);
						//sleep(1);
						
                	}
                }
        		memset(MSG, 0, sizeof(MSG));
        		
        	}
        	
        	close(df[0]);
        	close(fd1);
        	shmdt(addr);

             
        }	

        else
        {
 
        	int set;
        	pthread_t tid;
			pthread_create(&tid, NULL, GetmusicMSG, chuSi);

			pthread_t tid1;
			pthread_create(&tid1, NULL, PlayLyrics, chuSi);
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
				case 0:
				{
					printf("can not understand\n");
					printf("please input help\n");
				}
				break;
				case 27:
				{
                    Help();
				}
				break;
				case 1:
				{
					--(chuSi->music_id);
                    FindMusicname(list, path, &(chuSi->music_id));
					sprintf(cmd, "loadfile %s\n", path);
					printf("%s\n", path);
					strcpy(chuSi->musicname, path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
					chuSi->IRCDO = 1;
				}
				break;
				case 2:
				{
					chuSi->music_id = atoi(str);
					FindMusicname(list, path, &(chuSi->music_id));
					sprintf(cmd, "loadfile %s\n", path);
					printf("%s\n", path);
					strcpy(chuSi->musicname, path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
					chuSi->IRCDO = 1;
				}
				break;
				case 3:
				{
					++(chuSi->music_id);
					FindMusicname(list, path, &(chuSi->music_id));
					sprintf(cmd, "loadfile %s\n", path);
					printf("%s\n", path);
					strcpy(chuSi->musicname, path);
					set = strlen(cmd);
					Sendcmd(cmd, set);
					chuSi->OUTDO = 1;
					chuSi->IRCDO = 1;
				}
				break;
				case 4:
				{
					set = strlen(buff);
                    Sendcmd(buff, set);
				}
				break;
				case 25:
				{
					set = strlen(buff);
                    Sendcmd(buff, set);
				}
				break;
				case 26:
				{
					set = strlen(buff);
                    Sendcmd(buff, set);
				}
				break;
				case 5:
				{
					if(atoi(str) == 0)
						chuSi->playmode = MODE1;
					else if(atoi(str) == 1)
						chuSi->playmode = MODE2;
					else if(atoi(str) == 2)
						chuSi->playmode = MODE3;
					else
						printf("无效指令，只有0，1，2三种模式\n");
				}
				break;
				case 28:
				{
					if(atoi(str) == 0)
					{
						LRCIO = 0;
						pthread_cond_signal(&cond);
					}
					else if(atoi(str) == 1)
						LRCIO = 1;
				}
				break;
				case 6:
				{
					chuSi->OnOff = 0;
					sleep(1);
                    Savepzwj(chuSi);
					printf("save success\n");
					close(df[0]);
					Sendcmd("q\n", 2);
					SaveGedang(list);
					ClearGedang(list);
					free(list);
					list = NULL;
					shmctl(id, IPC_RMID, NULL);
					return 0;
					
				}
				break;
				case 7:
				{

					printf("MODE = %d\n", chuSi->playmode);
					printf("music_id = %d\n", chuSi->music_id);
					printf("musicname = %s\n", chuSi->musicname);
					printf("目前歌曲播放时间为：time = %ld\n", chuSi->time);
					//printf("OUTDO = %d\n", chuSi->OUTDO);
					printf("LRCIO = %d\n", LRCIO);
				}
				break;
				case 8:Help();
				break;
				case 9:DisplayGedang(list);
				break;
				
				case 44:
				{
					PNODE head = DownloadIrc(str);
					printf("insert sucess!\n");
					printf("press a key to go on\n");
					getchar();
					long time = 0;
					while(time <= head->piror->time)
					{
	   					char *p = FindList(head, time);
	    				if(p != NULL)
	    				printf("%s\n", p);
	    				usleep(10000);
	    				time+=10;
					}
					displaylist(head);
				}
				break;
				case 21:InsertGedang(list, str);
				break;
				case 11:ClearGedang(list);
				break;
				case 22:InsertGdDIR(str, list);
				break;
				case 23:
				{
					int a;
					sscanf(str, "%d", &a);
					DelectGedang(list, a);
				}
				break;
				case 30:
				{
					if(atoi(str) == 1)
					{
						chuSi->PAUSE = 1;
						sleep(1);
						Sendcmd("pause\n", 6);
					}
					else
						chuSi->PAUSE = 0;
				}	
		    }
		    
		}
        }
		
	}
	return 0;
}