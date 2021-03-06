#include "Play_Persist.h"
#include "../Service/Play.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";

//将参数所指向的剧目信息写入到文件中
int Play_Perst_Insert(const play_t *data) 
{
	int rtn=0;
	FILE *fp;
	fp=fopen(PLAY_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("文件打开失败！");
	}
	else
	{
		fwrite(data,sizeof(play_t ),1,fp);
		rtn=1;
	}
	fclose(fp);
	return rtn;
}

//将参数所指向的剧目信息更新到剧目文件中
int Play_Perst_Update(const play_t *data) 
{
	FILE *fp = fopen(PLAY_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("打开文件失败！");
		return 0;
	}
	play_t buf;
	int found = 0;
	while (!feof(fp)) 
	{
		if (fread(&buf, sizeof(play_t), 1, fp)) 
		{
			if (buf.id == data->id) 
			{
				fseek(fp, -sizeof(play_t), SEEK_CUR);
				fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
}

//按照id从文件中删除剧目数据
int Play_Perst_DeleteByID(int ID) 
{
	int found=0;
	play_t *buf;
	buf=(play_t *)malloc(sizeof(play_t));
	rename(PLAY_DATA_FILE,PLAY_DATA_TEMP_FILE);
    	FILE *fp1,*fp2;
    	fp1=fopen(PLAY_DATA_TEMP_FILE,"rb");
    	fp2=fopen(PLAY_DATA_FILE,"wb");
	if(fp1==NULL||fp2==NULL)
    	{
		printf("不能打开此文件!");
		return 0;
	} 
	else
	 {
		while(fread(buf,sizeof(play_t),1,fp1)==1)
	    	{
			if(buf->id!=ID)
			{
				fwrite(buf,sizeof(play_t),1,fp2);
			}
			found=1;
		}
	}
	fclose(fp1);
	fclose(fp2);
	return found;
}

//按照id从文件中获取剧目数据
int Play_Perst_SelectByID(int ID, play_t *buf) 
{
	 int found=0;
        FILE *fp;
        fp=fopen(PLAY_DATA_FILE,"rb");
        if(fp==NULL)
        {
                printf("文件打开失败！");
                return 0;
        }
        else
        {
        	while(fread(buf,sizeof(play_t),1,fp)==1)
        	{
        		if(ID==buf->id)
        			found=1;
        	}
        }
        fclose(fp);
        return found;
}

//
int Play_Perst_SelectAll(play_list_t list) 
{
	int found=0;
	FILE *fp;
	fp=fopen(PLAY_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0; 
	}
	else
	{
		List_Free(list, play_node_t);
		play_t buf;
		play_node_t *p;
		p=(play_node_t *)malloc(sizeof(play_node_t));
		while(fread(&buf,sizeof(play_t),1,fp)==1)
		{
			p->data.id=buf.id;
			strcpy(p->data.name,buf.name);
			p->data.type=buf.type;
			strcpy(p->data.area,buf.area);
			p->data.rating=buf.rating;
			p->data.duration=buf.duration;
			p->data.start_date=buf.start_date;
			p->data.end_date=buf.end_date;
			p->data.price=buf.price;
			List_AddTail(list, p);
			p=(play_node_t *)malloc(sizeof(play_node_t));
			found++;
		}
	}
	fclose(fp); 
	return found;
}

int Play_Perst_SelectByName(play_list_t list, char condt[]) 
{
	play_node_t *p;
	play_t data;
	int recCount = 0;
	List_Free(list, play_node_t);
	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) 
	{
		printf("文件打开失败！回车继续");
		return 0;
	}
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(play_t), 1, fp)) 
		{
			if(strcmp(data.name,condt)==0)
			{
				p=(play_node_t*) malloc(sizeof(play_node_t));
				p->data = data;
				List_AddTail(list, p);
				recCount++;
			}
		}
	}
	fclose(fp);
	return recCount;
}

