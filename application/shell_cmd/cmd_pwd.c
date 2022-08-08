#include "shell_cmd.h"
#include "nv_flash.h"
#include "unit.h"
#include "password_list.h"
#include "stdlib.h"

extern nv_list pwd_list;

void debug_pwd_show_all(void)
{
    int32_t id = 0, cnt = 0;
    pwd_str pwd;

    nv_flash_getlock();
    for (uint32_t i = 0; i < pwd_list.max; i++)
    {
        nv_flash_read(pwd_list.index_addr + i * pwd_list.index_size, (uint8_t *)&id, 4);
        if (id != 0xFFFFFFFF)
        {
            nv_flash_read(pwd_list.data_addr + i * pwd_list.data_size, (uint8_t *)&pwd, pwd_list.data_size);
            log(DEBUG, "[%d]INDEX = %08X ,PWD = %d%d%d%d%d%d , TIME = %u\n", i, id, 
                pwd.pwd[0], pwd.pwd[1] , pwd.pwd[2] , 
                pwd.pwd[3], pwd.pwd[4] , pwd.pwd[5] , 
                pwd.time);
            cnt++;
        }
    }
    nv_flash_reslock();
    if (cnt == 0)
    {
        log(DEBUG, "设备没有临时密码\n");
    }
}

int debug_pwd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

    if (strcmp("-s", argv[1]) == 0)
    {
        if (strcmp("-a", argv[2]) == 0)
        {
            debug_pwd_show_all();
        }
    }
    if (strcmp("-clr", argv[1]) == 0)
    {
        nvlist_clear(&pwd_list);
    }

    if (strcmp("-a", argv[1]) == 0)
    {
        pwd_str apwd;

        apwd.time = atol(argv[3]);

        apwd.pwd[0] = str_to_hex(argv[2][0]);
        apwd.pwd[1] = str_to_hex(argv[2][1]);
        apwd.pwd[2] = str_to_hex(argv[2][2]);
        apwd.pwd[3] = str_to_hex(argv[2][3]);
        apwd.pwd[4] = str_to_hex(argv[2][4]);
        apwd.pwd[5] = str_to_hex(argv[2][5]);

        add_password(&apwd, &pwd_list);
    }

    if (strcmp("-m", argv[1]) == 0)
    {
        pwd_str apwd;
        uint32_t intpwd, cntMax;
        char strPwd[6];

        apwd.time = atol(argv[4]);

        intpwd = atol(argv[2]);
        cntMax = atol(argv[3]);

        if ((cntMax > 500000) || (intpwd > 999999) || ((intpwd + cntMax) > 999999))
        {
            log(WARN, "输入数字过大，请重新输入\n");
            return 0;
        }

        for (uint16_t i = 0; i < cntMax; i++)
        {
            sprintf(strPwd, "%06d", intpwd);
            apwd.pwd[0] = str_to_hex(strPwd[0]);
            apwd.pwd[1] = str_to_hex(strPwd[1]);
            apwd.pwd[2] = str_to_hex(strPwd[2]);
            apwd.pwd[3] = str_to_hex(strPwd[3]);
            apwd.pwd[4] = str_to_hex(strPwd[4]);
            apwd.pwd[5] = str_to_hex(strPwd[5]);
            add_password(&apwd, &pwd_list);
            intpwd++;
        }
    }

    return 1;
}

SHELL_CMD_REG(
    pwd, 6, 1, debug_pwd,
    "pwd\t-一次性密码调试  \n",
    "\t-一次性密码调试\n\t\
     pwd -a [pwd] , [timer] , examp:pwd -a 1111 1900000000 增加临时密码\n\t\
	 pwd -m [pwd],[add number],[timer] , examp:pwd -m 1111 10 1900000000 批量增加临时密码\n\t\
     pwd -clr 清空过期密码\n");
