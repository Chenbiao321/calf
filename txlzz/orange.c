#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "cbbc.h"

int analyse(char *buff,char *str)
{
		char *p1;
		if((p1 = strchr(buff,' ')) != NULL)
		{
		        *p1 = '\0';
				str = strcpy(str,p1+1);
		        if(strcmp(buff,"add") == 0)
						return 1;
				else if(strcmp(buff,"del") == 0)
						return 2;
				else if(strcmp(buff,"modify") == 0)
						return 3;
				else if(strcmp(buff,"find") == 0)
						return 4;
				else
						printf("指令无法接受\n");
		}
		else if(strcmp(buff,"list") == 0)
				return 5;
		else if(strcmp(buff,"help") == 0)
				return 6;
		else if(strcmp(buff,"clear") == 0)
				return 7;
		else
				printf("指令无法接受\n");
}




int main()
{
		LIST *list = CreateList();
		char buff[40];
		char str[40];
		int num;
		printf("请输入指令：\n");
		while(1)
		{
				printf("输入：");
				gets(buff);
				if(strcmp(buff,"quit") == 0)
						break;
				else
				{
						switch(analyse(buff,str))
						{
								case 1:{
											   char name[20] = "NULL";
											   char phone[15] = "NULL";
											   char *p2;
											   if((p2 = strchr(str,' ')) != NULL)
											   {
													   *p2 = '\0';
													   strcpy(name,str);
													   strcpy(phone,p2+1);
													   InserttailList(list,name,phone);
											   }
											   else
											   {
													   strcpy(name,str);
													   InserttailList(list,name,phone);
											   }
											   printf("insert success\n");
									   };
									   break;
								case 2:  DelectList(list,str);
										 break;
								case 3:{
											   char name[20] = "NULL";
											   char newname[20] = "NULL";
											   char *p2;
											   if((p2 = strchr(str,' ')) != NULL)
											   {
													   *p2 = '\0';
													   strcpy(name,str);
													   strcpy(newname,p2+1);
													   ModifyList(list,name,newname);
													   printf("modify success\n");
											   }
											   else
													   printf("指令无法接受\n");
									   }
									   break;
								case 4:   FindList(list,str);
										  break;
								case 5:   DisplayList(list);
										  break;
								case 6:   help();
										  break;
								case 7:   ClearList(list);
						}
				}	
		}

		return 0;
}


