/*
 * Example.c
 *
 *  Created on: 2015年6月25日
 *      Author: edu
 */
#include "Example.h"
#include "../Service/EntityKey.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//链表分页输出
static void Example_List_PgShow(student_list_t list, int listSize) {
	int pageSize, i;
	Pagination_t paging;
	student_node_t *p;
	char choice;

	printf("Input page size:");
	scanf("%d", &pageSize);

	paging.pageSize = pageSize;

	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);

	do {
		printf("============= Student List =================\n");
		printf("%5s%20s%8s\n", "ID", "Name", "Score");
		printf("--------------------------------------------\n");
		for (i = 0, p = (student_node_t *) (paging.curPos);
				p != list && i < paging.pageSize; i++) {
			printf("%5d%20s%8d\n", p->data.ID, p->data.name, p->data.score);
			p=p->next;
		}
		printf("--Total Records:%2d --------- Page %2d/%2d --\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("[P]revPage | [N]extPage | [R]eturn\n");
		printf("============================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);

		switch (choice) {
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, -1, student_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, 1, student_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');

}

static void Example_List_Show(student_list_t list) {

	student_node_t *p;
	printf("============= Student List =================\n");
	printf("%5s%20s%8s\n", "ID", "Name", "Score");
	printf("--------------------------------------------\n");
	List_ForEach(list, p)
	{
		printf("%5d%20s%8d\n", p->data.ID, p->data.name, p->data.score);
	}
	printf("============================================\n");

}

//链表使用示例
void Example_List() {
	student_list_t list;   //链表头指针变量
	student_node_t *p, *pNew;
	int i;

//初始化链表
	List_Init(list, student_node_t);

//头插法插入10个节点
	for (i = 0; i < 10; i++) {
		p = (student_node_t *) malloc(sizeof(student_node_t));
		p->data.ID = i;
		p->data.score = i * 10;
		sprintf(p->data.name, "%s%d", "Student-", i);
		List_AddHead(list, p);
	}

	//以普通模式输出链表
	Example_List_Show(list);


	if(List_IsEmpty(list))
		printf("List is empty!\n");
	else
		printf("List is not empty!\n");

	pNew = (student_node_t *) malloc(sizeof(student_node_t));
	pNew->data.ID = 100;
	pNew->data.score = 1000;
	sprintf(pNew->data.name, "%s%d", "Student-", 1000);

	//InsertBefore
	List_InsertBefore(list->next, pNew);

	//以普通模式输出链表
	Example_List_Show(list);


	//删除节点pNew
	List_DelNode(pNew);
	//以普通模式输出链表
	Example_List_Show(list);

	//InsertAfter
	List_InsertAfter(list->next, pNew);
	//以普通模式输出链表
	Example_List_Show(list);
	//释放节点pNew
	List_FreeNode(pNew);

//清空链表内容
	List_Free(list, student_node_t);

//以尾插法插入10个节点
	for (i = 0; i < 10; i++) {
		p = (student_node_t *) malloc(sizeof(student_node_t));
		p->data.ID = i;
		p->data.score = i * 10;
		sprintf(p->data.name, "%s%d", "Student-", i);
		List_AddTail(list, p);
	}

//以分页方式输出链表
	Example_List_PgShow(list, 10);

//销毁链表
	List_Destroy(list, student_node_t);

}

//主键服务示例
void Example_EntKeySrv() {

	long key;
	int count;

	//获取主键
	key=EntKey_Srv_CompNewKey("student");

	//输出主键
	printf("Entity Key:%ld\n", key);

	//批量获取主键
	printf("Input key count:");
	scanf("%d", &count);
	key=EntKey_Srv_CompNewKeys("student", count);

	//输出主键
	printf("Start Entity Key:%ld\n", key);

	//再次获取新主键
	key=EntKey_Srv_CompNewKey("student");
	//输出主键
	printf("Entity Key:%ld\n", key);

}

