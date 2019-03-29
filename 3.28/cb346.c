#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int fun(int *p,int a)
{
		int i;
		if(p == NULL)
				return 1;
		srand(time(NULL));

		for(i=0;i < a;i++)
		{
				*(p+i) = rand()%100;
				printf("%3d",*(p+i));
		}
		puts("");
		return 0;
}

int panduan(int a)
{
		int i;
		for(i=2;i <= a;i++)
				if(a%i == 0)
						break;

		if(i==a)
				return 1;
		else
				return 0;
}



int find(int *p,int a,int (*pf)(int c))
{
		int i;
		if(p == NULL)
				return 1;
		for(i=0;i < a;i++)
		{
				if(*(p+i) == (1||2))
				*(p+i) = 0;
				if(panduan(*(p+i)))
						;
				else
						*(p+i) = 0;
		}
		return 0;
}

int paixu(int *p,int a)
{
		int i,j,k;
		while(j != 0)
		{
				j = 0;
		for(i=1;i < a-1;i++)
				if(*(p+i) > *(p+i+1))
				{
						k = *(p+i);
						*(p+i) = *(p+i+1);
						*(p+i+1) = k;
						j++;
				}
		}
}

int main()
{
		int i,n;
		printf("please input length:\n");
		scanf("%d",&n);
		int s[n];

		fun(s,n);
		find(s,n,panduan);

		for(i=0;i < n;i++)
				printf("%3d",s[i]);
		puts("");
		paixu(s,n);
    	for(i=0;i < n;i++)
				if(*(s+i) != 0)
				printf("%3d",s[i]);
		puts("");


		paixu(s,n);

		return 0;
}
