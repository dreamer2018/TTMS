/*
 * account.c
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */
 #include"../Common/conio.h"
#include "Account.h"
#include "EntityKey.h"
#include "../Common/List.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>

extern account_t gl_CurUser;

void Account_Srv_InitSys()
{
	account_t data;
	if(!Account_Perst_CheckAccFile())
	{
		data.id=EntKey_Srv_CompNewKey("account");
		strcpy(data.username,"admin");
		strcpy(data.password,"admin123");
		data.type=1;
		Account_Perst_Insert(&data);	
	}
}
inline int Account_Srv_Verify(char usrName[], char pwd[],int type)
{
	int rtn=0;
	account_t buf;
	if(Account_Perst_SelByName(usrName,&buf))
	{
		if(0==strcmp(pwd,buf.password) && buf.type==type)
		{
			rtn=1;
		}
	}
	return rtn;
}

account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[])
{
       return ;
}

inline int Account_Srv_Add(const account_t *data)
{
	return  Account_Perst_Insert(data);
}
inline int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}
inline int Account_Srv_DeleteByID(int usrID)
{
	 return Account_Perst_DeleteByID(usrID);
}
inline int Account_Srv_FetchByID(int usrID, account_t *buf)
{
	return Account_Perst_SelectByID(usrID,buf);
}

inline int Account_Srv_FetchByName(char usrName[], account_t *buf)
{
	return Account_Perst_SelByName(usrName,buf);
}

inline int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}


