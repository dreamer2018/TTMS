

//在文件中存入演出厅函数
int Studio_Perst_Insert(studio_t  *p);

//在文件中更新演出厅函数 
int Studio_Perst_Update(studio_t *data);

//在文件中获取所有演出厅函数 
int Studio_Perst_SelectAll(studio_node_t *head);

//根据ID在文件中删除演出厅函数 
int Studio_Perst_DeleteByID(int ID);

//根据ID在文件中获取演出厅函数 
int Studio_Perst_SelectByID(int ID,studio_t *p) ;		//list为链表的头节点

