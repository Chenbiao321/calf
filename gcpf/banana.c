//歌词播放器模拟
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "bbbc.h"

int main()
{
	PNODE head = DownloadIrc();

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