#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "cbfg.h"

int i = 0;

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	strcpy(head->key, "name");
	strcpy(head->value, "chenbiao");

	head->next = head;
	head->piror = head;

	return head;
}

void InsertList(PNODE head, char *key, char *value)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	strcpy(pnew->key, key);
	strcpy(pnew->value, value);
    
    pnew->piror = head->piror;
    head->piror->next = pnew;
    pnew->next = head;
    head->piror = pnew;
}

void ModifyList(PNODE head, char *key, char *value)
{
	PNODE p = head->next;

	while(p != head)
	{
		if(strcmp(p->key, key) == 0)
		{
			strcpy(p->value, value);
			break;
		}
		else
		{
			p = p->next;
		}
	}
}

char *FindList(PNODE head, char *key)
{
	PNODE p = head->next;

	while(p != head)
	{
		if(strcmp(p->key, key) == 0)
		{
			return p->value;
		}
		else
		{
			p = p->next;
		}
	}
}

void ClearList(PNODE head)
{
	PNODE p = head->next;

	if(p != head)
	{
		p->next->piror = head;
		head->next = p->next;
		free(p);
		ClearList(head);
	}
}

void DisplayList(PNODE head)
{
	PNODE p = head->next;

	while(p != head)
	{
		printf("%s=%s\n", p->key, p->value);
		p = p->next;
	}
}

PNODE Downloadpzwj()
{
	PNODE head = CreateList();
	FILE *fd = fopen("pzwj.txt", "r");
	if(fd == NULL)
	{
		Chusipzwj();
		printf("pzwj.txt have Chisihua\n");
	}
	fd = fopen("pzwj.txt", "r");
    
    char buff[100] = {0};

	while(fgets(buff, sizeof(buff), fd) > 0)
	{
		buff[strlen(buff)-1] = '\0';
		char *p = strchr(buff, '=');
		*p = '\0';
		InsertList(head, buff, p+1);
	}

	fclose(fd);
	return head;
}

void Savepzwj(PNODE head)
{
	PNODE p = head->next;

	FILE *fd = fopen("pzwj.txt", "w");
	while(p != head)
	{
		fprintf(fd, "%s=%s\n", p->key, p->value);
		p = p->next;
	}

	fclose(fd);
}

void Chusipzwj()
{
	FILE *fd = fopen("pzwj.txt", "w");
	fprintf(fd, "Bb=5.1.182010\n");
	fprintf(fd, "pack_id=num\n");
	fprintf(fd, "Administrator=LOGNAME\n");
	fprintf(fd, "DESKTOP_TI6DHJ5=linux\n");
	fprintf(fd, "Nc=333333333\n");
	fprintf(fd, "Qz=555555555\n");
	fprintf(fd, "MAC=00:26:9e:f6:c3:17\n");
	fprintf(fd, "Ph=000000000\n");
	fprintf(fd, "Em=111111111\n");
	fprintf(fd, "IP=192.168.1.110\n");
    
	
	fprintf(fd, "pix=1\n");
	
	fprintf(fd, "Gx=222222222\n");
	
	
	

	fclose(fd);
}






















int Createservesock()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	int opt = 1;

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("error\n");
		close(sock);
		return -5;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2425);
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)))
	{
		perror("error\n");
		close(sock);
		return -2;
	}

	return sock;
}


int Createsock()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

	int opt = 1;

	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)))
	{
		perror("error\n");
		close(sock);
		return -1;
	}

	return sock;
}


void *UDPserve(void *arg)
{
	PLIST list = (PLIST)arg;

    int sock1 = Createservesock();

    int set;
    char buff[300] = {0};
    struct sockaddr_in rddr;
    int len = sizeof(rddr);
    
    while((set = recvfrom(sock1, buff, sizeof(buff), 0, (struct sockaddr *)&rddr, &len)) > 0)
  	{
        char ip[20];
        int port;
        inet_ntop(AF_INET, &rddr.sin_addr, ip, sizeof(ip));
        port = ntohs(rddr.sin_port);

        struct sockaddr_in sddr;
		sddr.sin_family = AF_INET;
		sddr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &sddr.sin_addr);


  		int cmd;
        PNODE pnew = RecvpackMsg(buff, &cmd, set);
        if(cmd == 1025)
        {
            char message[300] = {0};
            int len = PackMsg(message, list->next->head, 3);
			sendto(sock1, message, len, 0, (struct sockaddr *)&sddr, sizeof(sddr));
			printf("return success\n");
			++num;
			char str[10] = {0};
			sprintf(str, "%d", num);
			ModifyList(list->next->head, "pack_id", str);
            sleep(1);    
        }
        else if(cmd == 3)
        {
            if(FindNode(list, ip))
            {
            	ChangeNode(list, ip, pnew);
            }
            else
            {
            	InsertNode(list, ip, pnew);
            }
        }
        else
        {
        	perror("cmd error\n");   
        }
        memset(buff, 0 ,sizeof(buff));
   	}
}


PNODE RecvpackMsg(char *buff, int *cmd, int set)
{
	buff[set] = '\0';
	PNODE head = CreateList();
	char *p;
	char *q = buff;
	if((p = strchr(q, ':')) != NULL)
	{
		*p = '\0';
        InsertList(head, "Bb", q);
        q = p+1;
	}
	if((p = strchr(q, ':')) != NULL)
	{
		*p = '\0';
		InsertList(head, "pack_id", q);
        q = p+1;
	}
	if((p = strchr(q, ':')) != NULL)
	{
		*p = '\0';
        InsertList(head, "Administrator", q);
        q = p+1;
	}
	if((p = strchr(q, ':')) != NULL)
	{
		*p = '\0';
        InsertList(head, "DESKTOP_TI6DHJ5", q);
        q = p+1;
	}
	if((p = strchr(q, ':')) != NULL)
	{
		*p = '\0';
        *cmd = atoi(q);
        q = p+1;
	}
	p = q + strlen(q);
	InsertList(head, "Nc", q);
	q = p+1;
	p = q + strlen(q);
	InsertList(head, "Qz", q);
	q = p+1;
	p = q + strlen(q);
	InsertList(head, "MAC", q);
	q = p+2;
	p = q + strlen(q);
	InsertList(head, "Ph", q);
	q = p+1;
	p = q + strlen(q);
	InsertList(head, "Em", q);
	q = p+4;
	p = q + strlen(q);
	InsertList(head, "IP", q);
	q = p+2;
	p = q + strlen(q);
	InsertList(head, "pix", q);
	q = p+10;
	p = q + strlen(q);
	InsertList(head, "Gx", q);
	q = p+1;

    return head;
}



int PackMsg(char *message, PNODE head, int cmd)
{
	int len;
    len = sprintf(message, "%s:%d:%s:%s:%d:%s%c%s%c%s%c%c%s%c%s%c%c5%c%s%c%c%s%c10000001%c%s%c%s", 
    	FindList(head, "Bb"), num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, FindList(head, "Nc"), 0, FindList(head, "Qz"), 0, FindList(head, "MAC"), 
    		0, 0, FindList(head, "Ph"), 0, FindList(head, "Em"), 0, 0, 0, FindList(head, "IP"), 
    		0, 0, FindList(head, "Pix"), 0, 0, FindList(head, "Gx"), 0, FindList(head, "DESKTOP_TI6DHJ5"));
    

    return len;
}


void *SendMsg(void *arg)
{
    PNODE head = (PNODE)arg;

    int sock = Createsock();

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2425);
	inet_pton(AF_INET, "192.168.1.255", &addr.sin_addr);

	char message[300] = {0};
	while(1)
	{
		int len = PackMsg(message, head, 1025);
		sendto(sock, message, len, 0, (struct sockaddr *)&addr, sizeof(addr));
		printf("send success\n");
		++num;
		char str[10] = {0};
		sprintf(str, "%d", num);
		ModifyList(head, "pack_id", str);
		sleep(10);      
	}
}













PLIST CreateNode()
{
	PLIST list = (PLIST)malloc(sizeof(LIST));
	assert(list != NULL);

	strcpy(list->ip, "192.168.1.110");
	list->id = 9999;
	list->head = NULL;

	list->piror = list;
	list->next = list;

	return list;
}

void InsertNode(PLIST list, char *ip, PNODE head)
{
	PLIST pnew = (PLIST)malloc(sizeof(LIST));
	assert(pnew != NULL);

	strcpy(pnew->ip, ip);
	pnew->id = i;
	pnew->head = head;
	i++;

	pnew->piror = list->piror;
	list->piror->next = pnew;
	pnew->next = list;
	list->piror = pnew;
}

int FindNode(PLIST list, char *ip)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(strcmp(p->ip, ip) == 0)
		{
			return 1;
		}
		else
		{
			p = p->next;
		}
	}

	return 0;
}

void ChangeNode(PLIST list, char *ip, PNODE pnew)
{
	PLIST p = list->next;

	while(p != NULL)
	{
		if(strcpy(p->ip, ip) == 0)
		{
			ClearList(p->head);
			free(p->head);
			p->head = pnew;
			break;
		}
		else
		{
			p = p->next;
		}
	}
}

void DisplayNode(PLIST list)
{
	PLIST p = list->next;

	while(p != list)
	{
		printf("%d, %s\n", p->id, p->ip);
		p = p->next;
	}
}

void DisplayNodeList(PLIST list, int id)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(p->id == id)
		{
			DisplayList(p->head);
			break;
		}
		else
		{
			p = p->next;
		}
	}
}