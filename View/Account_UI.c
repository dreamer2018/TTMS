/*
 * Account_UI.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */
#include "Account_UI.h"
#include"../Service/Struct.h"
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>
#define PAGESIZE 6

int SysLogin() 
{
		int j=3,i,type,rtn=-1;
		char ch;
		char user_name[21],user_passwd[21];
       printf("\n\t\t========================================================\n");
       printf("\t\t======================== 用户登录 ======================\n");
       printf("\t\t========================================================\n");
       printf("\t\t用户类型(1.系统管理员,2.经理,3.售票员):");
		scanf("%d",&type);
		getchar();
       printf("\t\t请输入用户名：");
       scanf("\t\t%s",user_name);
       getchar();
       while(1)
       {
       		j--;
       		printf("\t\t请输入密码：");
		   for(i=0;i<20;i++)
			{
				    ch=getch();
					if(ch=='\n')
					{
						user_passwd[i]='\0';
						break;
					}
					user_passwd[i]=ch;
					putchar('*');
			}
			user_passwd[i]='\0';
			if(Account_Srv_Verify(user_name,user_passwd,type))
			{
				rtn=type;
				break;
			}
			else
			{
				if(j==0)
				{
					printf("\n密码三次输入错误，登录失败！\n");
					printf("Press Any Key To Continue........");
					getchar();
					break;
				}
				else
				{
					printf("\n密码输入错误，你还有%d次机会，请重新输入！\n",j);
					printf("Press Any Key To Continue........");
					getchar();
				}
			}
		}
		return rtn;
}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() 
{
		int listSize,usr_id,i;
		char choice,userName[21];
		Pagination_t paging;
		account_list_t p;
		List_Init(p,account_node_t);
		account_list_t list,pos;
		account_t buf;
		List_Init(list,account_node_t);
		listSize=Account_Srv_FetchAll(list);
		paging.pageSize =PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do {
					system("clear");
					printf("\n\t\t\033[32m****************** 计划表 *******************\n");
					printf("\t\t     %2s%20s%20s\n", "ID", "用户名", "用户类型");
					printf("\t\t*********************************************\n");
					Paging_ViewPage_ForEach(list,paging,account_node_t,pos,i)
					{
						printf("\033[34m\t\t    %2d%17s%20s\n",pos->data.id,pos->data.username,usrtype(pos->data.type));
					}
					printf("\t\t\033[32m---------------------------------------------\n");
					printf("\t\t----总记录数:%2d ----------- 页数： %2d/%2d ----\n",paging.totalRecords, Pageing_CurPage(paging),	Pageing_TotalPages(paging));
					printf("\t\t---------------------------------------------\n");
					printf("\t\t** 【P】上一页   【N】下一页  【R】返回    **\n");
					printf("\t\t** 【A】添加用户 【M】修改用户信息         **\n");
					printf("\t\t** 【Q】查询(By name) 【D】删除用户        **\n");
					printf("\t\t=============================================\033[0m\n");
					printf("\t\tYour Choice:");
					scanf("\t\t%c", &choice);
					getchar();
					switch (choice)
					{
							case 'Q': ;
							case 'q':
												printf("请输入你要查询的用户名：");
												scanf("%s",userName);
												getchar();
												if(Account_UI_Query(userName,p))
												{
													Print_Acc_Info(p);
												}
												else
												{
													printf("Not Found！\n");
													printf("Press Any  Key To Continue......");
													getchar();
												}
												paging.totalRecords=Account_Srv_FetchAll(list);
												List_Paging(list, paging, account_node_t);
											  	break;
							case 'A': ;
							case 'a':
												if(Account_UI_Add())
												{
													printf("添加成功！\n");
													printf("Press Any  Key To Continue......");
													getchar();
												}
												else
												{
													printf("添加失败！\n");
													printf("Press Any  Key To Continue......");
													getchar();
												}
												paging.totalRecords=Account_Srv_FetchAll(list);
												List_Paging(list, paging, account_node_t);
											  	break;
							case 'M': ;
							case 'm':
												printf("请输入id :");
												scanf("%d",&usr_id);
												getchar();
												if(!Account_Srv_FetchByID(usr_id,&buf ))
												{
													printf("你输入的id不存在！\n");
													printf("Press Any  Key To Continue......");
													getchar();
												}
												else
												{
													if(Account_UI_Modify(usr_id))
													{
														printf("修改成功！\n");
														printf("Press Any  Key To Continue......");
														getchar();
													}
													else
													{
														printf("修改失败！\n");
														printf("Press Any  Key To Continue......");
														getchar();
													}
												}
												paging.totalRecords=Account_Srv_FetchAll(list);
												List_Paging(list, paging, account_node_t);
											  	break;
							case 'D': ;
							case 'd':
												printf("请输入id :");
												scanf("%d",&usr_id);
												getchar();
												if(!Account_Srv_FetchByID(usr_id,&buf ))
												{
													printf("你输入的id不存在！\n");
													printf("Press Any  Key To Continue......");
													getchar();
												}
												else
												{
													if(Account_UI_Delete(usr_id))
													{
														printf("删除成功！\n");
														printf("Press Any  Key To Continue......");
														getchar();													
													}
													else
													{
														printf("删除失败！\n");
														printf("Press Any  Key To Continue......");
														getchar();
													}
												}
												paging.totalRecords=Account_Srv_FetchAll(list);
												List_Paging(list, paging, account_node_t);
												break;
							case 'r':
							case 'R':break;
							case 'p':
							case 'P':
												if (1 < Pageing_CurPage(paging))
												{
													Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
												}
												choice='r';
												break;
							case 'n':
							case 'N':
												if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
												{
													Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
												}
												choice='r';
												break;
					}
		} while (choice != 'r' && choice != 'R');
}


//添加一个用户账号信息
int Account_UI_Add()
{
	system("clear");
	int sign=1,i;
	account_t buf;
	char ch,passwd1[21],passwd2[21];
	printf("==============添加新用户=============\n");
	buf.id=EntKey_Srv_CompNewKey("account");
	printf("请输入用户名：");
	scanf("%s",buf.username);
	getchar();
	do
	{
		printf("请输入密码:");
		for(i=0;i<20;i++)
		{
		        ch=getch();
				if(ch=='\n')
				{
					passwd1[i]='\0';
					break;
				}
				passwd1[i]=ch;
				putchar('*');
		}
		passwd1[20]='\0';
		printf("\n请再次输入密码:");
		for(i=0;i<20;i++)
		{
		        ch=getch();
				if(ch=='\n')
				{
					passwd2[i]='\0';
					break;
				}
				passwd2[i]=ch;
				putchar('*');
		}
		passwd2[20]='\0';
		if(strcmp(passwd1,passwd2)==0)
		{
			strcpy(buf.password,passwd2);
			sign=0;
		}
		else
		{
			printf("\n两次输入的密码不同，请重新输入！\n");
		}
	}while(sign);
	printf("\n请输入账户类型：");
	scanf("%d",&buf.type);
	getchar();
	return Account_Srv_Add(&buf);
}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(int id)
{
	int sign=1,i,rtn=0;
	char ch,passwd1[21],passwd2[21];
	account_t buf,*data;
	Account_Srv_FetchByID(id,data);
    printf("==============修改用户信息=============\n");
    printf("=========原始用户信息=========\n");
    printf("id: %d\n",data->id);
    printf("用户名：%s\n",data->username);
    printf("用户类型: %s\n",usrtype(data->type));
    printf("=============================\n");
    printf("请输入新用户名：");
	scanf("%s",data->username);
	getchar();
	do
	{
		printf("请输入密码:");
		for(i=0;i<20;i++)
		{
		        ch=getch();
				if(ch=='\n')
				{
					passwd1[i]='\0';
					break;
				}
				passwd1[i]=ch;
				putchar('*');
		}
		passwd1[20]='\0';
		printf("\n请再次输入密码:");
		for(i=0;i<20;i++)
		{
		        ch=getch();
				if(ch=='\n')
				{
					passwd2[i]='\0';
					break;
				}
				passwd2[i]=ch;
				putchar('*');
		}
		passwd1[20]='\0';
		if(strcmp(passwd1,passwd2)==0)
		{
			strcpy(buf.password,passwd2);
			sign=0;
		}
		else
		{
			printf("\n两次输入的密码不同，请重新输入！\n");
		}
	}while(sign);
	printf("\n请输新的入账户类型：");
	scanf("%d",&buf.type);
	if(Account_Srv_Modify(&buf))
	{
		printf("修改成功！");
		printf("Press Any Key To Continue........");
		getchar();
		rtn=1;
	}
	return rtn;
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(int ID)
{
	 return Account_Srv_DeleteByID(ID);
}
//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息

int Account_UI_Query(char usrName[],account_list_t list)
{
	int rtn=0;
	account_list_t p;
	account_t buf;
	List_Init(list,account_node_t);
	while(Account_Srv_FetchByName(usrName,&buf))
	{
		p=(account_node_t *)malloc(sizeof(account_node_t));
		p->data=buf;
		List_AddTail(list,p);
		rtn=1;
	}
	return rtn;
}
void Print_Acc_Info(account_list_t  list)
{
	account_list_t p;
	p=list->next;
	while(p!=list)
	{
		printf("===========用户信息===========\n");
		printf("id: %d\n",p->data.id);
		printf("用户名：%s\n",p->data.username);
		printf("用户类型: %s\n",usrtype(p->data.type));
		printf("=============================\n");
		p=p->next;
    }
}
char *usrtype(int i)
{
	//administrator 系统管理员 1，manager 经理 2，clerk 售票员 3
	if(i==1)
	{
		return "系统管理员 ";
	}
	else if(i==2)
	{
		return "经理";
	}
	else if(i==3)
	{
		return "售票员";
	}
}
