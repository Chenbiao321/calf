#include <stdio.h>

void cbmemcpy(void *a,void *b,int c)
{
		unsigned char *p1;
		unsigned char const *p2;
		p2 = b;
		p1 = a;
		int i;
		for(i=0;i < c;i++)
		{
			     *(p1+i) = *(p2+i);
		}
}

int main()
{
		int x=123456;
		int y;
    	cbmemcpy(&y,&x,4);
        printf("%d\n",y);
		return 0;
}
