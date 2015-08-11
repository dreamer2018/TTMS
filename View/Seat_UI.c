/*
 *  Seat_UI.c
 *
 *  Created on: 2015年5月23日
 *  Author: lc
 */

#include <stdio.h>
#include "../Service/EntityKey.h"
#include "../Common/List.h"
#include"../Service/Struct.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "Seat_UI.h"

//根据状态返回对应表示状态符号
inline char Seat_UI_Status2Char(seat_status_t status) 
{
	if(status==SEAT_GOOD)
	{
		return '#';
	}else if(status==SEAT_BROKEN)
	{
		return '~';
	}else if(status==SEAT_NONE)
	{
		return ' ';
	}
}

//根据状态符号返回桌位状态
inline seat_status_t Seat_UI_Char2Status(char statusChar) 
{
	if(statusChar=='#')
		return SEAT_GOOD;
	if(statusChar=='~')
		 return SEAT_BROKEN;
	if(statusChar==' ') 
		return SEAT_NONE;
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) 
{
	int newRecCount=0;
	int rows,columns,id;
	seat_list_t s=NULL;
	seat_t *p;
	char choice;
	do
	{
		system("clear");
		printf("\t\t**************  添加新坐位  ***************\n");
		printf("\t\t输入添加座位的行，列号(空格隔开):");
		scanf("%d%d",&rows,&columns);
		getchar();
		if(row<rows-1||column<columns-1)
		{
			printf("\t\t超出座位限度,添加失败！\n");
			printf("\t\tPress Any Key To Continue ......");
			getchar();
		}
		else
		{
			if(Seat_Srv_FindByRowCol(list,rows-1,columns-1))
			{
				printf("\t\t座位存在!\n");
				printf("\t\tPress Any Key To Continue ......");
				getchar();
			}
			else
			{
				id=EntKey_Srv_CompNewKey("Seat");
				p=(seat_t *)malloc(sizeof(seat_t));
				p->id=id;
				p->roomID=roomID;
				p->row=rows-1;
				p->column=columns-1;
				p->status=1;
				if(Seat_Srv_Add(p))
				{
					printf("\t\t成功添加!\n");
					printf("\t\tPress Any Key To Continue ......");
					getchar();
				}
				else
				{
					free(p);
					free(s);
					printf("\t\t添加失败!\n");
					printf("\t\tPress Any Key To Continue ......");
					getchar();
				}
				free(p);
			}
		}
		printf("\t\t==========================================\n");
		printf("\t\t[A]继续  or [R] 返回");
		scanf("%c",&choice);
	}while(choice=='a'||choice=='A');
	return newRecCount;
}

int Seat_UI_Delete(int ID) 
{
	int rtn=0;
	seat_t buf;
	if(1==Seat_Srv_FetchByID(ID,&buf))
	{
		if(Seat_Srv_DeleteByID(ID))
		{	
			printf("删除成功！");
			printf("Press Any Key To Continue ......");
			getchar();
			rtn=1;
		}
		else
		{
			printf("删除失败 !\n");
			printf("Press Any Key To Continue ......");
			getchar();
		} 
	}
	else
	{
		printf("未找到！\n");
		printf("Press Any Key To Continue ......");
		getchar();
	}
	return rtn;
}


//座位管理函数
void Seat_UI_MgtEntry(int roomID) 
{
	int pageSize=10, i;
	int listSize;
	char choice;
	seat_list_t list,s;
	Pagination_t paging;
	List_Init(list,seat_node_t);
	int seat_id;
	studio_t p;
	if(Studio_Srv_FetchByID(roomID,&p))
	{
		system("clear");
		listSize=Seat_Srv_FetchByRoomID(list,roomID);
		if(0==listSize)
		{
			Seat_Srv_RoomInit(list,roomID,p.rowsCount,p.colsCount);
			Studio_Srv_Modify(&p);
		}
		paging.pageSize = pageSize;
		paging.totalRecords=Seat_Srv_FetchByRoomID(list,roomID);
		Paging_Locate_FirstPage(list, paging);
		do
		{
			system("clear");
			printf("\n\t\t\033[31m================座位列表====================\n");
			printf("\t\t%6s %15s%10s%6s   %6s\n","id","演出厅id","行","列","状态");
			printf("\t\t--------------------------------------------\033[0m\n");
			for (i = 0, s= (seat_node_t *) (paging.curPos);s != list && i < paging.pageSize; i++, s=s->next) 
			{
					printf("\t\t%6d %10d%11d%5d  %8s\n",s->data.id,s->data.roomID, s->data.row+1, s->data.column+1,Seat_Status(s->data.status));
			}
			printf("\t\t\033[31m--------------------------------------------\n");
			printf("\t\t----总行数 :%2d ------------页码： %2d/%2d ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("\t\t============================================\n");
			printf("\t\t**【A】添加座位        【D】删除座位      **\n");
			printf("\t\t**【R】返回上一级　    【M】修改座位状态  **\n");
			printf("\t\t**【P】上一页　　   　 【N】下一页　      **\n");
			printf("\t\t============================================\033[0m\n");
			printf("\t\tYour Choice:");
			scanf("\t\t%c", &choice);
			getchar();
			switch (choice)
			{
				case 'A': ;
				case 'a': 
									Seat_UI_Add(list,roomID,p.rowsCount,p.colsCount);			
									paging.totalRecords=Seat_Srv_FetchByRoomID(list,roomID);
									List_Paging(list,paging,seat_node_t);
									break;
				case 'D': 	;
				case 'd':
									printf("请输入你要删除的座位id：");
									scanf("%d",&seat_id);
									getchar();
									Seat_UI_Delete(seat_id);
									paging.totalRecords=Seat_Srv_FetchByRoomID(list,roomID);
									List_Paging(list,paging,seat_node_t);
									break;
				case 'r':
				case 'R':
									break;
				case 'm': 	;
				case 'M':
									printf("请输入你要修改的座位id：");
									scanf("%d",&seat_id);
									getchar();
									Seat_UI_Modify(seat_id);
									paging.totalRecords=Seat_Srv_FetchByRoomID(list,roomID);
									List_Paging(list,paging,seat_node_t);
									break;
				case 'p':
				case 'P':	
									if (1 < Pageing_CurPage(paging)) 
									{
											Paging_Locate_OffsetPage(list, paging, -1, seat_node_t);
									}	
									break;
				case 'n':
				case 'N':	
									if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
									{
											Paging_Locate_OffsetPage(list, paging, 1, seat_node_t);
									}
									break;
			}
		} while (choice != 'r' && choice != 'R');
	}
	else
	{
		printf("演出厅不存在！\n");
		printf("Press Any Key To Continue ......");
		getchar();
	}
}
char * Seat_Status(int i)
{
	if(i==1)
	{
		return "正常";
	}
	else if(i==0)
	{
		return "  \033[41m损坏\033[0m";
	}
	else
	{
		return "  \033[44m其他\033[0m";
	}
}
int Seat_UI_Modify(int seat_id)
{
	int choice,rtn=0;
	seat_t data,buf;
	if(Seat_Srv_FetchByID(seat_id,&data))
	{
		printf("\t\t==============修改座位状态==============\n");
		printf("\t\t当前状态：[ %s  ]\n",Seat_Status(data.status));
		printf("\t\t 请输入你要修改的状态; [1] 正常 [0] 损坏 [2]其他 \n");
		printf("Please Input :");
		scanf("%d",&choice);
		getchar();
		buf.id=data.id;
		buf.roomID=data.roomID;
		buf.row=data.row;
		buf.column=data.column;
		buf.status=choice;
		if(Seat_Srv_Modify(&buf))
		{
			printf("修改成功！\n");
			rtn=1;
			printf("Press Any Key To Continue ......");
			getchar();
		}
		else
		{
			printf("修改失败！\n");
			printf("Press Any Key To Continue ......");
			getchar();
		}
	}
	else
	{
		printf("未找到此座位 ! \n");
		printf("Press Any Key To Continue ......");
		getchar();
	}
	return rtn;
}


