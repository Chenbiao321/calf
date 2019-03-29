#include <stdio.h>

int add(int a,int b)
{
		return a+b;
}
int sub(int a,int b)
{
		return a-b;
}
int slm(int a,int b)
{
		return a*b;
}
int div(int a,int b)
{
		return a/b;
}


int main()
{
		int (*cb)(int a,int b);
		int i,j;
		char c;
		printf("please inuput i+-*/j\n");
		scanf("%d%c%d",&i,&c,&j);
		
		if(c == '+') 
				cb = add;
		if(c == '-') 
				cb = sub;
		if(c == '*') 
				cb = slm;
		if(c == '/') 
				cb = div;

		printf("%d\n",cb(i,j));

		return 0;
}
