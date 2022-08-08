#ifndef __NV_FLASH_H
#define __NV_FLASH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"
#include "init_d.h"

extern int8_t nv_flash_getlock( void );
extern int8_t nv_flash_reslock( void );
extern uint8_t nv_flash_earse( uint32_t addr );
extern uint8_t nv_flash_read( uint32_t addr , uint8_t *pdt  , uint32_t cnt );
extern uint8_t nv_flash_write( uint32_t addr , uint8_t *pdt  , uint32_t cnt );


#ifdef __cplusplus
}
#endif

#endif /* __NV_FLASH_H */
