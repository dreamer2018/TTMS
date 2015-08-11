#include "Ticket_Persist.h"
//#include "../Persistence/Common_Persist.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>

static const char TICKET_DATA_FILE[]="Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[]="TicketTemp.dat";

int Ticket_Perst_Insert(ticket_list_t list)
{
	int rtn=0;
	FILE *fp;
	ticket_list_t p;
	p=list->next;
	ticket_t buf;
	fp=fopen(TICKET_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("打开文件失败!");
	}
	else
	{
		while(p!=list)
		{
			buf.id=p->data.id;
			buf.schedule_id=p->data.schedule_id;
			buf.seat_id=p->data.seat_id;
			buf.price=p->data.price;
			buf.status=p->data.status;
			rtn+=fwrite(&buf,sizeof(ticket_t),1,fp);
			p=p->next;
		}
	}
	fclose(fp);
	return rtn;
}
int Ticket_Perst_Delete(int schedule_id)
{
	FILE *fp1,*fp2;
	rename(TICKET_DATA_FILE,TICKET_DATA_TEMP_FILE);
	fp1= fopen(TICKET_DATA_TEMP_FILE,"rb");
	fp2 = fopen(TICKET_DATA_FILE,"wb");
	if(fp2 == NULL || fp1==NULL){
	        printf("文件打开失败!\n");
	        return 0;
	 }
	ticket_t buf;
	int found = 0;
	while(fread(&buf, sizeof(ticket_t), 1, fp2)) 
	{
			if (schedule_id != buf.schedule_id) 
			{
				fwrite(&buf, sizeof(ticket_t), 1, fp1);
			}
			found=1;
	}
	fclose(fp2);
	fclose(fp1);
	return found;
}

int Ticket_Perst_Update(const ticket_t * data) 
{
	FILE  *fp = fopen(TICKET_DATA_FILE,"rb+");
	if(fp == NULL)
	{
		printf("文件打开失败！\n");
		return 0;
	}
	ticket_t buf;
	int found = 0;
	while(!feof(fp)) 
	{
		if(fread(&buf,sizeof(ticket_t),1,fp))
		{
			if(buf.id == data->id)
			{
				fseek(fp,-sizeof(ticket_t),SEEK_CUR);
				fwrite(data,sizeof(ticket_t),1,fp);
				found=1;
			 }
		}
	}
	fclose(fp);
    return found; 
}
int Ticket_Perst_FetchByID(int ID,ticket_t *buf)
{
	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) 
	{
		return 0;
	}
	ticket_t data;
	int found = 0;
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)) 
		{
			if (ID == data.id) 
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
}

int Ticket_Perst_SelectByID(int ID, ticket_t *buf)
{
	int found=0;
        FILE *fp;
        fp=fopen(TICKET_DATA_FILE,"rb");
        if(fp==NULL)
        {
                printf("文件打开失败！");
                return 0;
        }
        else
        {
        	while(fread(buf,sizeof(ticket_t),1,fp)==1)
        	{
        		if(ID==buf->id)
        		{
        			found=1;
        			break;
        		}
        		
        	}
        }
        fclose(fp);
        return found;
}

int Ticket_Perst_SelectAll(ticket_list_t list) 
{
	int found=0;
	FILE *fp;
	fp=fopen(TICKET_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0; 
	}
	else
	{
		List_Free(list, ticket_node_t);
		ticket_t buf;
		ticket_node_t *p;
		p=(ticket_node_t *)malloc(sizeof(ticket_node_t));
		while(fread(&buf,sizeof(ticket_t),1,fp)==1)
		{
			p->data.id=buf.id;
			p->data.schedule_id=buf.schedule_id;
			p->data.seat_id=buf.seat_id;
			p->data.price=buf.price;
			p->data.status=buf.status;
			List_AddTail(list, p);
			p=(ticket_node_t *)malloc(sizeof(ticket_node_t));
			found++;
		}
	}
	fclose(fp); 
	return found;
}


int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id) {
	
    ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);
	
	List_Free(list,ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) 
	{
		return 0;
	}
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)) 
		{
			if(data.schedule_id == schedule_id)
			{
				newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
				 newNode->data = data;
				 List_AddTail(list, newNode);
				 recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

//根据票主键列表载入票
int Ticket_Perst_SelectByKeyList(ticket_list_t list, entkey_list_t keyList) 
{
	
}
