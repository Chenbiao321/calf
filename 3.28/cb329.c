#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int b = 6;

int myputs(int (*p)[b],int a,int b)
{
		int i,j;
		for(i=0;i < a;i++)
		{
				for(j=0;j < b;j++)
				{
						printf("%3d",(*(p+i))[j]);
				}
				printf("\n");
		}

		return 0;
}


int create(int (*p)[b],int a,int b)
{
		int i,j;
		srand(time(NULL));

		for(i=0;i < a;i++)
				for(j=0;j < b;j++)
						*((*(p+i))+j) = rand()%100;

		return 0;
}

int mysum(int (*p)[b],int a,int b,int *k)
{	
		int i,j;
		int sum;
		for(i=0;i < a;i++)
		{
				sum = 0;
				for(j=0;j < b;j++)
				{
						sum+=(*(p+i))[j];
				}
				*(k+i) = sum;
		}

		return 0;
}



int main()
{
		int a,i;
		printf("please input a\n");
		scanf("%d",&a);
		int s[a][b];
		int d[a];

		create(s,a,b);
		myputs(s,a,b);
		mysum(s,a,b,d);

		for(i=0;i < a;i++)
				printf("%4d",d[i]);
		printf("\n");
		
		return 0;
}
