#include<stdio.h>
#include"Struct.h"
#include"../Persistence/Studio_Persist.h"
#include"Studio.h"
//添加新演出厅
//inline 
inline int Studio_Srv_Add(studio_t *data)
{
	int i=Studio_Perst_Insert(data);
	return i;
}

//修改演出厅服务
inline int Studio_Srv_Modify(studio_t *data)
{
	int i=Studio_Perst_Update(data);
	return i;
}

//删除演出厅服务函数
inline int Studio_Srv_DeleteByID(int ID)
{
	int i=Studio_Perst_DeleteByID(ID);
	return i;
}

//根据ID获取演出厅服务函数
inline int Studio_Srv_FetchByID(int ID,studio_t *buf)
{
	int i=Studio_Perst_SelectByID(ID,buf);
	return i;
}

//获取链表list上的所有演出厅服务函数
inline int Studio_Srv_FetchAll(studio_node_t *list)
{	
	int i;
	i=Studio_Perst_SelectAll(list);
	return i;
}

//根据ID在链表list中查找对应的放映厅结点 
inline studio_node_t *Studio_Srv_FindByID(studio_node_t *list,int ID)
{
	
	if(list==NULL)
		return NULL;
	else
	{
		studio_node_t *ptr;
		ptr=list->next;
		while(ptr!=list)
		{
			if(ptr->data.id==ID)
				return ptr;
			ptr=ptr->next;
		}
		return NULL;
	}
}
