#ifndef _CCCB_H_
#define _CCCB_H_

//#define hang 36
#define lie  50

#define size ((35)*(50))

typedef struct list
{
	struct list *piror;
	int dangchi_id;
	char dangchi[20];
	struct list *next;
}LIST, *PLIST;

typedef struct node
{
	struct node *piror;
	struct node *next;
    char hang[50];
    char dangchi[20];
}NODE, *PNODE;

struct msgbuf
{
	long mtype;
	char mtext[20];	
}msg;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern pthread_cond_t cond1; 
pthread_rwlock_t rwlock;
extern int num;
extern char DANGCHI[20]; 

extern PLIST CreateList();

extern void InsertList(PLIST head, char *dangchi);

extern PLIST Downloaddangchi();

extern void Finddangchi(PLIST head, int dangchi_id);

//extern ReadtoList(PNODE head);

extern PNODE CreateScreenList();

extern void InsertScreenList(PNODE screen, char *dangchi, int WeiZhi);

extern PNODE DownloadScreenList();

extern void DelectScreenList(PNODE screen);

extern void DisplayScreenList(PNODE screen);

extern void *RefreshScreen(void *arg);

extern void ClearScreenList(PNODE screen);

extern void SaveScreenList(void *addr, PNODE screen);

extern void *DelectScreenDanci(void *arg);
#endif