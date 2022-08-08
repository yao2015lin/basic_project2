#include "touch_keyboard.h"
#include "i2c_type.h"
#define MODULE_NAME "touch_keyboard"

#define DEVICE_ADDR (0x37u)
/* I2C and EZI2C slave address defines */
#define I2C_I2C_SLAVE_ADDR_POS (0x01u)  /* 7-bit address shift */
#define I2C_I2C_SLAVE_ADDR_MASK (0xFEu) /* 8-bit address mask */
/* Return 8-bit address. The input address should be 7-bits */
#define I2C_GET_I2C_8BIT_ADDRESS(addr) (((unsigned long)((unsigned long)(addr) << I2C_I2C_SLAVE_ADDR_POS)) & I2C_I2C_SLAVE_ADDR_MASK)

static DEV_HAND i2cfd;
static DEV_HAND irqfd;
xTaskHandle tkb_hdl;

static void tkb_open_i2c(void)
{
    i2cfd = c_open("i2c1", 0);
    INIT_PRINT((i2cfd == NULL) ? INIT_FAIL : INIT_OK, "open i2c1");
}

static void tkb_open_irq(void)
{
    irqfd = c_open("io_irq_line7", 0);
    INIT_PRINT((irqfd == NULL) ? INIT_FAIL : INIT_OK, "open io irq line7");
}

static int tkb_open(FIL_HAND *fd)
{
    tkb_open_irq();
    tkb_open_i2c();

    return 0;
}

static int tkb_read(FIL_HAND *fd, void *data, uint32_t count)
{
    i2c_data_t i2c_data;
    uint8_t rdata[4];
    int value = 0;

    if (c_read(irqfd, NULL, 0) == 0)
    {
        memset(&i2c_data, 0x00, sizeof(i2c_data_t));
        memset(rdata, 0x00, sizeof(rdata));

        i2c_data.sal_addr = (uint8_t)(DEVICE_ADDR << 1);
        i2c_data.mem_addr = 0xaa;
        i2c_data.mem_size = I2C_MEMADD_SIZE_8BIT;
        i2c_data.pdata = rdata;
        i2c_data.size = 4;
        i2c_data.timeout = 500;

        if (c_read(i2cfd, (void *)&i2c_data, sizeof(i2c_data_t)) < 0)
        {
            log(WARN, "touch key board read key value is fail.\n");
            return -EBUSY;
        }
        value = rdata[1];
        value <<= 8;
        value |= rdata[0];
 
        switch (value)
        {
        case 0x10:
            value = KEY_ENT;
            break;
        case 0x100:
            value = KEY_0;
            break;
        case 0x08:
            value = KEY_DEL;
            break;
        case 0x20:
            value = KEY_9;
            break;
        case 0x200:
            value = KEY_8;
            break;
        case 0x4000:
            value = KEY_7;
            break;
        case 0x40:
            value = KEY_6;
            break;
        case 0x400:
            value = KEY_5;
            break;
        case 0x2000:
            value = KEY_4;
            break;
        case 0x80:
            value = KEY_3;
            break;
        case 0x800:
            value = KEY_2;
            break;
        case 0x1000:
            value = KEY_1;
            break;
        default: value = -EBUSY; //log(DEBUG,"Read error value = %x.\r\n" , value);
            break;
        }
        
        if (data != NULL)
        {
            *(uint32_t *)data = value;
        }
    }

    return value;
}

static int tkb_resume(FIL_HAND *fd)
{
    c_ioctl(i2cfd, NULL, 0, IOCTL_FLAG_RESUME);
    return 0;
}

static int tkb_suspend(FIL_HAND *fd)
{
    c_ioctl(i2cfd, NULL, 0, IOCTL_FLAG_SUSPEND);
    return 0;
}

struct file_operations tkb_fops =
{
    .onwer = 0,
    .open = tkb_open,
    .read = tkb_read,
    .resume = tkb_resume,
    .suspend = tkb_suspend,
};

struct miscdevice tkb_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &tkb_fops,
    .nodename = "modules",
};

static int tkb_init(void)
{
    return (misc_register(&tkb_device));
}

modules_init(tkb_init);
