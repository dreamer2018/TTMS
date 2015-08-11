#include "Sale_Persist.h"
#include "../Common/List.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";

int Sale_Perst_Insert(const sale_t *data) 
{
	int rtn=0;
	FILE *fp;
	fp=fopen(SALE_DATA_FILE,"ab");
	if(fp==NULL)
	{
		printf("文件打开失败！");
		return 0;
	}
	else
	{
		rtn=fwrite(data,sizeof(sale_t),1,fp);
	}
	fclose(fp);
	return rtn;
}


int Sale_Perst_DeleteByID(int saleID)
{
	int found=0;
	sale_t *buf;
	buf=(sale_t *)malloc(sizeof(sale_t));
	rename(SALE_DATA_FILE,SALE_DATA_TEMP_FILE);
    	FILE *fp1,*fp2;
    	fp1=fopen(SALE_DATA_TEMP_FILE,"rb");
    	fp2=fopen(SALE_DATA_FILE,"wb");
	if(fp1==NULL || fp2==NULL)
	{
		printf("不能打开此文件!");
		return 0;
	} 
	else
	{
	    	while(fread(buf,sizeof(sale_t),1,fp1)==1)
	    	{
			if(buf->id!=saleID)
			{
				fwrite(buf,sizeof(sale_t),1,fp2);
			}
			found=1;
		}
	}
	fclose(fp1);
	fclose(fp2);
	return found;
}

//根据用户ID载入给定时间区间内的销售记录
int Sale_Perst_SelectByUsrID(sale_list_t list, int usrID, user_date_t stDate,user_date_t endDate) 
{
	int recCount=0,i,j;
	FILE *fp;
	sale_t saleRec;
	sale_list_t newNode;
	fp=fopen(SALE_DATA_FILE,"rb");
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		return 0;
	}
	else
	{
		while(fread(&saleRec,sizeof(sale_t),1,fp)==1)
		{
			i=DateCmp( saleRec.date,stDate); 
			j=DateCmp(  endDate,saleRec.date); 
			if(saleRec.user_id==usrID && i !=-1 && j !=-1)
			{
				newNode=(sale_node_t *)malloc(sizeof(sale_node_t ));
				newNode->data.id=saleRec.id;
				newNode->data.user_id=saleRec.user_id;
				newNode->data.ticket_id=saleRec.ticket_id;
				newNode->data.date=saleRec.date;
				newNode->data.time=saleRec.time;
				newNode->data.value=saleRec.value;
				newNode->data.type=saleRec.type;
				List_AddTail(list,newNode);
				recCount++;
				break;
			}
		}
	}
	return recCount;
}

//根据给定时间区间内的销售记录
int Sale_Perst_SelectByDate(sale_list_t list, user_date_t stDate,user_date_t endDate) 
{
	FILE *fp;
	int i,j;
	int recCount;
	fp=fopen(SALE_DATA_FILE,"rb");
	sale_t saleRec;
	sale_list_t newNode;
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
	}
	else
	{
		while(fread(&saleRec,sizeof(sale_t),1,fp)==1)
		{
			i=DateCmp( saleRec.date,stDate);    //          1
			j=DateCmp(  endDate,saleRec.date);     //   1 
			if( i !=-1 && j !=-1)
			{
				newNode=(sale_node_t *)malloc(sizeof(sale_node_t ));
				newNode->data.id=saleRec.id;
				newNode->data.user_id=saleRec.user_id;
				newNode->data.ticket_id=saleRec.ticket_id;
				newNode->data.date=saleRec.date;
				newNode->data.time=saleRec.time;
				newNode->data.value=saleRec.value;
				newNode->data.type=saleRec.type;
				List_AddTail(list,newNode);
				recCount++;
				break;
			}
		}
	}
	return recCount;
}

/*

//queryUid = null,query all the uid Sale information return the all uid Sale information number
//queryUid =  uid, query the uid Sale information return the uid Sale  number
 *
int  Sale_Infor_By_Uid(char * queryUid, Sale_list_t head)
{
       // 请补充完整
       return 1;
}

int Sale_Infor_By_Time(char  queryTime[][30], Sale_list_t head)
{
// 请补充完整
       return 1;
}

//根据票的id获得票的基本信息 
int Select_Price_By_Playid(int id, ticket_t *buf)
{
// 请补充完整
       return 1;
}
*/

