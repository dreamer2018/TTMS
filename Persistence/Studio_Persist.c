
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../Service/Struct.h"
#include"../Service/Studio.h"
#include"../Common/List.h"
#include"Studio_Persist.h"

//持久化层


//定义储存所有放映厅信息的文件名
//在文件中存入演出厅函数
int Studio_Perst_Insert( studio_t  *data)
{
	int rtn=0;
	FILE *fp;
	fp=fopen("Studio.dat","ab");
	if(fp==NULL)
	{
		printf("文件打开失败！");
		rtn=0;
	}
	else
	{
		fwrite(data,sizeof(studio_t),1,fp);
		rtn=1;
	}
	fclose(fp);
	return rtn;
}

//在文件中更新演出厅函数 
int Studio_Perst_Update( studio_t *data)
{
    int found=0; 
    FILE *fp;
    studio_t buf;
    fp=fopen("Studio.dat","rb+");
    if(fp==NULL)
    {
    	   printf("文件打开失败！");
    	   return 0;
    }
    else
    {
    	while(fread(&buf,sizeof(studio_t),1,fp)==1)
        {
        	if(buf.id==data->id)
        	{
        		fseek(fp,-sizeof(studio_t),1);
        		fwrite(data,sizeof(studio_t),1,fp);
        		found=1;
        		break;
        	}
        }
    }
    fclose(fp);
    return found;
}

//根据ID在文件中获取演出厅函数 
int Studio_Perst_SelectByID(int ID,studio_t *p) 			//head为链表的头节点
{
        int found=0;
        FILE *fp;
        studio_t buf;
        fp=fopen("Studio.dat","rb");
        if(fp==NULL)
        {
                printf("文件打开失败！");
                return 0;
        }
        else
        {
        	while(fread(&buf,sizeof(studio_t),1,fp)==1)
        	{
        		if(ID==buf.id)
        		{
        			*p=buf;
        			found=1;
        			break;
        		}
        	}
        }
        fclose(fp);
        return found;
}
//在文件中获取所有演出厅函数 
int Studio_Perst_SelectAll(studio_node_t *head)
{
	int found=0;
	FILE *fp;
	fp=fopen("Studio.dat","rb");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0; 
	}
	else
	{
		List_Free(head, studio_node_t);
		studio_t buf;
		studio_node_t *p;
		p=(studio_node_t *)malloc(sizeof(studio_node_t));
		while(fread(&buf,sizeof(studio_t),1,fp)==1)
		{
			p->data.id=buf.id;
			strcpy(p->data.name,buf.name);
			p->data.colsCount=buf.colsCount; 
			p->data.rowsCount=buf.rowsCount;
			List_AddTail(head, p);
			p=(studio_node_t *)malloc(sizeof(studio_node_t));
			found++;
		}
	}
	fclose(fp); 
	return found;
}
 

//根据ID在文件中删除演出厅函数 
int Studio_Perst_DeleteByID(int ID)
{  
	int found=0;
	studio_t *buf;
	buf=(studio_t *)malloc(sizeof(studio_t));
	rename("Studio.dat","Studiotmp.dat");
    FILE *fp1,*fp2;
    fp1=fopen("Studiotmp.dat","rb");
    fp2=fopen("Studio.dat","wb");
    if(fp1==NULL)
    {
        printf("不能打开此文件!");
        return 0;
    } 
    else
    {
    	while(fread(buf,sizeof(studio_t),1,fp1)==1)
    	{
	        if(buf->id!=ID)
	        {
	        	fwrite(buf,sizeof(studio_t),1,fp2);
	        }
	        found=1;
	    }
    }
    fclose(fp1);
    fclose(fp2);
    return found;
}

