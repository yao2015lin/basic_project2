#include "password_list.h"

nv_list pwd_list;

int password_list_init( void )
{
    pwd_list.max = 100;
    pwd_list.index_size = 4;
    pwd_list.page_size = 4096;
    pwd_list.index_addr = 0x4000;
    pwd_list.data_addr = 0x8000;
    pwd_list.data_size = sizeof(pwd_str);
    return 0;
}

void add_password( pwd_str *pwd , nv_list *list )
{
    list->data = (uint8_t *)pwd;
    
    list->index_cnt =  sizeof(pwd->pwd);
    list->index_data = pwd->pwd;
    
    nvlist_add(list);
}

comp_init(password_list_init);

