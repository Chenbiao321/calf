#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;

int fun(int (*p)[n],int a,int b)
{
		int i,j;
		srand(time(NULL));
		for(i=0;i < a;i++)
				for(j=0;j < b;j++)
						p[i][j] = rand()%100;
		return 0;
}

int myputs(int (*p)[n],int a,int b)
{
		int i,j;
		for(i=0;i < a;i++)
		{
				for(j=0;j < b;j++)
						printf("%3d",p[i][j]);
			puts("");
		}
	return 0;
}	

int func(int (*p)[n],int a,int b)
{
		int i,j,k;
		for(i=0;i < a;i++)
				for(j=0;j < i;j++)
				{
						k = p[i][j];
						p[i][j] = p[j][i];
						p[j][i] = k;
				}
						
		return 0;
}



int main()
{
		int m;
		printf("please int put m ,n:\n");
		scanf("%d%d",&m,&n);
		int a[m][n];

		fun(a,m,n);
		myputs(a,m,n);
		puts("");
		func(a,m,n);
		myputs(a,m,n);

		return 0;
}


