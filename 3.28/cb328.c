#include <stdio.h>

int fun(int a)
{
		int  q,w,e;
		e = a%10;
		w = (a%100-e)/10;
		q = (a-w*10-e)/100;
		return (a == e*e*e+w*w*w+q*q*q)?1:0;
}

int main()
{
		int n;
		printf("please input a num:\n");
		scanf("%d",&n);
		printf("%d\n",fun(n));
		return 0;
}
				
