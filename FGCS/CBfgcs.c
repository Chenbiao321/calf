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

long num = 80000;

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
		else if(strcmp(buff, "send") == 0)
		{
			return 10;
		}
		else if(strcmp(buff, "push") == 0)
		{
			return 11;
		}
		else if(strcmp(buff, "pull") == 0)
		{
			return 16;
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
	else if(strcmp(buff, "which") == 0)
	{
		return 15;
	}
	else if(strcmp(buff, "list") == 0)
	{
		return 7;
	}
	else if(strcmp(buff, "help") == 0)
	{
		return 9;
	}
	else if(strcmp(buff, "ulist") == 0)
	{
		return 12;
	}
	else if(strcmp(buff, "dlist") == 0)
	{
		return 13;
	}
	else
		return -1;
}






int main()
{
	int sock2 = Createsock();

	PNODE head = Downloadpzwj();
	
    ModifyList(head, "DESKTOP_TI6DHJ5", getenv("DESKTOP_SESSION"));
    ModifyList(head, "Administrator", getenv("LOGNAME"));

	char buff[300] = {0};
	char str[200] = {0};    
    
    pthread_t tid;
    pthread_create(&tid, NULL, SendMsg, head);

    PLIST list = CreateNode();
    PINET finet = CreateInet();
    BANANA banana;
    banana.inet = finet;
    banana.list = list;

    InsertNode(list, FindList(head, "IP"), head);

    pthread_t tid1;
    pthread_create(&tid1, NULL, UDPserve, &banana);

    PINET inet = CreateInet();

    pthread_t tid2;
    pthread_create(&tid2, NULL, TCPserve, inet);

	while(1)
	{
		read(0, buff, sizeof(buff));
		//printf("%s", buff);
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
			break;
			case 10:
			{
				char *p;
				if((p = strchr(str, ' ')) != NULL)
				{
					*p = '\0';

					PNODE INET = GetNodeid(list, atoi(str));

					if(INET == NULL)
						continue;

					SendChatmsg(INET, list, p, &sock2);

					
				}
				else
				{
					printf("please watch help\n");
				}				
			}
			break;
			case 16:
			{
 
				PINET INET = FindInetname(finet, str);

				if(INET == NULL)
				{
					memset(buff, 0, sizeof(buff));
					continue;
				}

				pthread_cond_signal(&INET->cond);
				printf("start download\n");
			}
			break;			
			case 11:
			{

				char *p;
				if((p = strchr(str, ' ')) != NULL)
				{
					*p = '\0';
					struct stat buf;
    				stat(p+1, &buf);
					
					printf("%ld\n", num);

					PNODE INET = GetNodeid(list, atoi(str));
					InsertInet(inet, num, p+1, buf.st_size, FindList(INET, "IP"));

					if(INET == NULL)
					{
						memset(buff, 0, sizeof(buff));
						continue;
					}

					struct sockaddr_in sddr;
					sddr.sin_family = AF_INET;
					sddr.sin_port = htons(2425);
					inet_pton(AF_INET, FindList(INET, "IP"), &sddr.sin_addr);
					
                	Sendcmd(&sddr, 2097440, p+1, list->next->head, &sock2);
				}

			}
			break;
			case 15:
			{
				printf("ccd:%ld, OnOff:%ld\n", ccd, OnOff);
			}
			break;
			case 12:
			{
				DisplayInet(inet);
			}
			break;
			case 13:
			{
				DisplayInet(finet);
			}
			break;
			case 9:
			{
				Help();
			}
			break;
			case -1:
			{
				printf("please input help\n");
			}
		}
		memset(buff, 0, sizeof(buff));
	}
}