//歌词播放器模拟
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "bbbc.h"

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("请指定歌词文件\n");
		return 0;
	}
	PNODE head = DownloadIrc(argv[1]);


	printf("insert sucess!\n");
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


	return 0;
}