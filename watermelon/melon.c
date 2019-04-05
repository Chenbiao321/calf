//修改文件内容
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "bbcc.h"

enum ML{KEY, SET_VALUE, SET_STRING, QUIT,LIST, CLEAR, HELP}CB;

int analyse(char *buff)
{
	char *p;
	if((p = (strchr(buff, ' '))) != NULL)
		return SET_VALUE;
	else if((strchr(buff, '=')) != NULL)
		return SET_STRING;
	else if(strcmp(buff, "quit") == 0)
		return QUIT;
	else if(strcmp(buff, "list") == 0)
		return LIST;
	else if(strcmp(buff, "clear") == 0)
		return CLEAR;
	else if(strcmp(buff, "help") == 0)
		return HELP;
	else
		return KEY;
}

int main()
{
	char buff[40];
	while(1)
	{
		printf("请输入指令：\n");
		gets(buff);
		switch(analyse(buff))
		{
			case KEY:getvalue(buff);break;
			case SET_VALUE:
			{
				char *p;
				char *fp;
				char value[20];
				int a;
				p = strchr(buff, ' ');
				*p = '\0';
				fp = strchr(p+1, ' ');
				*fp = '\0';
				strcpy(value, p+1);
				sscanf(fp+1,"%d", &a);
				setValue(buff, value, a);
			}
			break;

			case LIST:Displayfile();break;
			case SET_STRING:setString(buff);break;
			case CLEAR:ClearFile();break;
			case HELP:Help();break;
			case QUIT:return 0;
		}
	}
}