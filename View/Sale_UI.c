#include"../Service/Struct.h"
#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"
#include"../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGESIZE 10
//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票
void Sale_UI_ShowTicket(int schID)
{
		int pageSize=10, i;
		int listSize;
		int ticket_id;
		char choice;
		ticket_list_t list,s;
		seat_t buf;
		Pagination_t paging;
		List_Init(list,ticket_node_t);
		listSize=Ticket_Srv_FetchBySchID(list, schID);
		system("clear");
		paging.pageSize = pageSize;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do
		{
			system("clear");
			printf("\n\t\t\033[34m=====================影票列表======================\n");
			printf("\t\t%6s %17s%10s%8s%8s%8s\n","id","演出计划id","座位id","行","列","状态");
			printf("\t\t---------------------------------------------------\n");
			for (i = 0, s= (ticket_node_t *) (paging.curPos);s != list && i < paging.pageSize; i++, s=s->next) 
			{
					Seat_Srv_FetchByID(s->data.seat_id,&buf);
					if(buf.status==1)
						printf("\033[31m\t\t%6d%10d%11d%7d%7d    %12s\n\033[0m",s->data.id,s->data.schedule_id, s->data.seat_id,buf.row+1, buf.column+1,Status(s->data.status));
			}
			printf("\t\t\033[34m---------------------------------------------------\n");
			printf("\t\t------总行数 :%2d --------------页码： %2d/%2d -------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("\t\t===================================================\n");
			printf("\t\t**    【R】返回上一级　　　  【B】订票　　　   　**\n");
			printf("\t\t**    【P】上一页　　    　　【N】下一页　       **\n");
			printf("\t\t**    【T】 退票　　    　　            　       **\n");
			printf("\t\t===================================================\033[0m\n");
			printf("\t\tYour Choice:");
			scanf("\t\t%c", &choice);
			getchar();
			switch (choice)
			{
				case 'B': 	;
				case 'b':
									printf("请输入你要订购的票的id：");
									scanf("%d",&ticket_id);
									getchar();
									Sale_UI_SellTicket(ticket_id);
									paging.totalRecords=Ticket_Srv_FetchBySchID(list, schID);
									List_Paging(list,paging,ticket_node_t);
									break;
				case 'r':
				case 'R':
									break;
				case 't':
				case 'T':
									printf("请输入你要退的票的id：");
									scanf("%d",&ticket_id);
									getchar();
									Sale_UI_ReturnTicket(ticket_id);
									paging.totalRecords=Ticket_Srv_FetchBySchID(list, schID);
									List_Paging(list,paging,ticket_node_t);
									break;
				case 'p':
				case 'P':	
									if (1 < Pageing_CurPage(paging)) 
									{
											Paging_Locate_OffsetPage(list, paging, -1, ticket_node_t);
									}	
									break;
				case 'n':
				case 'N':	
									if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
									{
											Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
									}
									break;
			}
		} while (choice != 'r' && choice != 'R');
}
void Sale_UI_ReturnTicket(int ticket_id)
{
	ticket_t buf,data;
	if(Ticket_Srv_FetchByID(ticket_id,&buf))
	{
			if(buf.status==1)
			{
				printf("此票未售出！回车继续");
				getchar();
			}
			else
			{
				data.id=buf.id;
				data.schedule_id=buf.schedule_id;
				data.seat_id=buf.seat_id;
				data.price=buf.price;
				data.status=1;
				if(Ticket_Srv_Modify(&data))
				{
					printf("退订成功！回车继续");
					getchar();
				}
				else
				{
					printf("退订失败！回车继续");
					getchar();
				}
			}
	}
	else
	{
		printf("找不到此票！回车继续");
		getchar();
	}
}
inline int Sale_UI_SellTicket(int ticket_id)
{
	ticket_t buf,data;
	if(Ticket_Srv_FetchByID(ticket_id,&buf))
	{
			if(buf.status==0)
			{
				printf("此票已售出！回车继续");
				getchar();
			}
			else
			{
				data.id=buf.id;
				data.schedule_id=buf.schedule_id;
				data.seat_id=buf.seat_id;
				data.price=buf.price;
				data.status=0;
				if(Ticket_Srv_Modify(&data))
				{
					Ticket_Info(&data);
					printf("订购成功！回车继续");
					getchar();
				}
				else
				{
					printf("订购失败！回车继续");
					getchar();
				}
			}
	}
	else
	{
		printf("找不到此票！回车继续");
		getchar();
	}
}

void Ticket_Info(ticket_t *data)
{
	schedule_t p;
	play_t buf;
	seat_t s;
	Seat_Srv_FetchByID(data->seat_id,&s);
	Schedule_Srv_FetchByID(data->schedule_id,&p); 
	if(Play_Srv_FetchByID(p.play_id,&buf))
	{
		printf("\n\t\t====================影票信息======================\n");
		printf("\t\t剧目名称: %s\n",buf.name);
		printf("\t\t类型：%s\n",type(buf.type));
		printf("\t\t出品地区：%s\n",buf.area);
		printf("\t\t剧目等级：%s\n",rating(buf.rating));
		printf("\t\t时长: %d 分钟\n",buf.duration);
		printf("\t\t上映时间: %d年%d月 %d日\n",buf.start_date.year,buf.start_date.month,buf.start_date.day);
		printf("\t\t放映日期: %d时%d分%d秒\n",p.time.hour,p.time.minute,p.time.second);
		printf("\t\t结束放映时间：%d年%d月%d日\n",buf.end_date.year,buf.end_date.month,buf.end_date.day);
		printf("\t\t票价：%d 元\n",buf.price);
		printf("\t\t演出厅id :%d\n",p.studio_id);
		printf("\t\t位置: %d行%d列\n",s.row+1,s.column+1);
		printf("\t\t======================================================\n");
		printf("Press Any Key To Continue......");
		getchar();
	}
}
//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID)
{
	play_t buf;
	if(Play_Srv_FetchByID(playID,&buf))
	{
		int listSize,sal_id,i;
		char choice;
		Pagination_t paging;
		schedule_list_t list,pos;
		schedule_t buf;
		List_Init(list,schedule_node_t);
		listSize=Schedule_Srv_FetchByPlay(list,playID);
		paging.pageSize =PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do {
				system("clear");
				printf("\n\t\t\033[32m************************* 计划表 **************************\n");
				printf("\t\t    %2s  %10s  %8s  %10s  %10s   %10s\n", "ID", "剧目ID", "演出厅ID","放映日期","放映时间","座位数");
				printf("\t\t***********************************************************\n");
				Paging_ViewPage_ForEach(list,paging,schedule_node_t,pos,i)
				{
					printf("\033[34m\t\t    %2d%10d%8d%6d-%2d-%2d%4d:%2d:%2d%10d\n",pos->data.id,pos->data.play_id,pos->data.studio_id,pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
				}
				printf("\t\t\033[32m-----------------------------------------------------------\n");
				printf("\t\t-------总记录数:%2d ------------------- 页数： %2d/%2d -------\n",paging.totalRecords, Pageing_CurPage(paging),	Pageing_TotalPages(paging));
				printf("\t\t-----------------------------------------------------------\n");
				printf("\t\t** 【P】上一页 【[N】下一页 【R】返回 【S】进入售票系统  **\n");
				printf("\t\t===========================================================\033[0m\n");
				printf("\t\tYour Choice:");
				fflush(stdin);
				scanf("\t\t%c", &choice);
				getchar();
				switch (choice)
				{
					case 'S': ;
					case 's':
									printf("请输入演出计划id :");
									scanf("%d",&sal_id);
									getchar();
									if(!Schedule_Srv_FetchByID(sal_id,&buf ))
									{
										printf("你输入的演出计划不存在！回车继续");
										getchar();
									}
									else
									{
										Sale_UI_ShowTicket(sal_id);
									}
									paging.totalRecords=Schedule_Srv_FetchByPlay(list,playID);
									List_Paging(list, paging, schedule_node_t);
							      	break;
					case 'r':
					case 'R':break;
					case 'p':
					case 'P':
							if (1 < Pageing_CurPage(paging))
							{
								Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
							}
							choice='r';
							break;
					case 'n':
					case 'N':
							if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
							{
								Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
							}
							choice='r';
							break;
			}
		} while (choice != 'r' && choice != 'R');
	}
	else
	{
		printf("Not Found !");
		getchar();
	}
}

void Sale_UI_MgtEntry()
{
	
	int listSize,i,id;
	char choice;
	char name[31];
	Pagination_t paging;
    play_list_t list,p,data,pos;
    List_Init(data,play_node_t);
    List_Init(list,play_node_t);
	listSize=Play_Srv_FetchAll(list);
	paging.pageSize = PAGESIZE;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do {
				system("clear");
				printf("\n\t\t\033[36m========================================== 剧目列表 ========================================\n");
				printf("\t\t--------------------------------------------------------------------------------------------\n");
				printf("\t\t id     名称    类型   出品地区  等级   时长(min)    开始放映日期     结束放映日期    票价\n");
				printf("\t\t--------------------------------------------------------------------------------------------\n");
				for (i = 0, p = (play_node_t *) (paging.curPos);p != list && i < paging.pageSize; i++, p=p->next) 
				{
						printf("\033[31m\t\t%3d%11s%12s%7s%8s%8d%13d-%d-%d %13d-%d-%d %8d\033[0m\n",p->data.id,p->data.name, type(p->data.type),p->data.area,rating( p->data.rating),p->data.duration,p->data.start_date.year,p->data.start_date.month,p->data.start_date.day,p->data.end_date.year,p->data.end_date.month,p->data.end_date.day,p->data.price);
				}
				printf("\t\t\033[36m--------------------------------------------------------------------------------------------\n");
				printf("\t\t------总行数 :%2d ----------------------------------------------------- 页码： %2d/%2d --------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
				printf("\t\t============================================================================================\n");
				printf("\t\t**   【P】上一页               【R】返回                     【S】查询剧目(By Name)       **\n");
				printf("\t\t**   【N】下一页               【C】显示演出计划	                                  **\n");
				printf("\t\t============================================================================================\033[0m\n");
				printf("\t\tYour Choice:");
				fflush(stdin);
				scanf("\t\t%c", &choice);
				switch (choice)
				{
					case 'C': ;
					case 'c':
										printf("\t\t请输入您要查询的剧目id:");
										scanf("%d",&id);
										getchar();
									 	Sale_UI_ShowScheduler(id);
									 	paging.totalRecords=Play_Srv_FetchAll(list);
										List_Paging(list, paging, play_node_t);
									 	break;
					case 'S':
					case 's':
										printf("\t\t请输入您要查询的剧目名称:");
										scanf("%s",name);
										getchar();
										if(Play_Srv_FetchByName(data,name))
										{
											Print_Play(data);
										}
										else
										{
											printf("Not Found!");
											getchar();
										}
										paging.totalRecords=Play_Srv_FetchAll(list);
										List_Paging(list, paging, play_node_t);
										break;
					case 'r':
					case 'R':
										break;
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
			}while(choice !='r'&&choice !='R');
			List_Destroy(list,play_node_t);
}
void Print_Play(play_list_t list)
{
		play_list_t p;
		p=list->next;
		system("clear");
		do
		{
			printf("\n\t\t=================剧目信息======================\n");
			printf("\t\t剧目id: %d\n",p->data.id);
			printf("\t\t剧目名称: %s\n",p->data.name);
			printf("\t\t类型：%s\n",type(p->data.type));
			printf("\t\t出品地区：%s\n",p->data.area);
			printf("\t\t剧目等级：%s\n",rating(p->data.rating));
			printf("\t\t时长: %d 分钟\n",p->data.duration);
			printf("\t\t上映时间: %d年%d月 %d日\n",p->data.start_date.year,p->data.start_date.month,p->data.start_date.day);
			printf("\t\t结束放映时间：%d年%d月%d日\n",p->data.end_date.year,p->data.end_date.month,p->data.end_date.day);
			printf("\t\t票价：%d 元\n",p->data.price);
			printf("\t\t======================================================\n");
			p=p->next;
		}while(p!=list);
		printf("Press Any Key To Continue......");
		getchar();
}
char * Status(int i)
{
	if(i==1)
	{
		return "\033[32m未订\033[0m";
	}
	else if(i==0)
	{
		return "\033[43m已订\033[0m";
	}
	else if(i==-1)
	{
		return "损坏";
	}
	else
	{
		return "其他";
	}
}

