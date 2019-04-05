#ifndef _BBBC_H_
#define _BBBC_H_

typedef struct node
{
	struct node *piror;
	struct node *next;
	long time;
	char lyrics[60];
}NODE,*PNODE;

extern PNODE DownloadIrc();

extern PNODE CreateList();

extern void InsertList(PNODE head, long time, char *lyrics);

extern void PutLyrics(long time);

extern char *FindList(PNODE head, long time);

extern void displaylist(PNODE head);

extern void ClearList(PNODE head);

extern void DistoryList(PNODE head);

#endif