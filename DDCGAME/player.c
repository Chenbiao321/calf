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
#include <fcntl.h>
#include <stdio.h>

void *screen(void *arg)
{
    
	char buf[35][50] = {0};
	struct sockaddr_in saad;
	int len = sizeof(saad);
	int ret;
	while((ret = recvfrom(*(int *)arg, buf, sizeof(buf), 0, (struct sockaddr *)&saad, &len)) > 0)
	{
        for(int i = 0;i < 35;i++)
		{
			write(1, buf[i], 50);
			printf("\n");
		}
	}
}


void *fun(void *arg)
{
	pthread_detach(pthread_self());
	char buf[100] = {0};
	struct sockaddr_in saad;
	int len = sizeof(saad);
	int ret;
	while((ret = recvfrom(*(int *)arg, buf, sizeof(buf), 0, (struct sockaddr *)&saad, &len)) > 0)
	{
		if(strcmp(buf, "1") == 0)
		{
			pthread_t tid1;
			pthread_create(&tid1, NULL, screen, arg);
			break;
		}
		write(1, buf, ret);
		printf("\n");
		memset(buf, 0 ,sizeof(buf));
	}
}

int main()
{
	int flag = fcntl(0, F_GETFL);
	flag = flag&(~O_NONBLOCK);
	//flag = flag|O_NONBLOCK;
	if(flag&O_NONBLOCK)
		printf("O_NONBLOCK\n");
	fcntl(0, F_SETFL, flag);

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.0.122", &addr.sin_addr);

	char buff[20];
	int set;
	pthread_t tid;
	pthread_create(&tid, NULL, fun, &sock);
	while((set = read(0, buff, sizeof(buff))) >= 0)
	{
		buff[strlen(buff)-1] = '\0';
		sendto(sock, buff, set-1, 0 ,(struct sockaddr *)&addr, sizeof(addr));
		memset(buff, 0, sizeof(buff));
	}
}