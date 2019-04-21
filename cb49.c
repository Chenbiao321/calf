求任意整数的阶层。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct node
{
	struct node *piror;
	struct node *next;
	char ch;
}NODE, *PNODE;

int mypow(int a, int b)
{
	int sum=1;
	if(b == 0)
		return 1;
	else
	{
	    for(int i = 0;i < b;i++)
	    	sum = sum*a;
	}
	return sum;
}

PNODE CreateList()
{
	PNODE head = (PNODE)malloc(sizeof(NODE));
	assert(head != NULL);

	head->ch = '0';

	head->piror = head;
	head->next = head;

	return head;
}

void InsertList(PNODE head, char ch)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	pnew->ch = ch;

	pnew->next = head->next;
	head->next->piror = pnew;
	pnew->piror = head;
	head->next = pnew;
}

void InserttailList(PNODE NUM)
{
	PNODE pnew = (PNODE)malloc(sizeof(NODE));
	assert(pnew != NULL);

	pnew->ch = '0';

	pnew->piror = NUM->piror;
	NUM->piror->next = pnew;
	pnew->next = NUM;
	NUM->piror = pnew;
}

int FunList(PNODE head, int i)
{
	int k = 0;
	int j;
	PNODE p = head->piror;
	while(p != head)
	{
		j = k;
		k = ((p->ch-'0')*i+j)/10;
		p->ch = ((p->ch-'0')*i+j)%10 + '0';
		p = p->piror;  
	}
	return k;
}

void DisplayList(PNODE head)
{
	PNODE p = head->next;
	while(p != head)
	{
		printf("%c", p->ch);
		p = p->next;
	}

}

int analyse(PNODE list, int i)
{
	int k = 1;
	int num;
	while(i/(mypow(10, k)) != 0)
	{
		++k;
	}
	for(int j = 0;j < k;j++)
	{
		num = i % mypow(10,(j+1)) / (mypow(10, j));
		i = i-num*(mypow(10, j));
		InsertList(list, num+'0');
	}

	return k;
}

PNODE CopyList(PNODE head)
{
	PNODE list = CreateList();
	PNODE p = head->piror;

	while(p != head)
	{
		InsertList(list, p->ch);
		p = p->piror;
	}

	return list;
}

char MyfindList(PNODE list, int j)
{
	PNODE p = list->piror;
	for(int i = 0;i < j;i++)
		p = p->piror;
	return p->ch;
}

void ClearList(PNODE head)
{
	PNODE p = head->next;

	if(p != head)
	{
		p->next->piror = head;
		head->next = p->next;
		free(p);
		ClearList(head);
	}

}

void Distory(PNODE head)
{
	ClearList(head);
	PNODE p = head;
	free(p);
	head = NULL;
}

int mysum(PNODE p1, PNODE p2)
{
	PNODE p = p2->piror;
	PNODE q = p1->piror;
	int k=0,l;

	while(p != p2)
	{
		l = k;
		k = ((q->ch-'0') + (p->ch-'0') + l)/10;
		q->ch = ((q->ch-'0') + (p->ch-'0') + l)%10 +'0';
		p = p->piror;
		q = q->piror;
	}
	while(k > 0 && q != p1)
	{
		l = k;
		k = (q->ch-'0'+l)/10;
		q->ch = (q->ch-'0'+l)%10 + '0';
		q = q->piror;
	}
	return k;
}

int main()
{
	PNODE head = CreateList();
	
	
    int n;
    int b;
    printf("please input a num:\n");
    scanf("%d", &n);
    InsertList(head, '1');
    for(int i = 1;i <= n;++i)
    {
    	PNODE list = CreateList();
    	int jiwei = analyse(list, i);
    	PNODE NUM[jiwei];
    	for(int j = 0;j < jiwei;j++)
    	{
    		NUM[j] = CopyList(head);
    		if((b = FunList(NUM[j], (MyfindList(list, j)-'0'))) != 0)
    		InsertList(NUM[j], b+'0');
    	    for(int k = 0;k < j;k++)
    	        InserttailList(NUM[j]);
    	}
    	for(int j = 0;j < jiwei-1;j++)
    	{
    		b = mysum(NUM[jiwei-1], NUM[j]);
    		if(b != 0)
    			InsertList(NUM[jiwei-1], b+'0');
    		Distory(NUM[j]);
    	}
    	Distory(list);
        Distory(head);
    	head = CopyList(NUM[jiwei-1]);
    }
    DisplayList(head);
    printf("\n");
 
    return 0;
}