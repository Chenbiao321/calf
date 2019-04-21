#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include "bcbc.h"

int i;

int ChackGdang(PLIST list, char *path)
{
	PLIST p = list->next;
	while(p != list)
	{
		if(strcmp(p->path, path) == 0)
			return 1;
		else
			p = p->next;
	}
	return 0;
}

void InsertGedang(PLIST list, char *path)
{
	if(ChackGdang(list, path))
	{
		printf("已经存在\n");
		return;
	}
	PLIST pnew = (PLIST)malloc(sizeof(LIST));
	assert(pnew != NULL);

	strcpy(pnew->path, path);
	pnew->data = i;
	++i;

	pnew->piror = list->piror;
	list->piror->next = pnew;
	pnew->next = list;
	list->piror = pnew;

}


PLIST DownloadGedang()
{
	FILE *fd = fopen("/home/chenbiao/mywork/4.5/Gedang.txt", "r");
	if(fd == NULL)
		return NULL;
	char buff[80];
	PLIST list = CreateGedang();

	while(fgets(buff, 80, fd) != NULL)
	{
		buff[strlen(buff)-1] = '\0';
		InsertGedang(list, buff);
	}

	fclose(fd);

	return list;
}

PLIST CreateGedang()
{
	PLIST list = (PLIST)malloc(sizeof(LIST));
	assert(list != NULL);

    memset(list->path, 0, sizeof(list->path));

	list->piror = list;
	list->next = list; 
    
    return list;
}