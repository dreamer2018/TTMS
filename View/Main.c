#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Service/Struct.h"
#include"Common/List.h"
//#include"View/Studio_UI.h"
#include"View/Studio_UI.h"
int  Main()
{
	studio_node_t *head;
	int listSize=0;
	int flag=2;
	char choice;
	do
	{
		printf("\n\t\t************************************\n");
		printf("\t\t**           [A]       演出厅管理              \n");
		printf("\t\t**           [B]        剧目管理                 \n");
		printf("\t\t**           [C]        售票管理                    \n");
		printf("\t\t**           [D]       查询菜单                     \n");
		printf("\t\t**           [E]        票房统计                   \n");
		printf("\t\t**           [f]        系统用户管理            \n");
		printf("\t\t**           [G]        推出                             \n");
		printf("\t\t*************************************\n");
		printf("Please Input: ");
		scanf("%d",&choice);
		getchar();
		switch(choice)
		{
			case 'a' :;
		 	case 'A' :  
					List_Init(head, studio_node_t);
					Studio_UI_MgtEntry(head);
					break;
			case 'b' :;
			case 'B' :
					Play_UI_MgtEntry(flag);
					break;
			case 'C' :;
			case 'c'  :
					Schedule_UI_ListAll();
				break;
		}
	}while(choice!='G' || choice!='g');
}
