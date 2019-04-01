//冒泡排序，选择排序，插入排序模拟
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int fun(int *p,int a)
{
		int i;
		srand(time(NULL));
		for(i=0;i < a;i++)
		{
				*(p+i) = rand()%100;
		}

		return 0;
}

void myputs(int *p,int a)
{
		int i;
		for(i=0;i < a;i++)
		{
				if(i%20 == 0)
						printf("\n");
				printf("%3d",*(p+i));
		}
		puts("");
}

void mppx(int *p,int a)
{
		int i,j,k;
		for(i=a-1;i > 0;i--)
				for(j=0;j < i;j++)
				{
						if(p[j]  > p[j+1])
						{
								k = p[j];
								p[j] = p[j+1];
								p[j+1] = k;
						}
				}
}

void xzpx(int *p,int a)
{
		int i,j,k;
		int t;
		int max;
		for(i=a;i > 1;i--)
		{
				max = *(p+0);
				k = 0;
				for(j=0;j < i;j++)
				{
						if(*(p+j) > max){
								max = *(p+j);
								k = j;
						}
				}
				t = *(p+k);
				*(p+k) = *(p+i-1);
				*(p+i-1) = t;
		}
}

int crpx(int *p,int a)
{
		int i,j,k;
		int f[a];
		for(i=0;i < a;i++)
				printf("%d  ",p[i]);
		f[0] = *(p+0);
		for(i = 1;i < a;i++)
		{
				if(*(p+i) >= *(f+i-1))
						*(f+i) = *(p+i);
				else
				{
                        k = 1;
						while(*(p+i) < *(f+i-k))
						{
								++k;
								if(k == i+1)
								{
									//	for(j=i;j > 0;j--)
                                      //         *(f+j) = *(f+j-1);
									//	*(f+0) = *(p+i);
										break;
								}
						}
						for(j=i;j > i-k+1;j--)
								*(f+j) = *(f+j-1);
						*(f+i-k+1) = *(p+i);
				}

		}
		myputs(f,a);
		memcpy(p,f,a*sizeof(int));
}

        

int main(int argc,char *argv[])
{
		int n;
		int mode;
		printf("please input a num:\n");
		scanf("%d",&n);
		int s[n];
		fun(s,n);
		printf("please input the mode:1 or 2 or 3\n");//选择排序方式
		scanf("%d",&mode);
        switch(mode){
				case 1:mppx(s,n);break;
			    case 2:xzpx(s,n);break;
				case 3:crpx(s,n);break;
		}
		myputs(s,n);

		return 0;
}

