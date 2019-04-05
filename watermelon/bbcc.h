#ifndef _BBCC_H_
#define _BBCC_H_

typedef struct node
{
	struct node *piror;
	char key[20];
	char value[20];
	struct node *next;
}NODE,*PNODE;


extern PNODE CreateList();

extern int setString(const char *str);

extern void getvalue(const char *key);

extern int setValue(const char *key, const char *value, int opt);

extern void Displayfile();

extern void ClearFile();

extern void DistoryList(PNODE head);

extern void ClearList(PNODE head);

extern int SaveList(PNODE head);

extern void ReadList(PNODE head);

extern void Help();

#endif