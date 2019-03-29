#include <stdio.h>

int main()
{
		char *p[3];
		int i;
		p[0] = "chenbiao from China";
		p[1] = "zhejiang panan";
		p[2] = "huike!";

		for(i=0;i < 3;i++)
				printf("%s\n",p[i]);

		return 0;
}
