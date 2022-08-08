#include "ws2812b.h"

#define PIXEL_MAX 20

static DEV_HAND spifd;

uint8_t rBuffer[PIXEL_MAX] = {0};
uint8_t gBuffer[PIXEL_MAX] = {0};
uint8_t bBuffer[PIXEL_MAX] = {0};


static void send_bytes(uint8_t dat)
{
    uint8_t i = 0;
    static uint8_t code_one = 0x7c;  //7c，3e
    static uint8_t code_zero = 0x70; //70,38

    for (i = 0; i < 8; i++)
    {
        if ((dat & 0x80) == 0x80)
        {
            //HAL_SPI_Transmit_DMA(&hspi, &CodeOne, 1);
            c_write(spifd , &code_one , 1);
        }
        else
        {
            //HAL_SPI_Transmit_DMA(&hspi, &CodeZero, 1);
            c_write(spifd , &code_zero , 1);
        }
        dat = dat << 1;
    }
}

static void send_rgb_bytes(uint8_t r, uint8_t g, uint8_t b)
{
    send_bytes(g);
    send_bytes(r);
    send_bytes(b);
}

static void pixel_update(void) //should >24us
{
    uint8_t rst[24] = {0};
  
    c_write(spifd , rst , 24);
}

void set_all_pixel_color(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t i = 0;

    for (i = 0; i < PIXEL_MAX; i++)
    {
        send_rgb_bytes(r, g, b);
    }

    pixel_update();
}

void set_pixel_color(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t i = 0;
    
    for (i = 0; i < PIXEL_MAX; i++)
    {
        if( i == n)
        {
            send_rgb_bytes(r, g, b);
        }
        send_rgb_bytes(0 ,0 ,0);
    }
    
    pixel_update();
}


void set_pixel_color_32bits(uint16_t n, uint32_t c)
{
    uint8_t i = 0;

    rBuffer[n] = (uint8_t)(c >> 16);
    gBuffer[n] = (uint8_t)(c >> 8);
    bBuffer[n] = (uint8_t)c;

    for (i = 0; i < PIXEL_MAX; i++)
    {
        send_rgb_bytes(rBuffer[i], gBuffer[i], bBuffer[i]);
    }
    pixel_update();
}

uint32_t get_32bits_color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t wheel(uint8_t pos)
{
    pos = 255 - pos;
    if (pos < 85)
    {
        return get_32bits_color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170)
    {
        pos -= 85;
        return get_32bits_color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return get_32bits_color(pos * 3, 255 - pos * 3, 0);
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < PIXEL_MAX; i++)
        {
            set_pixel_color_32bits(i, wheel((i + j) & 255));
        }
        pixel_update();
        HAL_Delay(wait);
    }
}


void theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
        for (int q = 0; q < PIXEL_MAX; q++)
        {
            for (uint16_t i = 0; i < PIXEL_MAX; i = i + 1) //turn every one pixel on
            {
                set_pixel_color_32bits(i + q, wheel((i + j) % 255));
            }
            pixel_update();

            HAL_Delay(wait);

            for (uint16_t i = 0; i < PIXEL_MAX; i = i + 1) //turn every one pixel off
            {
                set_pixel_color_32bits(i + q, 0);
            }
            pixel_update();
        }
    }
}

static int ws2812_open(FIL_HAND *fd)
{
    uint8_t res_code[50] = {0};
    
    spifd = c_open("spi3", 0);
    INIT_PRINT((spifd == NULL) ? INIT_FAIL : INIT_OK, "open spi3");

    memset( rBuffer , 0 , PIXEL_MAX);
    memset( gBuffer , 0 , PIXEL_MAX);
    memset( bBuffer , 0 , PIXEL_MAX);
    
    c_write(spifd, res_code, 50);
    
    return 0;
}

static struct rgb_type
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

int ws2812_write(FIL_HAND *fd, const void *buf, uint32_t count)
{
    struct rgb_type *t = (struct rgb_type *)buf;
    uint8_t i = 0;

    if( count > PIXEL_MAX)
    {
        log(ERR , "")
    }
    for (i = 0; i < PIXEL_MAX; i++)
    {
        send_rgb_bytes(t->r[i], t->g[i], t->b[i]);
    }
    pixel_update();
    
    return 0;
}


int ws2812_ioctl(FIL_HAND *fd, void *buf, uint32_t count, uint32_t flag)
{
    
    return 0;
}


static int ws2812_resume(FIL_HAND *fd)
{
    c_ioctl(fd, NULL, 0, IOCTL_FLAG_RESUME);
    return 0;
}

static int ws2812_suspend(FIL_HAND *fd)
{
    c_ioctl(fd, NULL, 0, IOCTL_FLAG_SUSPEND);
    return 0;
}

struct file_operations ws2812_fops =
{
    .onwer = 0,
    .open = ws2812_open,
    .write = ws2812_write,
    .ioctl = ws2812_ioctl,
    .resume = ws2812_resume,
    .suspend = ws2812_suspend,
};

struct miscdevice ws2812_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ws2812b",
    .fops = &ws2812_fops,
    .nodename = "modules",
};

static int ws2812_init(void)
{
    return (misc_register(&ws2812_device));
}

modules_init(ws2812_init);






