#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "cbfg.h"

int num = 80000;

extern char **environ;

int analyse(char *buff, char *str)
{
	char *p;
	if((p = strchr(buff, ' ')) != NULL)
	{
		*p = '\0';
		strcpy(str, p+1);
		if(strcmp(buff, "add") == 0)
		{
			return 1;
		}
		else if(strcmp(buff, "set") == 0)
		{
			return 2;
		}
		else if(strcmp(buff, "get") == 0)
		{
			return 3;
		}
		else if(strcmp(buff, "info") == 0)
		{
			return 8;
		}
	}
	else if(strcmp(buff, "watch") == 0)
	{
		return 4;
	}
	else if(strcmp(buff, "quit") == 0)
	{
		return 5;
	}
	else if(strcmp(buff, "chusi") == 0)
	{
		return 6;
	}
	else if(strcmp(buff, "list") == 0)
	{
		return 7;
	}
	else
		return -1;
}






int main()
{
	PNODE head = Downloadpzwj();
	
    ModifyList(head, "DESKTOP_TI6DHJ5", getenv("DESKTOP_SESSION"));
    ModifyList(head, "Administrator", getenv("LOGNAME"));

	char buff[100] = {0};
	char str[80] = {0};    

    pthread_t tid;
    pthread_create(&tid, NULL, SendMsg, head);

    PLIST list = CreateNode();

    InsertNode(list, FindList(head, "IP"), head);

    pthread_t tid1;
    pthread_create(&tid1, NULL, UDPserve, list);

	while(1)
	{
		read(0, buff, sizeof(buff));
		printf("%s", buff);
		buff[strlen(buff)-1] = '\0';
		switch(analyse(buff, str))
		{
			case 1:
			{
				char *p;
				if((p = strchr(str,' ')) != NULL)
				{
					*p = '\0';
					InsertList(head, str, p+1);
					Savepzwj(head);
				}
				else
				{
					printf("please watch help\n");
				}
			}
			break;
			case 2:
			{
				char *p;
				if((p = strchr(str,' ')) != NULL)
				{
					*p = '\0';
					ModifyList(head, str, p+1);
					Savepzwj(head);
				}
				else
				{
					printf("please watch help\n");
				}
			}
			break;
			case 3:
			{
				printf("%s\n", FindList(head, str));
			}
			break;
			case 4:
			{
				DisplayList(head);
			}
			break;
			case 5:
			{
				Savepzwj(head);
				printf("save success\n");
				return 0;
			}
			break;
			case 6:
			{
				Chusipzwj();
				ClearList(head);
				free(head);
				head = Downloadpzwj();
				ModifyList(head, "DESKTOP_TI6DHJ5", getenv("DESKTOP_SESSION"));
    			ModifyList(head, "Administrator", getenv("LOGNAME"));
				printf("pzwj.txt have Chisihua\n");
			}
			break;
			case 7:
			{
				DisplayNode(list);
			}
			break;
			case 8:
			{
				DisplayNodeList(list, atoi(str));
			}
		}
		memset(buff, 0, sizeof(buff));
	}
}