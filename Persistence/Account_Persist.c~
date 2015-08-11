/*
 * Account_Persist.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#include "Account_Persist.h"
#include "../Service/Account.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//判断账号文件是否存在
int Account_Perst_CheckAccFile()
{
	FILE *fp;
	fp=fopen(ACCOUNT_DATA_FILE,"rb");
	if(NULL==fp)
	{
		return 0;
	}
	fclose(fp);
	return 1;
}

//根据用户名载入账号
int Account_Perst_SelByName(char usrName[], account_t *buf)
{
	FILE *fp;
	// account_t data;
	fp=fopen(ACCOUNT_DATA_FILE,"rb");
	if(NULL==fp)
	{
		printf("文件打开失败！\n");
		printf("Press Any  Key To Continue......");
		getchar();
		return 0;
	}
	while(1==fread(buf,sizeof(account_t),1,fp))
	{
		if(!strcmp(usrName,buf->username))
		{
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

int Account_Perst_Insert(const account_t *data)
{
	int rtn=0;
	FILE *fp;
	fp=fopen(ACCOUNT_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		printf("Press Any  Key To Continue......");
		getchar();
		rtn=0;
	}
	else
	{
		fwrite(data,sizeof(account_t),1,fp);
		rtn=1;
	}
	fclose(fp);
	return rtn;
}

int Account_Perst_Update(const account_t *data) 
{
	int found=0; 
    FILE *fp;
    account_t buf;
    fp=fopen(ACCOUNT_DATA_FILE,"rb+");
    if(fp==NULL)
    {
			printf("文件打开失败！\n");
			printf("Press Any  Key To Continue......");
			getchar();
    	  	return 0;
    }
    else
    {
    	while(fread(&buf,sizeof(account_t),1,fp)==1)
        {
        	if(buf.id==data->id)
        	{
        		fseek(fp,-sizeof(account_t),1);
        		fwrite(data,sizeof(account_t),1,fp);
        		found=1;
        		break;
        	}
        }
    }
    fclose(fp);
    return found;
}

int Account_Perst_DeleteByID(int id)
{
	int found=0;
	account_t *buf;
	buf=(account_t *)malloc(sizeof(account_t));
	rename(ACCOUNT_DATA_FILE,ACCOUNT_DATA_TEMP_FILE);
    FILE *fp1,*fp2;
    fp1=fopen(ACCOUNT_DATA_TEMP_FILE,"rb");
    fp2=fopen(ACCOUNT_DATA_FILE,"wb");
    if(fp1==NULL)
    {
        printf("不能打开此文件!\n");
        printf("Press Any  Key To Continue......");
		getchar();
        return 0;
    } 
    else
    {
    	while(fread(buf,sizeof(account_t),1,fp1))
    	{
	        if(buf->id!=id)
	        {
	        	fwrite(buf,sizeof(account_t),1,fp2);
	        }
	        found=1;
	    }
    }
    fclose(fp1);
    fclose(fp2);
    return found;
}

int Account_Perst_SelectByID(int id, account_t *buf)
{
	int found=0;
        FILE *fp;
        account_t data;
        fp=fopen(ACCOUNT_DATA_FILE,"rb");
        if(fp==NULL)
        {
                printf("文件打开失败！");
                printf("Press Any  Key To Continue......");
				getchar();
                return 0;
        }
        else
        {
        	while((fread(&data, sizeof(account_t), 1, fp)))
        	{
        		if(id==data.id)
        		{
        			//buf=&data;
        			
        			buf->id=data.id;
        			buf->type=data.type;
        			strcpy(buf->username,data.username);
        			strcpy(buf->password,data.password);
        			
        			found=1;
        			break;
        		}
        	}
        }
        fclose(fp);
        return found;
}


int Account_Perst_SelectAll(account_list_t list)
{
	account_node_t *newNode;
	account_t data;
	int recCount = 0;
	List_Free(list, account_node_t);
	FILE *fp;
	fp=fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL== fp)
	{
		printf("文件打开失败！\n");
		printf("Press Any  Key To Continue......");
		getchar();
	}
	else
	{
		while((fread(&data, sizeof(account_t), 1, fp)))
		{
			newNode=(account_node_t*) malloc(sizeof(account_node_t));
			newNode->data=data;
			List_AddTail(list,newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

