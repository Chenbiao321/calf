//9、写一函数完成将无符号十进制整数转换成十六进制字符表示并存于字符串数组中。
#include <stdio.h>
int fun(int);

int sixteen(char *p,void *a)
{
		if(p == NULL||a == NULL)
				return 1;
		unsigned char *p1;
		int i,j;
		p1 = a;
		for(i=3;i >= 0;i--)
		{
				int m,n;
			    m = (*(p1+i))%16;
				n = ((*(p1+i)-m))/16;
				*(p+((3-i)*2)) = (char)fun(n);
				*(p+((3-i)*2+1)) = (char)fun(m);
		}
		return 0;
}

int fun(int a)
{
		if(a < 10)
				return a+48;
		else
				return a+87;
}
						
int main()
{
		unsigned int a;
		char s[8];
		char f[] = {'0','x'};
		int i=0;
		printf("please input a:\n");
		scanf("%d",&a);

		sixteen(s,&a);


		while(i < 2)
				putchar(f[i++]);
		i = 0;
		while(i < 8)
		{
				if(s[i] != '0')
						break;
				else
						i++;
		}
		while(i < 8)
				putchar(s[i++]);

		putchar(s[i]);

		puts("");

		return 0;
}


