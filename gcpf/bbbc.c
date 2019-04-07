#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bbbc.h"

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
	if(p == head)
	free(head);
}

void displaylist(PNODE head)
{
	printf("%s\n", head->lyrics);
	printf("%ld\n", head->time);
	PNODE p = head->next;

	while(p != head)
	{
		printf("%s\n", p->lyrics);
		printf("%ld\n", p->time);
		p = p->next;
	}
}