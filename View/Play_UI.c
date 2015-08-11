#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"


#include <stdio.h>
#include <stdlib.h>

void Play_UI_MgtEntry(int flag)
{
	int i,id,play_id;
	char choice;
	play_t buf;
	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;
	List_Init(head,play_node_t);
	paging.offset=0;
	paging.pageSize = PLAY_PAGE_SIZE;
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		system("clear");
		printf("\n\t\t\033[36m=============================================================================\n");
		printf("\t\t********************************* 剧目列表 **********************************\n");
		printf("\t\t    ID    剧名   类型   地区   等级 时长(分钟)  开始时间 结束时间   票价\n");
	printf("\t\t=============================================================================\033[0m\n");
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i){
			printf("\033[31m\t\t    %d%11s %s   %s  %5s   %d        %d-%d-%d  %d-%d-%d   %d\n\033[0m",pos->data.id,pos->data.name,type(pos->data.type),pos->data.area,rating(pos->data.rating),pos->data.duration,pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);
		}
		printf("\t\t\033[36m=============================================================================\n");
		printf("\t\t--------总记录数:%2d   ---------------------------------- 页数: %2d/%2d --------\n",

				paging.totalRecords, Pageing_CurPage(paging),

				Pageing_TotalPages(paging));

		printf("\t\t*****************************************************************************\n");
		printf("\t\t**  [P]   上一页      [N]  下一页      [A]添加       [R]   返回       　　 **\n");
		printf("\t\t**  [D]   删除        [U]  修改        [Q]查询       [S]  管理演出计划　 　**\n");
		printf("\t\t**  [H]   显示所有演出计划                                               　**\n");
		printf("\t\t*****************************************************************************\033[0m\n");
		printf("\t\tPlease Input :   ");
		scanf("%c", &choice);
		fflush(stdin);
		char fne[31];
		switch (choice) 
		{
		case 'a':
		case 'A':
							if (Play_UI_Add()) 
							{
								paging.totalRecords = Play_Srv_FetchAll(head);
								Paging_Locate_LastPage(head, paging, play_node_t);
							}
							break;
		case 'd':
		case 'D':
							printf("\t\t请输入剧目ID:");
							scanf("%d", &id);
							getchar();
							if(Play_Srv_FetchByID(id, &buf))
							{
								if (Play_UI_Delete(id))
								{
									printf("删除成功！\n");
									printf("Press Any Key To Continue ..............\n");
									getchar();
								}
							}
							else
							{
								printf("未找到！\n");
								printf("Press Any Key To Continue ..............\n");
								getchar();
							}
							paging.totalRecords = Play_Srv_FetchAll(head);
							List_Paging(head, paging, play_node_t);
							break;
		case 'h': ;
		case 'H':
							 Schedule_UI_ListAll();
							paging.totalRecords = Play_Srv_FetchAll(head);
							List_Paging(head, paging, play_node_t);
							break;
		case 'u':
		case 'U':
							printf("\t\t请输入剧目ID: ");
							scanf("%d", &id);
							getchar();
							if(Play_Srv_FetchByID(id, &buf))
							{
								if (Play_UI_Modify(id))
								{
									paging.totalRecords = Play_Srv_FetchAll(head);
									List_Paging(head, paging, play_node_t);
								}
								else
								{
									printf("修改失败！\n");
									printf("Press Any Key To Continue ..............\n");
									getchar();
								}
							}
							else
							{
								printf("未找到！\n");
								printf("Press Any Key To Continue ..............\n");
								getchar();
							}
							break;
		case 'q':
		case 'Q':
							printf("请输入剧目ID:");
							scanf("%d",&id);
							getchar();
							if(Play_Srv_FetchByID(id, &buf))
							{
								Play_UI_Query(id);
							}
							else
							{
								printf("未找到！\n");
								printf("Press Any Key To Continue ..............\n");
								getchar();
							}
						 	break;
		case 'p':	
		case 'P':
							if (!Pageing_IsFirstPage(paging))
							{
								Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
							}
							break;
		case 'n':
		case 'N':
							if (!Pageing_IsLastPage(paging)) 
							{
								Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
							}
							break;
		case 's':
		case 'S':
							printf("请输入剧目id :");
							scanf("%d",&play_id);
							getchar();
							if(Play_Srv_FetchByID(play_id, &buf))
							{
								Schedule_UI_MgtEntry(play_id);
							}
							else
							{
								printf("未找到！\n");
								printf("Press Any Key To Continue ..............\n");
								getchar();
							}
							break;
		}
	} while(choice !='r' && choice != 'R');
	List_Destroy(head,play_node_t);
}

int Play_UI_Add(void)
{
       play_t *p;
	int newCount=0;
	char choice;
	do {
				p=(play_t *)malloc(sizeof(play_t));
				printf("请输入剧目名称：");
				scanf("%s",p->name);
				getchar();
				printf("请输入类型：");
				scanf("%d",&p->type);
				getchar();
				printf("请输入出品地区：");
				scanf("%s",p->area);
				getchar();
				printf("请输入剧目等级：");
				scanf("%d",&p->rating);
				getchar();
				printf("请输入时长：");
				scanf("%d",&p->duration);
				getchar();
				printf("请输入上映时间(年月日空格隔开)：");
				scanf("%d%d%d",&p->start_date.year,&p->start_date.month,&p->start_date.day);
				getchar();
				printf("请输入结束放映时间(年月日空格隔开)：");
				scanf("%d%d%d",&p->end_date.year,&p->end_date.month,&p->end_date.day);
				getchar();
				printf("请输入票价：");
				scanf("%d",&p->price);
				getchar();
				p->id=EntKey_Srv_CompNewKey("Play");
				if(Play_Srv_Add(p))
				{
					newCount++;
					printf("添加成功！\n");
				} else 
				{
					printf("添加失败！\n");
				}
				printf("  [A]  继续 or  [R]  退出 : ");
				scanf("%c", &choice);
	}while ('a' ==choice||'A'==choice);
       return newCount;
}

int Play_UI_Modify(int id)
{
	int rtn=0;
	play_t *buf,*p;
	buf=(play_t *)malloc(sizeof(play_t));
	p=(play_t*)malloc(sizeof(play_t));
	if(Play_Srv_FetchByID(id,buf))
	{
		printf("\n\t\t=================旧的剧目信息======================\n");
		printf("\t\t剧目名称: %s\n",buf->name);
		printf("\t\t类型：%s\n",rating(buf->type));
		printf("\t\t出品地区：%s\n",buf->area);
		printf("\t\t剧目等级：%s\n",rating(buf->rating));
		printf("\t\t时长: %d 分钟\n",buf->duration);
		printf("\t\t上映时间: %d年%d月 %d日\n",buf->start_date.year,buf->start_date.month,buf->start_date.day);
		printf("\t\t结束放映时间：%d年%d月%d日\n",buf->end_date.year,buf->end_date.month,buf->end_date.day);
		printf("\t\t票价：%d 元\n",buf->price);
		printf("\t\t======================================================\n");
		p->id=buf->id;
		printf("请输入新剧目名称：");
		scanf("%s",p->name);
		getchar();
		printf("请输新入类型：");
		scanf("%d",&p->type);
		getchar();
		printf("请输入新出品地区：");
		scanf("%s",p->area);
		getchar();
		printf("请输入新剧目等级：");
		scanf("%d",&p->rating);
		getchar();
		printf("请输入新时长：");
		scanf("%d",&p->duration);
		getchar();
		printf("请输入新上映时间(年月日空格隔开)：");
		scanf("%d%d%d",&p->start_date.year,&p->start_date.month,&p->start_date.day);
		getchar();
		printf("请输入新结束放映时间(年月日空格隔开)：");
		scanf("%d%d%d",&p->end_date.year,&p->end_date.month,&p->end_date.day);
		getchar();
		printf("请输入新票价：");
		scanf("%d",&p->price);
		getchar();
		if(Play_Srv_Modify(p))
		{
			printf("修改成功！\n");
			printf("Press Any Key To Continue ..............\n");
			getchar();
			rtn=1;
		}
		else
		{
			printf("修改失败！\n");
			printf("Press Any Key To Continue ..............\n");
			getchar();
		}
	}
	else
	{
		printf("找不到此剧目！\n");
		printf("Press Any Key To Continue ..............\n");
		getchar();
	}
	return rtn;
}

int Play_UI_Delete(int id)
{
	int rtn = 0;
	play_t rec;
       if(Play_Srv_DeleteByID(id))
       {
		printf("删除成功！\n");
		printf("Press Any Key To Continue ..............\n");
		getchar();
		rtn = 1;
	} else 
	{
		printf("该剧目不存在！\n");
		printf("Press Any Key To Continue ..............\n");
		getchar();
	}
	getchar();
	return rtn;
}
int Play_UI_Query(int id)
{
	int rtn=0;
	play_t *buf;
	buf=(play_t *)malloc(sizeof(play_t));
	if(Play_Srv_FetchByID(id,buf))
	{
		printf("\n\t\t=================剧目信息======================\n");
		printf("\t\t剧目名称: %s\n",buf->name);
		printf("\t\t类型：%s\n",type(buf->type));
		printf("\t\t出品地区：%s\n",buf->area);
		printf("\t\t剧目等级：%s\n",rating(buf->rating));
		printf("\t\t时长: %d 分钟\n",buf->duration);
		printf("\t\t上映时间: %d年%d月 %d日\n",buf->start_date.year,buf->start_date.month,buf->start_date.day);
		printf("\t\t结束放映时间：%d年%d月%d日\n",buf->end_date.year,buf->end_date.month,buf->end_date.day);
		printf("\t\t票价：%d 元\n",buf->price);
		printf("\t\t======================================================\n");
		rtn=1;
		getchar();
		getchar();
	}
	else
	{
		printf("未找到！");
	}
	return rtn;
}
char *type(int type)
{
	if(type==1)
	{
		return "儿童剧" ;
	}
	else if(type==2)
	{
		return "少儿剧" ;
	}
	else if(type==3)
	{
		return "成人剧";
	}
	else
	return "其他";
}
char *rating(int rating)
{
		if(rating==1)
	{
		return "2D" ;
	}
	else if(rating==2)
	{
		return "3D" ;
	}
	else if(rating==3)
	{
		return "IMAX" ;
	}
	else
	return "Other" ;
}

/*
typedef enum {
	PLAY_TYPE_FILE=1,						//儿童剧
	PLAY_TYPE_OPEAR=2,					//少儿剧
	PLAY_TYPE_CONCERT=3				//成人剧
} play_type_t;

typedef enum {
	PLAY_RATE_CHILD = 1,						// 2D
	PLAY_RATE_TEENAGE = 2,					//3D
	PLAY_RATE_ADULT = 3							//IMAX
} play_rating_t;
*/

