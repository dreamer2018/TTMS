#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";


//在文件中存入单个座位函数
int Seat_Perst_Insert(const seat_t *p) 
{
	int rtn=0;
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("文件打开失败！");
		rtn=0;
	}
	else
	{
		fwrite(p,sizeof(seat_t),1,fp);
		rtn=1;
	}
	fclose(fp);
	return rtn;
}


//在文件中批量存入座位函数
int Seat_Perst_InsertBatch(const seat_list_t list) 
{
	int rtn=0;
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("\t\t文件打开失败！\n");
	}
	else
	{
		seat_t data;
		seat_node_t  *p1;
		seat_node_t *p2;
		p2=list->next;
		while(p2!=list)
		{
		
			p1=(seat_node_t *)malloc(sizeof(seat_node_t));
			p1->data.id=p2->data.id;
			p1->data.roomID=p2->data.roomID;
			p1->data.row=p2->data.row;
			p1->data.column=p2->data.column;
			p1->data.status=1;
			data=p1->data;
			fwrite(&data,sizeof(seat_t),1,fp);
			p2=p2->next;
			rtn++;
		}
		fclose(fp);
	}
	return rtn;
}


//在文件中更新座位

int Seat_Perst_Update(const seat_t *data) 
{
	int found=0; 
    FILE *fp;
    seat_t buf;
    fp=fopen(SEAT_DATA_FILE,"rb+");
    if(fp==NULL)
    {
    	   printf("文件打开失败！");
    	   return 0;
    }
    else
    {
    	while(fread(&buf,sizeof(seat_t),1,fp)==1)
        {
        	if(buf.id==data->id)
        	{
        		fseek(fp,-sizeof(seat_t),1);
        		fwrite(data,sizeof(seat_t),1,fp);
        		found=1;
        		break;
        	}
        }
    }
    fclose(fp);
    return found;
}

int Seat_Perst_DeleteByID(int ID) 
{
	int found=0;
	seat_t *buf;
	buf=(seat_t *)malloc(sizeof(seat_t));
	rename(SEAT_DATA_FILE,SEAT_DATA_TEMP_FILE);
    FILE *fp1,*fp2;
    fp1=fopen(SEAT_DATA_TEMP_FILE,"rb");
    fp2=fopen(SEAT_DATA_FILE,"wb");
    if(fp1==NULL)
    {
        printf("不能打开此文件!");
    } 
    else
    {
    	while(fread(buf,sizeof(seat_t),1,fp1)==1)
    	{
	        if(buf->id!=ID)
	        {
	        	fwrite(buf,sizeof(seat_t),1,fp2);
	        }
	    }
	    found=1;
    }
    fclose(fp1);
    fclose(fp2);
    return found;
}

int Seat_Perst_DeleteAllByRoomID(int roomID) 
{
	int found=0;
	seat_t *buf;
	buf=(seat_t *)malloc(sizeof(seat_t));
	rename(SEAT_DATA_FILE,SEAT_DATA_TEMP_FILE);
    FILE *fp1,*fp2;
    fp1=fopen(SEAT_DATA_TEMP_FILE,"rb");
    fp2=fopen(SEAT_DATA_FILE,"wb");
    if(fp1==NULL)
    {
        printf("不能打开此文件!");
        return 0;
    } 
    else
    {
    	while(fread(buf,sizeof(seat_t),1,fp1)==1)
    	{
	        if(buf->id!=roomID)
	        {
	        	fwrite(buf,sizeof(seat_t),1,fp2);
	        }
	        found=1;
	    }
    }
    fclose(fp1);
    fclose(fp2);
    return found;
}

//根据id获取座位函数
int Seat_Perst_SelectByID(int ID, seat_t *p) 
{
	 int found=0;
        FILE *fp;
        fp=fopen(SEAT_DATA_FILE,"rb");
        if(fp==NULL)
        {
                printf("文件打开失败！");
        }
        else
        {
        	while(fread(p,sizeof(seat_t),1,fp)==1)
        	{
        		if(ID==p->id)
        		{
        			found=1;
        			break;
        		}
        	}
        }
        fclose(fp);
        return found;
}

//在文件中获取所有座位
int Seat_Perst_SelectAll(seat_node_t *head) 
{
	int found=0;
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0; 
	}
	else
	{
		List_Free(head, seat_node_t);
		seat_t buf;
		seat_node_t *p;
		p=(seat_node_t *)malloc(sizeof(seat_node_t));
		while(fread(&buf,sizeof(seat_t),1,fp)==1)
		{
			p->data=buf;
			List_AddTail(head, p);
			p=(seat_node_t *)malloc(sizeof(seat_node_t));
			found++;
		}
	}
	fclose(fp); 
	return found;
}

int Seat_Perst_FetchValidByRoomID(seat_list_t list, int roomID)
{
	int found=0;
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败! \n");
		return 0;
	}
	else
	{
		List_Free(list, seat_node_t);
		seat_t buf;
		seat_node_t *p;
		while(fread(&buf,sizeof(seat_t),1,fp)==1)
		{
			if(buf.roomID==roomID && buf.status==1)
			{
				p=(seat_node_t *)malloc(sizeof(seat_node_t));
				p->data=buf;
				List_AddTail(list, p);
				found++;
			}
		}
	}
	fclose(fp); 
	return found;
	
}
//根据演出厅id从文件中获取所有座位
int Seat_Perst_SelectByRoomID(seat_node_t *list, int roomID)
 {
	int found=0;
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败! \n");
		return 0;
	}
	else
	{
		List_Free(list, seat_node_t);
		seat_t buf;
		seat_node_t *p;
		while(fread(&buf,sizeof(seat_t),1,fp)==1)
		{
			if(buf.roomID==roomID)
			{
				p=(seat_node_t *)malloc(sizeof(seat_node_t));
				p->data.id=buf.id;
				p->data.roomID=buf.roomID;
				p->data.column=buf.column; 
				p->data.row=buf.row;
				p->data.status=buf.status;
				List_AddTail(list, p);
				found++;
			}
		}
	}
	fclose(fp); 
	return found;
}

