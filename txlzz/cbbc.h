#ifndef _CBBC_H_
#define _CBBC_H_

typedef struct code
{
		char name[20];
		char phone[15];
		struct code *next;
}NODE, *PNODE;

typedef struct list
{
		PNODE head;
		PNODE tail;
}LIST;

extern LIST *CreateList();

extern void InsertheadList(LIST *list,char const *name,char const *phone);
extern void InserttailList(LIST *list,char const *name,char const *phone);
extern void DelectList(LIST *list,char const *name);

extern void FindList(LIST *list,char const *name);

extern void ModifyList(LIST *list,char const *name,char const *newname);

extern void DisplayList(LIST *list);

extern void ClearList(LIST *list);

extern void help();

#endif

