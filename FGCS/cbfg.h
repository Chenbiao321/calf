#ifndef _CBFG_H_
#define _CBFG_H_

extern long ccd;
extern long OnOff;

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

typedef struct inet
{
	struct inet *piror;
	struct inet *next;
	long pack_id;	
	char filename[100];
	int size;
	char ip[20];
	pthread_cond_t cond;
}INET, *PINET;

typedef struct orange
{
    PNODE pnew;
    PLIST list;
    PINET inet;
    int sock;
}ORANGE;


typedef struct apple
{
	PINET inet;
	int sock;	
}APPLE;

typedef struct banana
{
	PINET inet;
	PLIST list;
}BANANA;


extern long num ;

extern long OnOff;

extern char **environ;

extern PNODE CreateList();

extern void InsertList(PNODE head, char *key, char *value);

extern void ModifyList(PNODE head, char *key, char *value);

extern void ClearList(PNODE head);

extern PNODE Downloadpzwj();

extern void DisplayList(PNODE head);

extern char *FindList(PNODE head, const char *key);

extern void Chusipzwj();

extern void Savepzwj(PNODE head);

extern int Createsock();

extern int Createservesock();

extern void DisplayNodeList(PLIST list, int id);

extern void DisplayNode(PLIST list);

extern PNODE FindNode(PLIST list, const char *ip);

extern void InsertNode(PLIST list, char *ip, PNODE head);

extern PLIST CreateNode();

extern void ChangeNode(PLIST list, char *ip, PNODE pnew);

extern void *UDPserve(void *arg);

extern PNODE RecvpackMsg(char *buff, int *cmd, int set, char *extra);

extern int PackMsg(char *message, PNODE head, int cmd, char *extra);

extern void *SendMsg(void *arg);

extern PNODE GetNodeid(PLIST list, int id);

extern void Help();

extern void InsertInet(PINET inet, long pack_id, char *filename, int size, char *ip);

extern PINET CreateInet();

extern void *TCPserve(void *arg);

extern void *Recvfile(void *arg);

extern int Createtcpsock();

extern char *Getfilename(PINET inet, const long tt);

extern void *Downloadfile(void *arg);

extern void SendChatmsg(PNODE INET, PLIST list, char *p, void *sock);

extern void DisplayInet(PINET inet);

extern void DelectInet(PINET inet, long tt);

extern PNODE CopyList(PNODE pnew);

extern void BigtoLittle(char *extra);

extern void Sendcmd(struct sockaddr_in *addr, int cmd, char *extra, PNODE head, void *sock);

extern PINET FindInet(PINET inet, long tt);

extern PINET FindInetname(PINET inet,char *filename);
#endif