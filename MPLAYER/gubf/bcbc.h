#ifndef _BCBC_H_
#define _BCBC_H_

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
}PZWJ, *PPZWJ;

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

#endif