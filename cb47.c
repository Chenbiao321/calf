#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct A
{
	char ch[5];
	int num;
};

int intcmp(void *a, void *b)
{
	if(*(int *)a > *(int *)b)
		return 1;
	else
		return 0;
}

int chcmp(void *a, void *b)
{
	if(*(char *)a > *(char *)b)
		return 1;
	else
		return 0;
}

int str1cmp(void *a, void *b)
{
	return strcmp(*(char **)a , *(char **)b);
}

int str2cmp(void *a, void *b)
{
	return strcmp(*(char (*)[5])a, *(char (*)[5])b);
}

int structcmp(void *a, void *b)
{
	if(((struct A *)a)->num > ((struct A *)b)->num)
		return 1;
	else
		return 0;
}

void change(void *a, void *b, int size)
{
	void *c = (void *)malloc(size);
	memcpy(c, a, size);
	memcpy(a, b, size);
	memcpy(b, c, size);
	free(c);
}

void mysort(void *a, int size, int c, int (*fun)(void *p1, void *p2))
{
	for(int i = c-1;i > 0;--i)
		for(int j = 0;j < i;++j)
		{
			if(fun(a+size*j, a+size*(j+1)) > 0)
				change(a+size*j, a+size*(j+1), size);
		}
}

int main()
{
	int a[5] = {3, 2, 4, 5, 1};
	char  ch[5] = {'d', 'e', 'a', 'c', 'b'};
	char *str1[5] = {"BBB", "AAA", "DDD", "EEE", "CCC"};
	char str2[5][5] = {"AAA", "CCC", "EEE", "DDD", "BBB"};
	struct A s[3] = {{"CCC", 80}, {"BBB", 75}, {"AAA", 90}};

	mysort(s, sizeof(struct A), 3, structcmp);
	for(int i = 0;i < 3;i++)
		printf("%s  %d\n", s[i].ch, s[i].num);

	printf("\n");
}