#ifndef _CBFG_H_
#define _CBFG_H_

typedef struct node
{
	char key[20];
	char value[80];
	struct node *next;
	struct node *piror;
}NODE, *PNODE;

typedef struct list
{
	struct list *piror;
	struct list *next;
	PNODE head;
	int id;
	char ip[20];
}LIST, *PLIST;

extern int num ;

extern char **environ;

extern PNODE CreateList();

extern void InsertList(PNODE head, char *key, char *value);

extern void ModifyList(PNODE head, char *key, char *value);

extern void ClearList(PNODE head);

extern PNODE Downloadpzwj();

extern void DisplayList(PNODE head);

extern char *FindList(PNODE head, char *key);

extern void Chusipzwj();

extern void Savepzwj(PNODE head);

extern int Createsock();

extern int Createservesock();

extern void DisplayNodeList(PLIST list, int id);

extern void DisplayNode(PLIST list);

extern int FindNode(PLIST list, char *ip);

extern void InsertNode(PLIST list, char *ip, PNODE head);

extern PLIST CreateNode();

extern void ChangeNode(PLIST list, char *ip, PNODE pnew);

extern void *UDPserve(void *arg);

extern PNODE RecvpackMsg(char *buff, int *cmd, int set);

extern int PackMsg(char *message, PNODE head, int cmd);

extern void *SendMsg(void *arg);

#endif