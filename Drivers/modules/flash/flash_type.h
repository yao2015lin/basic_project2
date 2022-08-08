#ifndef __FLASH_TYPE_H
#define __FLASH_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "BAT32G157.h"
#include "major.h"
#include "unit.h"
#include "init_d.h"
#include "miscdevice.h"
#include "priv_os.h"
#include "startup.h"

typedef struct 
{
    uint32_t addr;
    uint8_t *point;
    uint32_t cnt;
}flash_data_t;

  
#define IOCTL_FLAG_EARSE    IOCTL_FLAG_USER_FLAG1

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_TYPE_H */
