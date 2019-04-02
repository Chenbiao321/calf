#include "cbbc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

LIST *CreateList()
{
		LIST *list = (LIST *)malloc(sizeof(LIST));
		assert(list != NULL);

        PNODE head = (PNODE)malloc(sizeof(NODE));
		assert(head != NULL);
 
		head->next = NULL;
		strcpy(head->name,"name");
		strcpy(head->phone,"phone");

		list->head = head;
		list->tail = head;
		return list;
}

void InserttailList(LIST *list,char const *name,char const *phone)
{
	    PNODE pnew = (PNODE)malloc(sizeof(NODE));
		assert(pnew != NULL);

		strcpy(pnew->name,name);
		strcpy(pnew->phone,phone);
		pnew->next = NULL;

		if(list->head != NULL)
		{
		list->tail->next = pnew;
		list->tail = pnew;
		}
		else
		{
				list->head = pnew;
				list->tail = pnew;
		}
}

void InsertheadList(LIST *list,char const *name,char const *phone)
{
        PNODE pnew = (PNODE)malloc(sizeof(NODE));
		assert(pnew != NULL);

		strcpy(pnew->name,name);
		strcpy(pnew->phone,phone);

		pnew->next = list->head->next;
		list->head->next = pnew;
}

void DelectList(LIST *list,char const *name)
{
		PNODE p = list->head->next;
		PNODE q = list->head;

		while(p != NULL)
		{
				if(strcmp(p->name,name) == 0)
				{
						q->next = p->next;
						free(p);
						p = q->next;
						continue;
				}
				else
				{
						p = p->next;
						q = q->next;
				}
		}
		if(strcmp(list->head->name,name) == 0)
		{
				p = list->head;
				list->head = list->head->next;
				free(p);
		}
}

void FindList(LIST *list,char const *name)
{
		PNODE p = list->head;

		while(p != NULL)
		{
				if(strcmp(p->name,name) == 0)
				{
						printf("%s, %s\n",p->name,p->phone);
						p = p->next;
						continue;
				}
				else
						p = p->next;
		}
}

void ModifyList(LIST *list,char const *name,char const *newname)
{
		PNODE p = list->head;

		while(p != NULL)
		{
				if(strcmp(p->name,name) == 0)
				{
						strcpy(p->name,newname);
						p = p->next;
						continue;
				}
				else
						p = p->next;
		}
}

void DisplayList(LIST *list)
{
		PNODE p = list->head;
		while(p != NULL)
		{
				printf("%-20s  | %-15s\n",p->name,p->phone);
				p = p->next;
		}
}

void ClearList(LIST *list)
{
		PNODE p = list->head->next;
		PNODE q = list->head;
		while(p != NULL)
		{
				q->next = p->next;
				free(p);
				p = q->next;
		}
}



void help()
{
		char *p[9];
		p[0] = "指令集合：";
		p[1] = "增加:add chenbiao 15957102049";
		p[2] = "删除:del chenbiao";
		p[3] = "修改:modify chenbiao xiaoming";
		p[4] = "查找:find chenbiao";
		p[5] = "查看列表:list";
		p[6] = "帮助:help";
		p[7] = "退出:quit";
		p[8] = "清空列表:clear";

		int i;
		for(i=0;i < 8;i++)
				puts(p[i]);
}
         













