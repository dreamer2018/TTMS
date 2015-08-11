#include "Ticket.h"
#include "EntityKey.h"
#include "../Common/List.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Persistence/Ticket_Persist.h"
#include "../Persistence/Play_Persist.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/Seat.h"
#include <stdio.h>
#include <stdlib.h>

int Ticket_Srv_AddBatch(int schedule_id, int studio_id)
{
	int count;
	seat_node_t *list,*pos;
	ticket_node_t *head,*p;
	List_Init(list,seat_node_t);
	count=Seat_Perst_FetchValidByRoomID(list,studio_id);
	if(0==count)
	{
		printf("座位数为零，批量新增票失败 ! 回车继续");
		getchar();
	}
	else
	{
		List_Init(head,ticket_node_t);
		pos=list->next;
		while(1)
		{
			if(list==pos)
			{
				Ticket_Perst_Insert(head);
				break;
			}
			else
			{
				p=(ticket_node_t*)malloc(sizeof(ticket_node_t));
				p->data.id=EntKey_Srv_CompNewKey("ticket");
				p->data.schedule_id=schedule_id;
				p->data.seat_id=pos->data.id;
				p->data.status=1;
				List_AddTail(head,p);
			}
			pos=pos->next;
		}
	}
	return count;
}

int Ticket_Srv_DeleteBatch(int schedule_id) 
{
	return Ticket_Perst_Delete(schedule_id);
}

int Ticket_Srv_Modify(const ticket_t * data)
{
	return Ticket_Perst_Update(data);
}

int Ticket_Srv_FetchByID(int ID, ticket_t *buf)
{
       return Ticket_Perst_SelectByID(ID,buf) ;
}


//根据座位ID在list中找对应票
/*
inline ticket_node_t * Ticket_Srv_FindBySeatID(ticket_list_t list, int seat_id)
{
       return Ticket_Perst_SelectBySchID(list,seat_id);
}*/


//根据计划ID提取所有演出票
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id) 
{
       return Ticket_Perst_SelectBySchID(list,schedule_id);
}

//根据演出计划ID，统计上座率及票房，返回票房输入
int Ticket_Srv_StatRevBySchID(int schedule_id, int *soldCount, int *totalCount)
{
       return 1;
}


