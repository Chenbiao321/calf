//歌词播放器模拟
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "bbbc.h"

int analyse(char *buff, char *str)
{
	char *p;
	if((p = strchr(buff, ' ')) != NULL)
	{
		*p = '\0';
		strcpy(str, p+1);
		if(strcmp(buff, "addmusic") == 0)
			return 4;
		else if(strcmp(buff, "addmusicdir") == 0)
			return 6;
		else if(strcmp(buff, "delmusic") == 0)
			return 7;
		else if(strcmp(buff, "play") == 0)
			return 3;
	}
	else if(strcmp(buff, "help") == 0)
		return 0;
	else if(strcmp(buff, "list") == 0)
		return 1;
	else if(strcmp(buff, "quit") == 0)
		return 2;
	else if(strcmp(buff, "clear") == 0)
		return 5;
	else
		printf("无效指令\n");
}



int main(int argc, char **argv)
{
	PLIST list = DownloadGedang();
	printf("insert success\n");
	printf("wlecome use mplayer!\n");
	printf("please input command\n");
	char buff[100];
	char str[80];
	
	while(1)
	{
		gets(buff);

		switch(analyse(buff, str))
		{
			case 0:Help();
			break;
			case 1:DisplayGedang(list);
			break;
			case 2:
			{
				SaveGedang(list);
				ClearGedang(list);
				free(list);
				list = NULL;
				return 0;
			}
			break;
			case 3:
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
			case 4:InsertGedang(list, str);
			break;
			case 5:ClearGedang(list);
			break;
			case 6:InsertGdDIR(str, list);
			break;
			case 7:
			{
				int a;
				sscanf(str, "%d", &a);
				DelectGedang(list, a);
			}
			break;	
		}
	}

	
	return 0;
}


