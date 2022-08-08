#ifndef __NV_LIST_H
#define __NV_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "nv_flash.h"

    /**
  * @}
  */
#define EMPTY_ID 0xFFFFFFFF
#define LIST_SUCCESS (1)
#define FIND_NULL_ID (-1)
#define LIST_FULL (-2)

    /**
  * @}
  */
    typedef struct nv_list
    {
        /* data */
        uint32_t max;
        uint32_t index_size;
        uint32_t page_size;
        uint32_t index_addr;
        uint32_t data_addr;
        uint8_t *index_data;
        uint8_t index_cnt;
        uint8_t *data;
        uint8_t data_size;
    } nv_list;
    /**
  * @}
  */

    /**
  * @}
  */

    /**
  * @}
  */
extern int32_t nvlist_find(nv_list *list);
extern int32_t nvlist_add(nv_list *list);
extern int32_t nvlist_del(nv_list *list);
extern void nvlist_clear(nv_list *list);
extern void nvlist_show(nv_list *list);


#ifdef __cplusplus
}
#endif

#endif /* __NV_LIST_H */
