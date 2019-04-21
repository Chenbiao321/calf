#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <sys/types.h>
#include "bbbc.h"

int i;

long Conversion(char *time)
{
	long t;
	long a, b, c;
	char *p;
	*(strchr(time, ':')) = ' ';
	if((p = strchr(time, '.')) != NULL)
		*p = ' ';
	else
		strcat(time, " 0");
	sscanf(time, "%ld%ld%ld", &a, &b, &c);
	t = c*10+b*1000+a*60*1000;
	return t;
}

PNODE DownloadIrc(char *bc)
{
	FILE *fp = fopen(bc, "r");
	if(fp == NULL)
	return 0;
    PNODE head = CreateList();
    char TIME[11];
    char lyrics[60];
    char buff[150];
    int i;
    fgets(buff, 150, fp);
    char *lp = strchr(buff, ']');

    while(*(lp+1) == '\r' || *(lp+1) == '\n')
    {
    	printf("%s", buff);
    	fgets(buff, 150, fp);
        lp = strchr(buff, ']');
    }
    fseek(fp, -(strlen(buff)), SEEK_CUR);
    while(fgets(buff, 150, fp) != NULL)
    {
    	if(buff[0] != '[')
    		continue;
    	char *pf = buff;
    	buff[strlen(buff) - 1] = '\0';
    	char *p;
    	while((p = strchr(buff, ']')) != NULL)
    	{
    		*p = '*';
    		if(*(p+1) != '[')
    			break;
    	}
    	strcpy(lyrics, p+1);
    	while((p = strchr(pf, '*')) != NULL)
    	{
    		*p = '\0';
    		strcpy(TIME, pf+1);
    		InsertList(head, Conversion(TIME), lyrics);
    		
    		pf = p+1;
        }
    }
    fclose(fp);
    return head;
}

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	head->time = 0;
	strcpy(head->lyrics, "lyrics");

	head->piror = head;
	head->next = head;
	return head;
}

void InsertList(PNODE head, long time, char *lyrics)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	pnew->time = time;
	strcpy(pnew->lyrics, lyrics);

	PNODE p = head->next;

	while(p != head)
	{
		if(pnew->time <= p->time)
		{
			pnew->piror = p->piror;
			p->piror->next = pnew;
			p->piror = pnew;
			pnew->next = p;
			break;
		}
		else
			p = p->next;
	}
	if(p == head)
	{
		pnew->piror = p->piror;
		p->piror->next = pnew;
		p->piror = pnew;
		pnew->next = p;
	}
}

char *FindList(PNODE head, long time)
{
	char *fp;
	PNODE p = head->next;

	while(p != head)
	{
		if(time == p->time)
		{
			fp = p->lyrics;
			head->next = p->next;
		    p->next->piror = head;
			free(p);
			return fp;
		}
		else
			return NULL;
	}
}

void displaylist(PNODE head)
{
	PNODE p = head->next;

	while(p != head)
	{
		printf("%s\n", p->lyrics);
		printf("%ld\n", p->time);
		p = p->next;
	}
}




















void SaveGedang(PLIST list)
{
	PLIST p = list->next;
	FILE *fd = fopen("/home/chenbiao/mywork/4.5/Gedang.txt", "w");
	while(p != list)
	{
		fprintf(fd, "%s\n", p->path);
		p = p->next;
	}
	fclose(fd);
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

void DelectGedang(PLIST list, int data)
{
	PLIST p = list->next;
	while(p != list)
	{
		if(p->data == data)
		{
			p->next->piror = p->piror;
			p->piror->next = p->next;
			free(p);
			break;
		}
		else
			p = p->next;
	}
}
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

void InsertGdDIR(char *str, PLIST list)
{
	DIR *pf = opendir(str);
	if(pf == NULL)
		return;
	struct dirent *dir;
	char path[80];
	while((dir = readdir(pf)) != NULL)
	{
		strcpy(path, str);
		char *p = (dir->d_name)+strlen(dir->d_name)-3;
		if(strcmp(p, "mp3") == 0)
		{
			if(path[strlen(path)-1] != '/')
				strcat(path, "/");
			strcat(path, dir->d_name);
			//printf("%d\n", dir->d_type);
			InsertGedang(list, path);
		}
		else if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0) && (dir->d_type == 4))
		{
			if(path[strlen(path)-1] != '/')
				strcat(path, "/");
			strcat(path, dir->d_name);
			InsertGdDIR(path, list);
		} 
	}
	printf("insert success\n");
	closedir(pf);
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

void Help()
{
	printf("添加歌曲：addmusic 文件绝对路径\n");
	printf("添加歌曲：addmusicdir 目录的绝对路径\n");
	printf("退出：quit\n");
	printf("查看歌单：list\n");
	printf("删除歌曲：delmusic\n");
	printf("清空歌单：clear\n");
}

void DisplayGedang(PLIST list)
{
	PLIST p = list->next;
	if(p == NULL)
		return;
	while(p != list)
	{
		printf("%d, %s\n", p->data, p->path);
		p = p->next;
	}
}

void ClearGedang(PLIST list)
{
	i = 0;
	PLIST p = list->next;
	if(p != list)
	{
		p->next->piror = list;
		list->next = p->next;
		free(p);
		ClearGedang(list);
	}	
}
