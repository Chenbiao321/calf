#include <stdio.h>

int chifang(int a,int b)
{
		if(b != 1)
		return	a*chifang(a,b-1);
		else
				return a;
}

int main()
{
		int m,n;
		printf("please input m,n:\n");
		scanf("%d%d",&m,&n);

		printf("%d\n",chifang(m,n));

		return 0;
}
