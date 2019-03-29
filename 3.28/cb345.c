#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int fun(int *p,int a)
{
		int i;
		if(p != NULL)
		{
		srand(time(NULL));
		for(i=0;i < a;i++)
		{
				*(p+i) = rand()%100;
				printf("%3d",*(p+i));
		}
		printf("\n");
		}
		return 0;
}

int func(int *p,int a)
{
		int c[a];
		int i,j,k;
		for(i=0;i < a;i++)
				c[i] = *(p+i);

        while(j != 0)
		{
		j = 0;		
		for(i=0;i < a-1;i++)
				if(c[i] > c[i+1])
				{
						k = c[i];
						c[i] = c[i+1];
						c[i+1] = k;
						j++;
				}
		}
		for(i=0;i < a;i++)
				printf("%3d",c[i]);
		puts("");
}

int main()
{
		int n;
		int i;
		printf("please input len:\n");
		scanf("%d",&n);
		int a[n];

		fun(a,n);
		func(a,n);

		return 0;
}



