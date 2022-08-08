#ifndef __WB_FLASH_8M_H
#define __WB_FLASH_8M_H

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

#define FLASH_CMD_WRITE			0x02  // Write to Memory instruction 
#define FLASH_CMD_WRSR			0x01  // Write Status Register instruction 
#define FLASH_CMD_WREN			0x06  // Write enable instruction 
#define FLASH_CMD_READ			0x03  // Read from Memory instruction 
#define FLASH_CMD_RDSR			0x05  // Read Status Register instruction  
#define FLASH_CMD_RDID			0x9F  // Read identification 
#define FLASH_CMD_SE			0x20  // Sector Erase instruction 
#define FLASH_CMD_BL32K			0x52  // 32K Block Erase instruction 
#define FLASH_CMD_BL64K			0xD8  // 32K Block Erase instruction 
#define FLASH_CMD_BE			0xC7  // Bulk Erase instruction 

#define FLASH_WIP_FLAG			0x01  // Write In Progress (WIP) flag 
#define FLASH_DUMMY_BYTE		0xA5
#define FLASH_SPI_PAGESIZE		0x100
#define FLASH_SPI_BLOCKSIZE     0x1000


  
#ifdef __cplusplus
}
#endif

#endif /* __WB_FLASH_8M_H */
