#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int b=4;

int fun(int (*p)[b],int a,int b)
{
		int i,j;
		srand(time(NULL));

		for(i=0;i < a;i++)
		{
				for(j=0;j < b;j++)
				{
						(*(p+i))[j] = rand()%100;
						printf("%3d",p[i][j]);
				}
				printf("\n");
		}
		return 0;
}

int *myfind(int (*p)[b],int a)
{
		return *p;
}
		



int main()
{
		int a,id;
		printf("please input student num:\n");
		scanf("%d",&a);
		int s[a][b];
		int *p;

		fun(s,a,b);
		printf("please input student_id:\n");
		scanf("%d",&id);

		myfind(s,id);
		p = myfind;
		printf("%d\n",*p);

		return 0;
}
