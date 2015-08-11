
#include<stdio.h>
#include<stdlib.h>
#include"../Service/Struct.h"
#include"Schedule_UI.h"

#include"Play_UI.h"
#include"../Service/Seat.h"
#include"../Common/List.h"
#include"../Service/Schedule.h"
#include"../Service/Play.h"
#include"../Service/Studio.h"
#include"../Service/EntityKey.h"

static const int SCHEDULE_PAGE_SIZE = 10;

//显示所有演出计划
void Schedule_UI_MgtEntry(int play_id)
{
	int i,id;
	char choice;
	schedule_t buf;
	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;
	List_Init(head,schedule_node_t);//初始化链表
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords =Schedule_Srv_FetchByPlay(head,play_id);
	Paging_Locate_FirstPage(head,paging);
	do {
				system("clear");
				printf("\n\t\t\033[31m****************************** 计划表 ***************************\n");
				printf("\t%12s %13s%14s %16s %16s%14s\n", "ID", "剧目ID", "演出厅ID","放映日期","放映时间","座位数");
				printf("\t\t=================================================================\033[0m\n");
				Paging_ViewPage_ForEach(head,paging,schedule_node_t,pos,i)
				{
					printf("\t\t\033[32m  %2d%10d%12d    %4d-%2d-%2d    %2d:%2d:%2d%10d\n",pos->data.id,pos->data.play_id,pos->data.studio_id,pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
				}
				printf("\t\t\033[31m=================================================================\n");
				printf("\t\t---------总记录数:%2d ------------------------ 页数： %2d/%2d ------\n",paging.totalRecords, Pageing_CurPage(paging),	Pageing_TotalPages(paging));
				printf("\t\t*****************************************************************\n");
				printf("\t\t=================================================================\n");
				printf("\t\t**     [N]:下一页          [P]:上一页         [U]:  修改       **\n");       
				printf("\t\t**     [A]:  添加          [D]:  删除         [R]:  返回       **\n");  
				printf("\t\t=================================================================\033[0m\n");
		 		printf("\t\t Please Input :");
				fflush(stdin);
				scanf(" %c",&choice);
				fflush(stdin);
				switch(choice){
				case 'a':
				case 'A':
								 	if(Schedule_UI_Add(play_id))	
								 	{
										paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);
										Paging_Locate_LastPage(head, paging, schedule_node_t);
								   }
									break;
			 	case 'd':
				case 'D': 
									printf("\t\t请输入ID：");
									scanf("%d",&id);
									getchar();
									if(Schedule_Srv_FetchByID(id,&buf))
									{
										Schedule_UI_Delete(id);
									}
									else
									{
										printf("未找到！回车继续");
										getchar();
									}
									paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);
									List_Paging(head, paging, schedule_node_t);
									break;
				 case 'u':
				 case 'U':
									printf("\t\t请输入ID:");
									scanf("%d",&id);
									getchar();
										if(Schedule_UI_Modify(id))
										{
											paging.totalRecords =Schedule_Srv_FetchByPlay(head,play_id);
											List_Paging(head, paging, schedule_node_t);
										}
									break;
				case 'P':
				case 'p':
						  			 if (!Pageing_IsFirstPage(paging)) 
						  			 {
										Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
									}
									break;
				case 'n':
				case 'N':
									if (!Pageing_IsLastPage(paging)) 
									{
										Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
									}
									break;
									}		
		}while(choice !='r'&&choice !='R');
		List_Destroy(head,schedule_node_t);
}

//增加演出计划
//int Schedule_UI_Add(int play_id) 

int Schedule_UI_Add(int id) 
{
	studio_t buf;
	schedule_t rec;
	seat_list_t list;
	List_Init(list,seat_node_t);
	int art=0;
	char choice;
	rec.play_id=id;
	do
	{
		printf("请输入演出厅ID：");
		scanf("%d",&rec.studio_id);
		getchar();
		printf("放映日期 :");
		scanf("%d%d%d",&rec.date.year,&rec.date.month,&rec.date.day);
		getchar();
		printf("放映时间 :");
		scanf("%d%d%d",&rec.time.hour,&rec.time.minute,&rec.time.second);
		getchar();
		if(Studio_Srv_FetchByID(rec.studio_id,&buf))
		{
			choice='r';
			rec.seat_count=Seat_Srv_FetchValidByRoomID(list,buf.id);
			rec.id=EntKey_Srv_CompNewKey("schedule");
			break;
		}
		else
		{
			printf("您输入的演出厅不存在！\n");
			printf("Press Any Key To Countinue ......");
			getchar();
		}
	}while(choice !='r' || choice !='R');
	if(Schedule_Srv_Add(&rec)) 
	{
		printf("添加成功！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
		art = 1;
	}
	 else 
	{
		printf("添加失败！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
	}
	return art;
}

//修改

int Schedule_UI_Modify(int id)
{
	schedule_t buf,rec;
	int rnt = 0;
	if(!Schedule_Srv_FetchByID(id,&rec)) 
	{
		printf("ID不存在！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
		return rnt;
	}
	buf.play_id=rec.play_id;
	printf("请输入演出厅ID：");
	scanf("%d",&buf.studio_id);
	printf("放映日期 :");
	scanf("%d%d%d",&buf.date.year,&buf.date.month,&buf.date.day);
	printf("放映时间 :");
	scanf("%d%d%d",&buf.time.hour,&buf.time.minute,&buf.time.second);
	rec.seat_count=rec.seat_count;
	rec.id=id;
	if(Schedule_Srv_Modify(&rec)) 
	{
		printf("修改成功！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
		rnt = 1;
		return rnt;
 	}
 	else
 	{
		printf("修改失败！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
		return rnt;
	}
}
//删除
int Schedule_UI_Delete(int id)
{
	schedule_t rec;
	int rnt=0;
	if(!Schedule_Srv_DeleteByID(id)) 
	{
		printf("删除失败！\n");
		printf("Press Any Key To Countinue ......");
		getchar();
		return rnt;
	}
	printf("删除成功！\n");
	printf("Press Any Key To Countinue ......");
	getchar();
	rnt = 1;
	return rnt;
}
//根据ID查询
int Schedule_UI_Query(int id)
{
 	schedule_t rec;
	if(0==Schedule_Srv_FetchByID(id,&rec)) 
	{
		printf("ID不存在 ! \n");
		printf("Press Any Key To Countinue ......");
		getchar();
		return 0;
	}
	system("clear");
	printf("\t\t================演出计划=====================\n");
	printf("\t\tID : %d\n",rec.id); 
	printf("\t\t剧目ID : %d\n", rec.play_id);
	printf("\t\t演出厅ID : %d\n",rec.studio_id);
	printf("\t\t放映日期 : %d年%d月%d日\n",rec.date.year,rec.date.month,rec.date.day);
	printf("\t\t放映时间 : %d:%d:%d\n",rec.time.hour,rec.time.minute,rec.time.second);
	printf("\t\t座位数 : %d\n",rec.seat_count);
	printf("\t\t=============================================\n");
	printf("Press Any Key To Countinue ......");
	getchar();
	return 1;
}
/*以列表模式显示给定剧目的演出计划信息*/

void Schedule_UI_ListAll()
{
	int i,id;
	char choice;
	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;
	List_Init(head,schedule_node_t);//初始化链表
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head,paging);
	do {
				system("clear");
				printf("\n\t\t\033[34m******************************** 计划表 *************************\n");
				printf("\t\t%7s%12s%12s%15s%16s%14s\n", "ID", "剧目ID", "演出厅ID","放映日期","放映时间","座位数");
				printf("\t\t*****************************************************************\n");
				Paging_ViewPage_ForEach(head,paging,schedule_node_t,pos,i)
				{
					printf("\t\t\033[31m%7d%9d%8d%9d-%d-%d%6d:%d:%d%12d\033[0m\n",pos->data.id,pos->data.play_id,pos->data.studio_id,pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
				}
				printf("\t\t\033[34m*****************************************************************\n");
				printf("\t\t-------总记录数:%2d ------------------------- 页数： %2d/%2d -------\n",paging.totalRecords, Pageing_CurPage(paging),	Pageing_TotalPages(paging));
				printf("\t\t******************************************************************\n");
				printf("\t\t**    [N] : 下一页     [P] : 上一页    [Q] :  查询    [R] 返回  **\n");
				printf("\t\t==================================================================\033[0m\n");
		 		printf("\t\t Please Input :");
				fflush(stdin);
				scanf(" %c",&choice);
				fflush(stdin);
				switch(choice)
				{
					case 'q':
					case 'Q':
										printf("\t\t请输入ID：");
										scanf("%d",&id);
										getchar();
										Schedule_UI_Query(id);
										break;
					case 'P':
					case 'p':
							  			 if (!Pageing_IsFirstPage(paging)) 
							  			 {
											Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
										}
										break;
					case 'n':
					case 'N':
										if (!Pageing_IsLastPage(paging)) 
										{
											Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
										}
										break;
					case 'r':
					case 'R':
										break;
			}		
		}while(choice !='r'&&choice !='R');
		List_Destroy(head,schedule_node_t);
}
//根据剧目ID管理演出计划
//   Schedule_UI_menu(){}

