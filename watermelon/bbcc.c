#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bbcc.h"

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	strcpy(head->key,"KEY");
	strcpy(head->value,"VALUE");

	head->next = head;
	head->piror = head;
	return head;
}

void InsertheadList(PNODE head, char *key, char *value)
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

int setString(const char *str)
{
	PNODE head = CreateList();
	ReadList(head);

	PNODE p = head->next;
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	char *fp;
	fp = strchr(str, '=');
	*fp = '\0';
	strcpy(pnew->key, str);
	strcpy(pnew->value, fp+1);

	pnew->piror = head->piror;
	head->piror->next = pnew;
	pnew->next = head;
	head->piror = pnew;

	SaveList(head);
	DistoryList(head);
}

void getvalue(const char *key)
{
	PNODE head = CreateList();
	ReadList(head);

	PNODE p = head->next;

	while(p != head)
	{
		if(strcmp(p->key, key) == 0)
		printf("%s\n", p->value);
	    p = p->next;
	}

	SaveList(head);
	DistoryList(head);
}

int setValue(const char *key, const char *value, int opt)
{
	switch(opt)
	{
		case 0:
		{
			PNODE head = CreateList();
			ReadList(head);

			PNODE p = head->next;
			PNODE pnew = (PNODE)malloc(sizeof(NODE));
			assert(pnew != NULL);

			strcpy(pnew->key, key);
			strcpy(pnew->value, value);

			pnew->piror = head->piror;
			head->piror->next = pnew;
			pnew->next = head;
			head->piror = pnew;

			SaveList(head);
			DistoryList(head);
			printf("Insert success!\n");
		}
		break;
		case 1:
		{
			PNODE head = CreateList();
			ReadList(head);

			PNODE p = head->next;

			while(p != head)
			{
				if(strcmp(p->key, key) == 0)
				{
					strcpy(p->value, value);
					break;
				}
				else
					p = p->next;
			}
			if(p == head)
				printf("修改失败\n");
			else
				printf("Modify success!\n");
			SaveList(head);
			DistoryList(head);
		}
		break;
		case 2:
		{
			PNODE head = CreateList();
			ReadList(head);
			PNODE p = head->next;
	        while(p != head)
	        {
		        if(strcmp(p->key, key) == 0)
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
			if(p == head)
				printf("删除失败\n");
			else
				printf("Delect success!\n");
			SaveList(head);
			SaveList(head);
			DistoryList(head);
		}
	}
}

void Displayfile()
{	
	PNODE head = CreateList();
	ReadList(head);

	PNODE p = head->next;
	while(p != head)
	{
		printf("%s=%s\n",p->key, p->value);
		p = p->next;
	}
	SaveList(head);
	DistoryList(head);
}

void ClearFile()
{
	PNODE head = CreateList();
	ReadList(head);

	PNODE p = head->next;

	if(p != head)
	{
		p->piror->next = p->next;
		p->next->piror = p->piror;
		free(p);
		ClearList(head);
	}

	SaveList(head);
	DistoryList(head);
}

void ClearList(PNODE head)
{
	PNODE p = head->piror;

	if(p != head)
	{
		p->piror->next = p->next;
		p->next->piror = p->piror;
		free(p);
		ClearList(head);
	}

}

void DistoryList(PNODE head)
{
	ClearList(head);
	free(head);
}

int SaveList(PNODE head)
{
	FILE *fp = fopen("savefile", "w");
	if(fp == NULL)
		return 0;
	PNODE p = head->next;

	while(p != head)
	{
		fprintf(fp, "%s=%s\n",p->key, p->value);
		p = p->next;
	}
	fclose(fp);
}

void ReadList(PNODE head)
{
	FILE *fp = fopen("savefile", "r");
	char ch[40];
	char key[20];
	char value[20];
	if(fp == NULL)
		;
	else
	{
		while(fgets(ch,40,fp) != NULL)
		{
		sscanf(ch,"%[^=]=%s", key, value);
		InsertheadList(head, key, value);
	    }
	}
}

void Help()
{
	printf("退出：quit\n");
	printf("查看文件内容：list\n");
	printf("添加文件内容：key=value\n");
	printf("添加文件内容：key value 0\n");
	printf("修改文件内容：key newvalue 1\n");
	printf("删除文件内容：key value 2\n");
	printf("清空文件内容：clear\n");
}