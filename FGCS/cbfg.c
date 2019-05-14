#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "cbfg.h"

long ccd;

long OnOff;

int i = 0;

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	strcpy(head->key, "wwwwww");
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

char *FindList(PNODE head, const char *key)
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

PNODE CopyList(PNODE pnew)
{
	PNODE p = CreateList();

	InsertList(p, "pack_id", FindList(pnew, "pack_id"));
	InsertList(p, "MAC", FindList(pnew, "MAC"));
	InsertList(p ,"IP", FindList(pnew, "IP"));

	return p;
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
	BANANA *banana = (BANANA *)arg;
    PLIST list = banana->list;
    PINET inet = banana->inet;

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
  		char extra[200] = {0};
        PNODE pnew = RecvpackMsg(buff, &cmd, set, extra);
        if((cmd != 3)&&(cmd != 1025))
        {
        	PNODE p = FindNode(list, ip);
        	if(p == NULL)
        	{
        		perror("not find ip\n");
        		continue;
        	}
        	ModifyList(p, "pack_id", FindList(pnew, "pack_id"));
        	ClearList(pnew);
        	free(pnew);
        	pnew = p;

        	if(pnew == NULL)
        	{
        		perror("stranger message\n");
                continue;
        	}
        }
        if(cmd == 1025)
        {
            Sendcmd(&sddr, 3, " ", list->next->head, &sock1);
            //printf("return success\n");
			ClearList(pnew);
			free(pnew);
			pnew = NULL;
            sleep(1);    
        }
        else if(cmd == 3)
        {
            if(FindNode(list, ip) != NULL)
            {
            	ChangeNode(list, ip, pnew);
            }
            else
            {
            	InsertNode(list, ip, pnew);
            }
        }
        else if(cmd == 2097440)
        {
        	char *p;
        	char *q;
        	if((p = strchr(extra, ':')) == NULL)
        	{
        		perror("error\n");
        		break;
        	}
        	*p = '\0';
            q = p+1;
            char filename[20] = {0};
            int size;
            if((p = strchr(q, ':')) == NULL)
        	{
        		perror("error\n");
        		break;
        	}
            *p = '\0';
            strcpy(filename, q);
            q = p+1;
           
            p = strchr(q, ':');
            *p = '\0';
            sscanf(q, "%d", &size);
            q = p+1;
            

        	printf("recv:%s(%s):name:%s:szie:%d\n", FindList(pnew, "Nc"), FindList(pnew, "IP"), filename, size);
            long t;
            sscanf(FindList(pnew, "pack_id"), "%ld", &t);
            InsertInet(inet, t, filename, size, FindList(pnew, "IP"));

        	ORANGE SET;
        	
        	PNODE copy = CopyList(pnew);

            SET.inet = inet;
        	SET.pnew = copy;
        	SET.list = list;
        	SET.sock = sock1;
            pthread_t tid;
            pthread_create(&tid, NULL, Recvfile, &SET);
            
        }
        else if(cmd == 487)
        {
        	Sendcmd(&sddr, 33, FindList(pnew, "pack_id"), list->next->head, &sock1);
			
        }
        else if(cmd == 288)
        {
        	//printf("%s\n", FindList(pnew, "Nc"));
            printf("%s(%s):%s\n", FindList(pnew, "Nc"), ip, extra);
            Sendcmd(&sddr, 33, FindList(pnew, "pack_id"), list->next->head, &sock1);
			
        }
        else if(cmd == 33)
        {
        	//printf("recv:33 PackMsg\n");
        	OnOff = atoi(extra);
        	//printf("%ld\n", OnOff);

        }
        else
        {
        	//perror("cmd error\n"); 
        }
        memset(buff, 0 ,sizeof(buff));
        memset(extra, 0, sizeof(extra));
   	}
}


PNODE RecvpackMsg(char *buff, int *cmd, int set, char *extra)
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
	if((*cmd == 3)||(*cmd == 1025))
	{
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
	}

	else if(*cmd == 288)
	{
		p = strchr(q, '[');;
		*p = '\0';
		strcpy(extra, q);
	}
	else if(*cmd == 2097440)
	{
		strcpy(extra, q+1);
	}
	else
	{
		strcpy(extra, q);
	}

    return head;
}



int PackMsg(char *message, PNODE head, int cmd, char *extra)
{
	int len;
	
	if((cmd == 1025)||(cmd == 3))
	{
    	len = sprintf(message, "%s:%ld:%s:%s:%d:%s%c%s%c%s%c%c%s%c%s%c%c5%c%s%c%c%s%c10000001%c%s%c%s", 
    	FindList(head, "Bb"), num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, FindList(head, "Nc"), 0, FindList(head, "Qz"), 0, FindList(head, "MAC"), 
    		0, 0, FindList(head, "Ph"), 0, FindList(head, "Em"), 0, 0, 0, FindList(head, "IP"), 
    		0, 0, FindList(head, "pix"), 0, 0, FindList(head, "Gx"), 0, FindList(head, "DESKTOP_TI6DHJ5"));
    }
    else if(cmd == 288)
    {
    	len = sprintf(message, "%s:%ld:%s:%s:%d:%s[rich]0A0000000000860008AE5F6F8FC596D19E12000000000000000000000000000000000000[/rich]", 
    	FindList(head, "Bb"), num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, extra);
    }
    else if(cmd == 2097440)
    {
    	char path[80] = {0};
    	char name[20] = {0};
    	strcpy(path, extra);
    	//printf("%s\n", path);
    	char *p;
    	p = extra + strlen(extra) - 1;
    	while(*p != '/')
    	{
    		--p;
    	}
        strcpy(name, p+1);
    	struct stat buff;
    	if(stat(path, &buff))
    	{
    		perror("error\n");
    		return 0;
    	}
        
        len = sprintf(message, "%s:%ld:%s:%s:%d:%c%ld:%s:%lx:0:1:%ld:%ld:%ld:%c", 
    	FindList(head, "Bb"), num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, 0, num-1, name, buff.st_size, buff.st_atime, buff.st_mtime, buff.st_ctime, 0);
    }
    else if(cmd == 96)
    {
        len = sprintf(message, "1.1.180210:%ld:%s:%s:%d:%s", 
    	 num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, extra);
    }
    else
    {
    	if(cmd == 487)
    		ccd = num;
    	//printf("1111111111\n");
    	len = sprintf(message, "%s:%ld:%s:%s:%d:%s", 
    	FindList(head, "Bb"), num, FindList(head, "Administrator"), FindList(head, "DESKTOP_TI6DHJ5"),
    		cmd, extra);
    }

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
		Sendcmd(&addr, 1025, " ", head, &sock);
		//printf("send success\n");
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

PNODE FindNode(PLIST list, const char *ip)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(strcmp(p->ip, ip) == 0)
		{
			return p->head;
		}
		else
		{
			p = p->next;
		}
	}

	return NULL;
}

PNODE GetNodeid(PLIST list, int id)
{
	PLIST p = list->next;

	while(p != list)
	{
		if(p->id == id)
		{
			return p->head;
		}
		else
		{
			p = p->next;
		}
	}
	printf("can not fand id\n");
	return NULL;
}

void ChangeNode(PLIST list, char *ip, PNODE pnew)
{
	PLIST p = list->next;

	while(p != NULL)
	{
		if(strcmp(p->ip, ip) == 0)
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

void Help()
{
	printf("查看联系人：list\n");
	printf("查看联系人详细信息：info id\n");
	printf("发送消息：send id message\n");
	printf("修改个人信息：set *\n");
	printf("查看个人信息：watch\n");
	printf("查找个人某个信息：get *\n");
	printf("初始化个人信息：chusi\n");
	printf("查看上传列表：ulist\n");
	printf("查看下载列表：dlist\n");
	printf("指定下载文件：pull filename\n");
	printf("指定上传文件：push id 绝对路径\n");
}











int Createtcpsock()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

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
    
    listen(sock, 6);
	return sock;
}


void Sendcmd(struct sockaddr_in *addr, int cmd, char *extra, PNODE head, void *sock)
{
    int sock1 = *(int *)sock;
    
	char message[300] = {0};
    int len = PackMsg(message, head, cmd, extra);
    sendto(sock1, message, len, 0, (struct sockaddr *)addr, sizeof(*addr));
    ++num;
    char str[10] = {0};
	sprintf(str, "%ld", num);
	ModifyList(head, "pack_id", str);
}

PINET FindInet(PINET inet, long tt)
{
	PINET p = inet->next;

	while(p != inet)
	{
        if(p->pack_id == tt)
        {
        	return p;
        }
        else
        {
        	p = p->next;
        }
	}
	return NULL;
}

PINET FindInetname(PINET inet,char *filename)
{
	PINET p = inet->next;

	while(p != inet)
	{
        if(strcmp(p->filename, filename) == 0)
        {
        	return p;
        }
        else
        {
        	p = p->next;
        }
	}
	return NULL;
}


void *Recvfile(void *arg)
{    
    pthread_detach(pthread_self());
    ORANGE *SET = (ORANGE *)arg;

    PNODE pnew = SET->pnew;
    PLIST list = SET->list;
    PINET inet = SET->inet;

    int sock1 = SET->sock;

	struct sockaddr_in sddr;
	sddr.sin_family = AF_INET;
	sddr.sin_port = htons(2425);
	inet_pton(AF_INET, FindList(pnew, "IP"), &sddr.sin_addr);

	Sendcmd(&sddr, 33, FindList(pnew, "pack_id"), list->next->head, &sock1);

	long tt;
    sscanf(FindList(pnew, "pack_id"), "%ld", &tt);

    PINET node = FindInet(inet, tt);
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	
	pthread_cond_init(&node->cond, NULL);
	
	if(node == NULL)
	{
		perror("contact error\n");
		return NULL;
	}

	pthread_mutex_lock(&mutex);
	//printf("wait\n");
	pthread_cond_wait(&node->cond, &mutex);
	printf("wake up\n");
	pthread_mutex_unlock(&mutex);

    
	Sendcmd(&sddr, 487, FindList(pnew, "MAC"), list->next->head, &sock1);
	
	int t = 0;
	while(1)
	{
		if(OnOff == ccd)
		{
            int sock = socket(AF_INET, SOCK_STREAM, 0);

            int opt = 1;
            if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
            {
                perror("error5\n");
                close(sock);
                return NULL;
            }

            if(connect(sock, (struct sockaddr *)&sddr, sizeof(sddr)))
            {
            	perror("connect error\n");
            	close(sock);
            	return NULL;
            }
            printf("connect success\n");

            char extra[30] = {0};
  
            sprintf(extra, "%lx:%lx:0:", tt, tt-1);
            //printf("%s\n", extra);
            char message[300] = {0};
            int len = PackMsg(message, list->next->head, 96, extra);
    		send(sock, message, len, 0);
    		++num;
    		char str[20] = {0};
			sprintf(str, "%ld", num);
			ModifyList(list->next->head, "pack_id", str);
            
            int fd = open(node->filename, O_WRONLY | O_CREAT, 0666);
            if(fd < 0)
            {
            	perror("error 6\n");
                return NULL;
            }
            char buff[1024];
            int set;
            while((set = recv(sock, buff, sizeof(buff), 0)) >= 0)
            {
            	if(set == 0)
            	{
            		printf("download success\n");
            		DelectInet(inet, tt);
            		close(fd);
            		free(pnew);
            		pnew = NULL;

            		pthread_mutex_destroy(&mutex);
            		pthread_cond_destroy(&node->cond);
            		pthread_exit(NULL);
            	}
            	write(fd, buff, set);
            }

		}
		else
		{
			sleep(1);
		}
		++t;
		if(t > 10)
		{
			printf("no reply\n");
			free(pnew);
			pnew = NULL;
			pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&node->cond);
			break;
		}
	}
} 

void *TCPserve(void *arg)
{
	PINET p = (PINET)arg;

	APPLE apple;

	int sock2 = Createtcpsock();
	int fsock;
	struct sockaddr_in rddr;
	int len = sizeof(rddr);
	int set;
	int epfd = epoll_create(1024);
	struct epoll_event temp;
	temp.data.fd = sock2;
	temp.events = EPOLLIN | EPOLLPRI;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock2, &temp);
	struct epoll_event efd[30] = {0};

	while((set = epoll_wait(epfd, efd, 30, -1)) >= 0)
	{
		for(int i = 0;i < set;i++)
		{
			if(efd[i].data.fd == sock2)
			{
                fsock = accept(sock2, (struct sockaddr *)&rddr, &len);
                temp.data.fd = fsock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, fsock, &temp);
			}
			else
			{
				apple.inet = p;
				apple.sock = efd[i].data.fd;
				pthread_t tid;
				pthread_create(&tid, NULL, Downloadfile, &apple);
				epoll_ctl(epfd, EPOLL_CTL_DEL, efd[i].data.fd, &temp);
			}
		}
	}

}

void BigtoLittle(char *extra)
{
	while(*extra)
	{
		if((*extra >= 65)&&(*extra <= 90))
		{
			*extra = *extra + 32;
			++extra;
		}
		else
			++extra;
	}
}

void *Downloadfile(void *arg)
{
	pthread_detach(pthread_self());

	APPLE *apple = (APPLE *)arg;

	int sock = apple->sock;
    char buff[300] = {0};
    char extra[200] = {0};
    int set;
    int cmd;


	set = recv(sock, buff, sizeof(buff), 0);
    PNODE pnew = RecvpackMsg(buff, &cmd, set, extra);
    char *p = strchr(extra, ':');
    *p = '\0';
    BigtoLittle(extra);
    long tt;
    sscanf(extra, "%lx", &tt);

    int fd = open(Getfilename(apple->inet, tt), O_RDONLY);
    if(fd < 0)
    {
    	perror("error 7\n");
    	close(fd);
    	return NULL;
    }
    int ret;
    printf("openfile success\n");
    char message[1024];
    while((ret = read(fd, message, sizeof(message))) > 0)
    {
    	send(sock, message, ret, 0);
    }
    printf("filesend success\n");
    DelectInet(apple->inet, tt);
    close(fd);
    close(sock);
    return NULL;	
}

char *Getfilename(PINET inet, const long tt)
{
	PINET p = inet->next;   
      
	while(p != inet)
	{
        //printf("%ld, %ld\n", p->pack_id, tt); 
		if(p->pack_id == tt)
		{
			
			//printf("%s\n", p->filename);
            return p->filename;
		}
		else
		{
			p = p->next;
		}
	}
	return NULL;
}

PINET CreateInet()
{
	PINET inet = (PINET)malloc(sizeof(INET));
	assert(inet != NULL);

	inet->pack_id = 9999;
	strcpy(inet->filename, "FFFF");
	inet->size = 7777;
	strcpy(inet->ip, "0.0.0.0");

	inet->piror = inet;
	inet->next = inet;

	return inet;
}

void InsertInet(PINET inet, long pack_id, char *filename, int size, char *ip)
{
	PINET pnew = (PINET)malloc(sizeof(INET));
	assert(pnew != NULL);

	pnew->pack_id = pack_id;
	strcpy(pnew->filename, filename);
	pnew->size = size;
    strcpy(pnew->ip, ip);

	pnew->piror = inet->piror;
	inet->piror->next = pnew;
	pnew->next = inet;
	inet->piror = pnew;
}

void DisplayInet(PINET inet)
{
	PINET p = inet->next;

	while(p != inet)
	{
		printf("(%lx),filename:%s, size:%d, ip:(%s)\n", p->pack_id, p->filename, p->size, p->ip);
		p = p->next;
	}
}

void DelectInet(PINET inet, long tt)
{
	PINET p = inet->next;
	while(p != inet)
	{
		if(p->pack_id == tt)
		{
			p->piror->next = p->next;
			p->next->piror = p->piror;
			free(p);
			break;
		}
		else
		{
			p = p->next;
		}
	}
}







void SendChatmsg(PNODE INET, PLIST list, char *p, void *sock)
{
	int sock2 = *(int *)sock;

	struct sockaddr_in sddr;
	sddr.sin_family = AF_INET;
	sddr.sin_port = htons(2425);
	inet_pton(AF_INET, FindList(INET, "IP"), &sddr.sin_addr);
					
    Sendcmd(&sddr, 487, FindList(INET, "MAC"), list->next->head, &sock2);

	Sendcmd(&sddr, 288, p+1, list->next->head, &sock2);
}
					