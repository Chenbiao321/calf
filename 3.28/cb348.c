#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int fun(int *p,int a)
{
		if(a != 0)
		return *(p+a-1)*fun(p,a-1);
		else
				return 1;
}

int main()
{
		int n;
		printf("please input  n:\n");
		scanf("%d",&n);
		int a[n],i;
		srand(time(NULL));

		for(i=0;i < n;i++)
		{
				a[i] = rand()%10;
				printf("%2d",a[i]);
		}
		puts("");

		printf("%d\n",fun(a,n));

		return 0;
}

