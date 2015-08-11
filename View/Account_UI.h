/*
 * Account_UI.h
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#ifndef ACCOUNT_UI_H_
#define ACCOUNT_UI_H_

#include "../Common/List.h"
#include "../Service/Account.h"

static const int ACCOUNT_PAGE_SIZE = 3;

int SysLogin();
void Account_UI_MgtEntry();
int Account_UI_Add();
int Account_UI_Modify(int ID);
int Account_UI_Delete(int ID);
int Account_UI_Query(char usrName[],account_list_t list);
char *usrtype(int i);
void Print_Acc_Info(account_list_t  list);
#endif /* ACCOUNT_UI_H_ */

