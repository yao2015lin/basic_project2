#include "nv_list.h"
#include "crc32.h"
#include "stdlib.h"

uint8_t nv_buf[4096];
/*
**
*/
void nvlist_clear(nv_list *list)
{
    uint32_t addr = 0;
    uint32_t page = list->max * list->index_size / list->page_size;

    if ((list->max * list->index_size) % list->page_size != 0)
    {
        page = list->max * list->index_size / list->page_size + 1;
    }
    else
    {
        page = list->max * list->index_size / list->page_size;
    }

    nv_flash_getlock();

    for (uint8_t i = 0; i < page; i++)
    {
        addr = list->index_addr + i * list->page_size;
        nv_flash_earse(addr);
    }

    nv_flash_reslock();
}
/*
**
*/
uint32_t nvlist_read_id( nv_list *list , uint32_t index)
{
    uint32_t tempID = 0;

    nv_flash_read(list->index_addr + index * list->index_size, (uint8_t *)&tempID, list->index_size);

    return tempID;
}

/*
**
*/
void nvlist_read_data(nv_list *list , uint32_t index)
{
    nv_flash_read(list->data_addr + index * list->data_size, list->data, list->data_size);
}

void nvlist_del_index( nv_list *list , uint32_t index)
{
    uint32_t addr = 0, page = 0;

    addr = list->index_addr + index * list->index_size;

    page = addr / list->page_size;

    nv_flash_getlock();

    nv_flash_read(page * list->page_size, nv_buf, list->page_size);

    *(uint32_t *)(nv_buf + (index * list->index_size) % list->page_size) = EMPTY_ID;

    nv_flash_earse(page * list->page_size);

    nv_flash_write(page * list->page_size, nv_buf, list->page_size);

    nv_flash_reslock();
}

void nvlist_del_index_no_lock( nv_list *list , uint32_t index)
{
    uint32_t addr = 0, page = 0;

    addr = list->index_addr + index * list->index_size;

    page = addr / list->page_size;

    nv_flash_read(page * list->page_size, nv_buf, list->page_size);

    *(uint32_t *)(nv_buf + (index * list->index_size) % list->page_size) = EMPTY_ID;

    nv_flash_earse(page * list->page_size);

    nv_flash_write(page * list->page_size, nv_buf, list->page_size);
}

void nvlist_add_index(nv_list *list , uint32_t index)
{
    uint32_t addr = 0, page = 0, ID = 0;

    addr = list->index_addr + index * list->index_size;

    page = addr / list->page_size;

    ID = crc32(list->index_data, list->index_cnt);

    nv_flash_getlock();

    nv_flash_read(page * list->page_size, nv_buf, list->page_size);

    *(uint32_t *)(nv_buf + (index * list->index_size) % list->page_size) = ID;

    nv_flash_earse(page * list->page_size);

    nv_flash_write(page * list->page_size, nv_buf, list->page_size);

    nv_flash_reslock();
}

int32_t nvlist_find_index(nv_list *list , uint32_t ID)
{
    uint32_t tempID;

    nv_flash_getlock();
    for (uint32_t i = 0; i < list->max; i++)
    {
        nv_flash_read(list->index_addr + i * list->index_size, (uint8_t *)&tempID, 4);
        if (tempID == ID)
        {
            nv_flash_reslock();
            return i;
        }
    }
    nv_flash_reslock();
    return FIND_NULL_ID;
}

void nvlist_add_data( nv_list *list , uint32_t index)
{
    uint32_t addr = 0, page = 0;

    addr = list->data_addr + index * list->data_size;

    page = addr / list->page_size;

    nv_flash_getlock();

    nv_flash_read(page * list->page_size, nv_buf, list->page_size);

    memcpy(nv_buf + (index * list->data_size) % list->page_size, list->data, list->data_size);

    nv_flash_earse(page * list->page_size);

    nv_flash_write(page * list->page_size, nv_buf, list->page_size);

    nv_flash_reslock();
}

int32_t find_index(nv_list *list)
{
    uint32_t tem = 0;
    uint32_t pos = 0;
    uint32_t ID = 0;

    ID = crc32(list->index_data, list->index_cnt);

    nv_flash_getlock();
    for (uint32_t addr = list->index_addr; addr < list->index_addr + list->max * list->index_size; addr += 4096)
    {
        nv_flash_read(addr, nv_buf, 4096);
        for (uint16_t i = 0; i < 4096 / list->index_size; i++)
        {
            tem = 0;
            memcpy(&tem, nv_buf + i * list->index_size, list->index_size);
            if (tem == ID)
            {
                pos = ((addr + i * list->index_size) - list->index_addr) / list->index_size;
                nvlist_read_data( list , pos );
                nv_flash_reslock();
                return (pos);   
            }
        }
    }
    nv_flash_reslock();
    return FIND_NULL_ID;
}

int32_t nvlist_find(nv_list *list)
{
    uint32_t pos = 0;
    uint32_t tick = HAL_GetTick();

    if ((pos = find_index(list)) != FIND_NULL_ID)
    {
        log(DEBUG, "在 %d 位置查询到数据 , 查询时间:%d\n", pos, HAL_GetTick() - tick);
        return pos;
    }

    log(DEBUG, "该卡号不在黑白名单中, 查询时间:%d\n", HAL_GetTick() - tick);

    return FIND_NULL_ID;
}

int32_t nvlist_add(nv_list *list)
{
    uint32_t index = 0;
    nv_list temp_list;

    memcpy(&temp_list , list , sizeof(nv_list));
    temp_list.data = malloc(temp_list.data_size);
    memset(temp_list.data , 0x00 , temp_list.data_size);
    
    if ((index = nvlist_find(&temp_list)) == FIND_NULL_ID)
    {
        log(DEBUG, "当前黑白名单列表没有找到对应卡号，新增名单\n");

        if ((index = nvlist_find_index(&temp_list , EMPTY_ID)) == FIND_NULL_ID)
        {
            log(WARN, "黑白名单数据已经满了。\n");
            return LIST_FULL;
        }
        log(DEBUG, "找到一个空闲位置，位置:%d\n", index);
        nvlist_add_index(&temp_list , index);
    }
    else
    {
        log(DEBUG, "当前列表中存在改卡号，位置：%d \n", index);
    }

    if (memcmp(temp_list.data,list->data, list->data_size) == TRUE)
    {
        log(DEBUG, "写入的数据和存储的数据一样，不用重新写入\n");
    }
    else
    {
        log(DEBUG, "数据将写入到 %d 位置中。\n", index);
        nvlist_add_data( list , index );
    }
    return LIST_SUCCESS;
}

int32_t nvlist_del(nv_list *list)
{
    uint32_t index = 0;

    if ((index = nvlist_find(list)) == FIND_NULL_ID)
    {
        log(DEBUG, "列表中没有改名单,删除失败\n");
    }
    else
    {
        log(DEBUG, " 在%d 位置删除\n", index);
        nvlist_del_index(list , index);
    }
    return LIST_SUCCESS;
}

void nvlist_show(nv_list *list)
{
    uint32_t cnt = 0;
    nv_flash_getlock();
    for (uint32_t i = 0; i < list->max; i++)
    {
        if (nvlist_read_id(list , i) != EMPTY_ID)
        {
            cnt++;
        }
    }
    nv_flash_reslock();
    log(DEBUG, "当前设备黑白名单条数:%d\n", cnt);
}


/*
void permiList_clear_overdue(void)
{
    permiListType list;
    uint32_t deviceTime;

    if (config.read(CFG_SYS_UPDATA_TIME, NULL) == TRUE)
    {
        deviceTime = rtc.read_stamp();

        log(DEBUG, "当前设备时间戳:%d\n", deviceTime);
        nv_flash_getlock();
        for (uint32_t i = 0; i < 8000; i++)
        {
            if (nvlist_read_id(i) != EMPTY_ID)
            {
                nvlist_read_data(i, &list);
                if (list.time < deviceTime)
                {
                    log(DEBUG, "删除过期黑白名单，POS=%d , ID=%llx , TIME:%u\n", i, list.ID, list.time);
                    nvlist_del_index_no_lock(i);
                }
                if ((list.status != LIST_BLACK) && (list.status != LIST_WRITE))
                {
                    log(INFO, "黑白名单状态错误,status=%d , 删除POS=%d\n", list.status, i);
                    nvlist_del_index_no_lock(i);
                }
            }
            HAL_IWDG_Refresh(&hiwdg);
        }
        nv_flash_reslock();
    }
    else
    {
        log(INFO, "设备还没有同步时间\n");
    }
}
*/