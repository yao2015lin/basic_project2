#include "nv_flash.h"
#include "flash_type.h"


static DEV_HAND fd;

static sc_semaphore_t nv_flash_semap = NULL;


int8_t nv_flash_getlock( void )
{
    if( read_semaphore(nv_flash_semap, WAIT_FOR_EVER) < 0 )
    {
        return -EBUSY;
    }
    __set_PRIMASK(1); 

    return 0;
}

int8_t nv_flash_reslock( void )
{
    __set_PRIMASK(0);


    send_semaphore(nv_flash_semap);
    
    return 0;
}


uint8_t nv_flash_earse( uint32_t addr )
{
    c_ioctl( fd  , (void *)&addr , 4 , IOCTL_FLAG_EARSE);
    
    return 0;
}

uint8_t nv_flash_write( uint32_t addr , uint8_t *pdt  , uint32_t cnt )
{
    flash_data_t wdtp;

    wdtp.addr  = addr ; 
    wdtp.point = pdt;
    wdtp.cnt = cnt;

    c_write(fd , (void *)&wdtp , sizeof(flash_data_t) );

    return 0 ; 

}

uint8_t nv_flash_read( uint32_t addr , uint8_t *pdt  , uint32_t cnt )
{
    flash_data_t wdtp;

    wdtp.addr  = addr ; 
    wdtp.point = pdt;
    wdtp.cnt = cnt;

    c_read(fd , (void *)&wdtp , sizeof(flash_data_t) );

    return 0 ; 
}

uint8_t nv_flash_check( void )
{
	uint8_t buff[256];

	for( uint16_t i =  0 ; i < 255 ; i++)
	{
		buff[i] =  i;
	}
    
    nv_flash_getlock();

    nv_flash_earse(0);
    nv_flash_write(0 , buff , 255);
    
	memset(buff , 0x00 , sizeof(buff));

    nv_flash_read(0 , buff , 255);

    nv_flash_reslock();

	for( uint16_t i =  0 ; i < 255 ; i++)
	{
		if( buff[i] !=  i)
		{
			return FALSE;
		}
	}

	return TRUE;
}


int nv_flash_init(void)
{   
    fd = c_open("wbf_8m", 0);

    INIT_PRINT( (fd==NULL)?INIT_FAIL:INIT_OK,"Loading spi flash component");

    nv_flash_semap = creat_semqphore();
    INIT_PRINT((nv_flash_semap == NULL) ? INIT_FAIL : INIT_OK, "nv flash semqphore creat");

    nv_flash_reslock();
    
    if( nv_flash_check() == FALSE)
    {
        printf("**** nv flash check error.\n");
    }
    else
    {
       printf("**** nv flash check done.\n");
    }
    
    
    return 0;
}

comp_init(nv_flash_init);