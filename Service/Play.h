#ifndef PLAY_H_
#define PLAY_H_

#include "../Common/Common.h"

typedef enum {
	PLAY_TYPE_FILE=1,						//��ͯ��
	PLAY_TYPE_OPEAR=2,					//�ٶ���
	PLAY_TYPE_CONCERT=3				//���˾�
} play_type_t;

typedef enum {
	PLAY_RATE_CHILD = 1,						// 2D
	PLAY_RATE_TEENAGE = 2,					//3D
	PLAY_RATE_ADULT = 3							//IMAX
} play_rating_t;

typedef struct {
	int id;
	char name[31];
	int  type;
	char area[9];
	int rating;
	int duration; //�Է���Ϊ��λ
	user_date_t start_date;
	user_date_t end_date;
	int price;
} play_t;

//˫������
typedef struct play_node {
	play_t data;
	struct play_node *next, *prev;
} play_node_t, *play_list_t;

inline int Play_Srv_Add(const play_t *data);

inline int Play_Srv_Modify(const play_t *data);

inline int Play_Srv_DeleteByID(int ID);

inline int Play_Srv_FetchByID(int ID, play_t *buf);

inline int Play_Srv_FetchAll(play_list_t list);

//���ݾ�Ŀ���������Ŀ����������ľ�Ŀ����
inline int Play_Srv_FetchByName(play_list_t list, char condt[]);

//���ݾ�Ŀ���ƹ��˴�filter��list���й��ˣ����ع��˺�ʣ��ľ�Ŀ����
int Play_Srv_FilterByName(play_list_t list, char filter[]);

#endif //PLAY_H_
