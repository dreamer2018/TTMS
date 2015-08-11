#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include "../Common/Common.h"

typedef struct {
	int id;
	int play_id;
	int studio_id;
	user_date_t date;
	user_time_t time;
	int seat_count;
}schedule_t;

//˫������
typedef struct schedule_node {
	schedule_t data;
	struct schedule_node *next, *prev;
} schedule_node_t, *schedule_list_t;

inline int Schedule_Srv_Add(const schedule_t *data);

inline int Schedule_Srv_Modify(const schedule_t *data);

inline int Schedule_Srv_DeleteByID(int ID);

inline int Schedule_Srv_FetchByID(int ID, schedule_t *buf);

inline int Schedule_Srv_FetchAll(schedule_list_t list);

inline int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id);

//�����ݳ�ID��ͳ�������ʼ�Ʊ��������Ʊ��������
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount);


#endif //SCHEDULE_H_
