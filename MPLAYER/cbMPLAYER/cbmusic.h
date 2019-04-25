#ifndef _CBMUSIC_H_
#define _CBMUSIC_H_

typedef struct node
{
	struct node *piror;
	struct node *next;
	long time;
	char lyrics[60];
}NODE,*PNODE;

typedef struct list
{
	struct list *piror;
	struct list *next;
	char path[80];
	int data;
}LIST, *PLIST;


typedef struct pzwj
{
	int music_id;
	char musicname[100];
	long time;
	int playmode;
	int OnOff;
	int OUTDO;
	int IRCDO;
	int PAUSE;
	int musicnumber;
	struct list *connect;
}PZWJ, *PPZWJ;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern int LRCIO;

extern PNODE DownloadIrc();

extern PNODE CreateList();

extern void InsertList(PNODE head, long time, char *lyrics);

extern void PutLyrics(long time);

extern char *FindList(PNODE head, long time);

extern void displaylist(PNODE head);

extern void ClearList(PNODE head);

extern void DistoryList(PNODE head);

extern void InsertGedang(PLIST list, char *path);

extern PLIST CreateGedang();

extern void SaveGedang(PLIST list);

extern PLIST DownloadGedang();

extern void Help();

extern void DisplayGedang(PLIST list);

extern void ClearGedang(PLIST list);

extern void InsertGdDIR(char *str, PLIST list);

extern void DelectGedang(PLIST list, int data);

extern void FindMusicname(PLIST list, char *path, int *music_id);

extern void Sendcmd(char *buff, int set);

extern PPZWJ Downloadpzwj();

extern void *GetmusicMSG(void *arg);

extern void PlayChusimusic();

extern void fun();

extern void fun1();

extern void *PlayLyrics(void *arg);

extern void Savepzwj(PPZWJ Chusi);

extern char *Changetolrc(PPZWJ Chusi, char *path);

extern void Firstpzwj(FILE *pf);

extern void *DomusicMSG(void *agc);

#endif