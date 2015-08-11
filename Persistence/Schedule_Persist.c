
#include "Schedule_Persist.h"
#include "../Service/Schedule.h"
#include "../Common/List.h"
#include "../Service/Ticket.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";

int Schedule_Perst_Insert(const schedule_t *data)
{
	int found=0;
	FILE *fp;
	fp=fopen(SCHEDULE_DATA_FILE,"ab");
	if(fp==NULL) 
	{
		printf("打开文件失败!\n");
	}
	else
	{
		fwrite(data,sizeof(schedule_t),1,fp);
		fclose(fp);
		found = 1;
    }
    return found;
}
int Schedule_Perst_Update(const schedule_t *data)
{
	FILE *fp=fopen(SCHEDULE_DATA_FILE,"rb+");
	if(fp==NULL)
	{
		printf("打开文件失败！回车继续");
		getchar();
		return 0;
	}
	schedule_t buf;
	int found=0;
	while (!feof(fp))
	{
		if(fread(&buf,sizeof(schedule_t),1,fp))
		{
			if(buf.id==data->id)
			{
				fseek(fp,-sizeof(schedule_t),SEEK_CUR);
				fwrite(data,sizeof(schedule_t),1,fp);
				found=1;
				break;
			}
		}
	}		
	fclose(fp);
	return found;
}

int Schedule_Perst_SelectByID(int ID, schedule_t *buf)
{
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) 
	{
		return 0;
	}
	schedule_t data;
	int found = 0;
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(schedule_t), 1, fp)) 
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
int Schedule_Perst_DeleteByID(int ID)
{
	rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE);
	FILE *fp1, *fp2;
	fp1 = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	fp2 = fopen(SCHEDULE_DATA_FILE, "wb");
	if (NULL == fp1 || NULL ==fp2 )
	{
		printf("打开文件失败！");
		return 0;
	}
	schedule_t buf;
	int found = 0;
	while(fread(&buf, sizeof(schedule_t), 1, fp1)) 
	{
		if (ID != buf.id) 
		{
			fwrite(&buf, sizeof(schedule_t), 1, fp2);
			found++;
		}
	}
	fclose(fp2);
	fclose(fp1);
	return found;
}

int Schedule_Perst_SelectAll(schedule_list_t list)
{
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;
	List_Free(list, schedule_node_t);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("文件打开失败！\n");
		printf("Press Any  Key To Continue......");
		getchar();
	}
	else
	{
		while (fread(&data, sizeof(schedule_t), 1, fp)) 
		{
				newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
				newNode->data = data;
				List_AddTail(list, newNode);
				recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;
	List_Free(list,schedule_node_t);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) 
	{
		return 0;
	}
	int found = 0;
	List_Free(list, schedule_node_t);
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(schedule_t), 1, fp)) 
		{
			if (play_id==data.play_id)
			{
				newNode=(schedule_node_t*)malloc(sizeof(schedule_node_t));
				newNode->data=data;
				List_AddTail(list,newNode);
				recCount++;	
			}
		}
	}
	fclose(fp);
	return recCount;
}
