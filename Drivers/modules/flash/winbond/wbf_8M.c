#include "wbf_8m.h"
#include "spi1.h"
#include "flash_type.h"

#define MODULE_NAME "wbf_8m"

static DEV_HAND spifd;

static void wbf_open_spi(void)
{
    spifd = c_open("spi1", 0);
    INIT_PRINT((spifd == NULL) ? INIT_FAIL : INIT_OK, "open spi1");
}

static void wbf_set_cs(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

static void wbf_clr_cs(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static void wbf_cs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



static uint8_t wbf_write_read_byte( uint8_t wbyte )
{
    spi_wr_t wrt;
    uint8_t w_d[2] = {0xFF,0xFF}, r_d[2] = {0,0} ;

    w_d[0] = wbyte;
    wrt.w_d = w_d;
    wrt.r_d = r_d;
    wrt.cnt = 1;

    c_ioctl(spifd , (void *)&wrt , sizeof(spi_wr_t), IOCTL_SET_WR_FLAG);
    
    return r_d[0];
}


static int wbf_read_id(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    wbf_set_cs();
    
    wbf_write_read_byte(0x9F);
    Temp0 = wbf_write_read_byte(FLASH_DUMMY_BYTE);
	Temp1 = wbf_write_read_byte(FLASH_DUMMY_BYTE);
	Temp2 = wbf_write_read_byte(FLASH_DUMMY_BYTE);
    
	wbf_clr_cs();
    
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    
	return Temp;
}

static void wbf_write_enable(void)
{
	wbf_set_cs();
	wbf_write_read_byte( FLASH_CMD_WREN);
	wbf_clr_cs();
}

static void wbf_wait_for_write_end(void)
{
	uint8_t flashstatus = 0;
	wbf_set_cs();
	wbf_write_read_byte( FLASH_CMD_RDSR);
	do
    {
        flashstatus = wbf_write_read_byte( FLASH_DUMMY_BYTE);
	}
	while ((flashstatus & FLASH_WIP_FLAG) == SET); 
    
	wbf_clr_cs();
}

void wbf_earse_chip( void )
{
	
	uint8_t cmd[4] ={0x00};
     
    wbf_write_enable();
        
    cmd[0] = FLASH_CMD_BE;

    wbf_set_cs();
    
    c_write(spifd, cmd, 1); 
    
	wbf_clr_cs();
    
	wbf_wait_for_write_end();
}


void wbf_erase_sector(uint32_t sectorAddr)
{
    uint8_t cmd[4] ={0x00};
    
    wbf_write_enable();
        
    cmd[0] = FLASH_CMD_SE;
    cmd[1] = (sectorAddr & 0xFF0000) >> 16;
    cmd[2] = (sectorAddr& 0xFF00) >> 8;
    cmd[3] = sectorAddr & 0xFF;
    
    wbf_set_cs();
    
    c_write(spifd, cmd, 4); 
    
	wbf_clr_cs();
    
	wbf_wait_for_write_end();
    

}


static void wbf_write_page(uint8_t* buffer, uint32_t w_addr, uint16_t w_cnt)
{
    uint8_t cmd[4] ={0x00};

    
    wbf_write_enable();
        
    cmd[0] = FLASH_CMD_WRITE;
    cmd[1] = (w_addr & 0xFF0000) >> 16;
    cmd[2] = (w_addr& 0xFF00) >> 8;
    cmd[3] = w_addr & 0xFF;
    
    wbf_set_cs();
 
    c_write(spifd, cmd, 4); 
    
    c_write(spifd, buffer,w_cnt);
    
	wbf_clr_cs();
    
	wbf_wait_for_write_end();
    

}

void wbf_write_buffer(uint32_t w_addr, uint8_t* buffer, uint16_t w_cnt)
{

	uint8_t num_page = 0, num_single = 0, addr = 0, count = 0, temp = 0;
  
	addr = w_addr % FLASH_SPI_PAGESIZE;
	count = FLASH_SPI_PAGESIZE - addr;
	num_page =  w_cnt / FLASH_SPI_PAGESIZE;
	num_single = w_cnt % FLASH_SPI_PAGESIZE;

	if (addr == 0)
    {
		if (num_page == 0)
        {
			wbf_write_page(buffer, w_addr, w_cnt);
		}
		else
        {
			while (num_page--)
            {
				wbf_write_page(buffer, w_addr, FLASH_SPI_PAGESIZE);
				w_addr +=  FLASH_SPI_PAGESIZE;
				buffer += FLASH_SPI_PAGESIZE;
			}
			wbf_write_page(buffer, w_addr, num_single);
		}
	}
	else
    {
		if (num_page == 0)
        {
			if (num_single > count)
            {
				temp = num_single - count;
				wbf_write_page(buffer, w_addr, count);
				w_addr +=  count;
				buffer += count;
				wbf_write_page(buffer, w_addr, temp);
			}
			else
            {
				wbf_write_page(buffer, w_addr, w_cnt);
			}
		}
		else
        {
			w_cnt -= count;
			num_page =  w_cnt / FLASH_SPI_PAGESIZE;
			num_single = w_cnt % FLASH_SPI_PAGESIZE;
			wbf_write_page(buffer, w_addr, count);
			w_addr +=  count;
			buffer += count;
			while (num_page--)
            {
				wbf_write_page(buffer, w_addr, FLASH_SPI_PAGESIZE);
				w_addr +=  FLASH_SPI_PAGESIZE;
				buffer += FLASH_SPI_PAGESIZE;
			}
			if (num_single != 0)
            {
				wbf_write_page(buffer, w_addr, num_single);
			}
		}
	}

}

void wbf_read_buffer(uint32_t read_addr,uint8_t* buffer,  uint16_t r_cnt)
{
    uint8_t cmd[4] ={0x00};
    

    cmd[0] = FLASH_CMD_READ;
    cmd[1] = (read_addr & 0xFF0000) >> 16;
    cmd[2] = (read_addr& 0xFF00) >> 8;
    cmd[3] = read_addr & 0xFF;


    wbf_set_cs();
    
    c_write(spifd, cmd, 4); 

    if( c_read(spifd , buffer,r_cnt) != HAL_OK)
    {
        cmd[0] = 0x01 ;
    }

	wbf_clr_cs();

}


static int wbf_open(FIL_HAND *fd)
{
    int chipid=0;
    
    wbf_open_spi();
    wbf_cs_init();
    
    chipid = wbf_read_id();
    
    printf("\tWinbond flash chip id=0x%x\n" , chipid);

    return 0;
}

int wbf_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    flash_data_t *fdt = ( flash_data_t *) buf;
    
    wbf_write_buffer(fdt->addr , fdt->point , fdt->cnt);
    return 0;
}


static int wbf_read(FIL_HAND *fd, void *data, uint32_t count)
{
    flash_data_t *fdt = ( flash_data_t *) data;
    
    wbf_read_buffer(fdt->addr , fdt->point , fdt->cnt);
    return 0;
}

static int wbf_resume(FIL_HAND *fd)
{
    c_ioctl(spifd, NULL, 0, IOCTL_FLAG_RESUME);
    return 0;
}

static int wbf_suspend(FIL_HAND *fd)
{
    c_ioctl(spifd, NULL, 0, IOCTL_FLAG_SUSPEND);
    return 0;
}

int wbf_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    if (flag & IOCTL_FLAG_EARSE)
    {
        uint32_t addr = *((uint32_t *)buf);
        if( addr % FLASH_SPI_PAGESIZE != 0)
        {
            log(INFO , "flash erase addr is error , %x\n" , addr);
            return -EFAULT;
        }
        wbf_erase_sector(addr);
    }
    return 0;
}


struct file_operations wbf_fops =
{
    .onwer = 0,
    .open = wbf_open,
    .write = wbf_write,
    .read = wbf_read,
    .ioctl = wbf_ioctl,
    .resume = wbf_resume,
    .suspend = wbf_suspend,
};

struct miscdevice wbf_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &wbf_fops,
    .nodename = "modules",
};

static int wbf_init(void)
{
    return (misc_register(&wbf_device));
}

modules_init(wbf_init);



