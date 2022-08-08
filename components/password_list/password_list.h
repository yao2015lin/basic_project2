#ifndef __PASSWORD_LIST_H
#define __PASSWORD_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "nv_list.h"

typedef struct pwd_str
{
    uint32_t time;
    uint8_t pwd[6];
    uint8_t temp[6];
}pwd_str;

extern void add_password( pwd_str *pwd , nv_list *list );

#ifdef __cplusplus
}
#endif

#endif /* __PWD_TASK_H */
