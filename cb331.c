//6、在主函数中输入10个等长的字符串。用另一函数对它们排序。然后在主函数输出这10个已排好序的字符串。
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int n;
int func(char (*p)[n],int i,int j,int *o);

int fun(char (*p)[n],int a,int b)
{
		if(p == NULL)
				return 1;
		int i,j=0,k;
		int *p1;
		int r=1;
		while(r != 0)
		{
				r = 0;
		for(i=0;i< a-1;i++,j=0)
		{
                func(p,i,j,&r);	    
		}
		}
		return 0;
		
}

int func(char (*p)[n],int i,int j,int *o)
{
		if(p == NULL)
				return 1;
		int k;
		int b=j;
		if(p[i][j] > p[i+1][j])
		{
				for(j=0;j < 3;j++)
				{
						k = p[i][j];
						p[i][j] = p[i+1][j];
						p[i+1][j] = k;
				}
				(*o)++;
		}
		else if(p[i][b] == p[i+1][b]&&b == n)
				return 0;

		else if(p[i][b] == p[i+1][b]&&b != n)
				func(p,i,++j,o);

		return 0;
}


int main()
{
		int i,j,x;
		
		srand(time(NULL));
		printf("the length of student_name\n");
		scanf("%d",&n);
		getchar();
		printf("please input the num:\n");
		scanf("%d",&x);
		char a[x][n];

		for(i=0;i< x;i++)
		{
				for(j=0;j < n;j++)
				{
						a[i][j] = rand()%26+65;
						printf("%c",a[i][j]);
				}
				printf("\n");
		}
		fun(a,x,n);
		puts("");

		for(i=0;i< x;i++)
		{
				for(j=0;j < n;j++)
				{
						printf("%c",a[i][j]);
				}
				printf("\n");
		}
		return 0;
}


