#include "Play.h"
#include "../Common/List.h"
#include "../Persistence/Play_Persist.h"
#include <string.h>


inline int Play_Srv_Add(const play_t *data) 
{
       return  Play_Perst_Insert(data);
}

inline int Play_Srv_Modify(const play_t *data) 
{
       return Play_Perst_Update(data);
}

inline int Play_Srv_DeleteByID(int ID) 
{
       return Play_Perst_DeleteByID(ID);
}

inline int Play_Srv_FetchByID(int ID, play_t *buf)
 {

       return Play_Perst_SelectByID(ID,buf);

}

inline int Play_Srv_FetchAll(play_list_t list) {

       return Play_Perst_SelectAll(list);
}
inline int Play_Srv_FetchByName(play_list_t list,char fne[]) {
	return Play_Perst_SelectByName(list,fne);
}



