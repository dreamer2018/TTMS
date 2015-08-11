
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Service/Struct.h"
#include"../Service/Studio.h"
#include"Studio_UI.h"
#include"../Service/EntityKey.h"
#include"../Common/List.h"
#include"../Service/Seat.h"
#include"Seat_UI.h"
#define PAGESIZE 10
//全局的放映厅链表头指针

//添加新演出厅

int Studio_UI_Add()
{
	int sign,newCount=0;
	studio_t  *data;
	data=(studio_t *)malloc(sizeof(studio_t ));
	char ch;
	do
	{
		printf("请输入演出厅名称：");
		scanf("%s",data->name);
		getchar();
		printf("请输入演出厅行数：");
		scanf("%d",&data->rowsCount);
		getchar();
		printf("请输入演出厅列数：");
		scanf("%d",&data->colsCount);
		getchar();
		data->id=EntKey_Srv_CompNewKey("studio");
		newCount+=Studio_Srv_Add(data);
		printf("是否继续添加y/N：");
		while(1)
		{
			ch=getchar();
			if(89==ch || 121==ch)
			{
				sign=1;
				break;
			}
			else if(78==ch || 110==ch)
			{
				sign=0;
				break;
			}
			else
				printf("输入错误，请重新输入！");
		}
	}while(sign);
	free(data);
	return newCount;
}

//修改演出厅

int Studio_UI_Modify(int id)
{
	int rtn=0,cols,rows;
	studio_t *data;
	seat_list_t list;
	List_Init(list,seat_node_t);
	data=(studio_t *)malloc(sizeof(studio_t ));
	if(Studio_Srv_FetchByID(id,data))
	{
		printf("请输入演出厅名称：");
		scanf("%s",data->name);
		getchar();
		if(Seat_Srv_FetchByRoomID(list,id))
		{
			printf("%d %d",data->rowsCount,data->colsCount);
			while(1)
			{
					printf("请输入演出厅行数(>=%d)：",data->rowsCount);
					scanf("%d",&rows);
					getchar();
					printf("请输入演出厅列数(>=%d)：",data->colsCount);
					scanf("%d",&cols);
					getchar();
					if(rows<data->rowsCount || cols<data->colsCount)
					{
						printf("你的输入不正确，请重试！");
						getchar();
					}
					else
					{
						data->rowsCount=rows;
						data->colsCount=cols;
						break;
					}
			}
		}
		else
		{
			printf("请输入演出厅行数：");
			scanf("%d",&data->rowsCount);
			getchar();
			printf("请输入演出厅列数：");
			scanf("%d",&data->colsCount);
			getchar();
		}
		data->id=id;
		Studio_Srv_Modify(data);
		rtn=1;
	}
	else
	{
		printf("您输入的演出厅不存在！\n");
		getchar();
	}
	free(data);
	return rtn;
}

//删除演出厅
int Studio_UI_Delete(int id)
{
	int rtn=0;
	studio_t buf;
	if(Studio_Srv_FetchByID(id,&buf))
	{
		if(Studio_Srv_DeleteByID(id))
		{
			rtn=1;
			Seat_Srv_DeleteAllByRoomID(id);
			printf("删除成功！回车继续\n");
			getchar();
		}
		else
		{
			printf("删除失败！回车继续\n");
			getchar();
		}
	}
	else
	{
		printf("未找到！回车继续");
		getchar();
	}
	return rtn;
}

void Studio_UI_MgtEntry()
 {
 	
	int pageSize=PAGESIZE, i,id;
	int listSize;
	studio_node_t *list,*p;
	List_Init(list,studio_node_t);
	Pagination_t paging;
	char choice;
	system("clear");
	listSize=Studio_Srv_FetchAll(list);
	paging.pageSize = pageSize;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do {
			system("clear");
			printf("\n\t\t\033[33m======================= 演出厅列表 =======================\n");
			printf("\t\t%8s%18s%15s%12s\n","id", "名称", "行数", "列数");
			printf("\t\t----------------------------------------------------------\n");

			for (i = 0, p = (studio_node_t *) (paging.curPos);p != list && i < paging.pageSize; i++, p=p->next) {
					printf("\t\t%8d%15s%12d%10d\n",p->data.id,p->data.name, p->data.rowsCount, p->data.colsCount);
			}
			printf("\t\t----------------------------------------------------------\n");
			printf("\t\t--------总行数 :%2d ----------------- 页码： %2d/%2d --------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("\t\t----------------------------------------------------------\n");
			printf("\t\t**【P】上一页             【N】下一页        【R】返回  **\n");
			printf("\t\t**【A】 : 添加新演出厅　　【U】 : 修改演出厅            **\n");
			printf("\t\t**【D】 : 删除演出厅　　　【S】 : 座位管理              **\n");
			printf("\t\t==========================================================\033[0m\n");
			printf("\t\tYour Choice:");
			scanf("\t\t%c", &choice);
			switch (choice)
			{
				case 'A': ;
				case 'a': 
									Studio_UI_Add();
									getchar();
							 		paging.totalRecords=Studio_Srv_FetchAll(list);
							      	List_Paging(list,paging,studio_node_t);               // 添加完成后，显示第一页 Paging_Locate_LastPage(list, paging,studio_node_t);
							      	break;
				case 'U':
				case 'u':
									printf("请输入您要修改的演出厅id:");
									scanf("%d",&id);
									getchar();
									Studio_UI_Modify(id) ;
									paging.totalRecords=Studio_Srv_FetchAll(list);
									List_Paging(list,paging,studio_node_t);
									break;
				case 'D':;
				case 'd':
									printf("请输入您要删除的演出厅id:");
									scanf("%d",&id);
									getchar();
									Studio_UI_Delete(id);
									paging.totalRecords=Studio_Srv_FetchAll(list);
									List_Paging(list,paging,studio_node_t);
									break;
				case 'S':;
				case 's': 
									printf("请输入您要管理的演出厅id:");
									scanf("%d",&id);
									getchar();
									Seat_UI_MgtEntry(id);break;
				case 'r':
				case 'R':break;
				case 'p':
				case 'P':
									if (1 < Pageing_CurPage(paging)) 
									{
										Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
									}
									break;
				case 'n':
				case 'N':
									if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) 
									{
										Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
									}
									break;
			}
	} while (choice != 'r' && choice != 'R');
}
