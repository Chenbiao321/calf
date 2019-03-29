#include <stdio.h>

int add(int a,int b)
{
		return a+b;
}
int fun(int (*pf)(int a,int b),int c)
{
		int t;
		t = pf(1,2)+c;
		return t;
}

int main()
{
		printf("%d\n", fun(add,3));
		return 0;
}

