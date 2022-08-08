#include "shell_cmd.h"

#include "unit.h"

int do_ls(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    uint8_t total = 0;
    uint8_t all_flag = 0;
    
    if (( strlen(argv[1]) > 0)&&(argc == 2))
    {
        list *misclist = misc_get_list();
        list_iter *iter;
        list_node *node;
        DEVICE_OPS *dev;
        iter = list_get_iterator(misclist, START_FROM_HEAD);
        
        if ((strncmp("all" ,(char *)argv[1] , strlen(argv[1]))) == 0)
        {
            all_flag = 1;
        }
        
        while ((node = list_next(iter)) != NULL)
        {
            dev = (DEVICE_OPS *)(node->value);

            if ((all_flag==1)||
               ((strncmp((char *)dev->nodename ,(char *)argv[1] , strlen(argv[1]))) == 0))
            {
                total++;
                PRINTK(DEBUG,"%-16s\t%d\t%d\t%d\t%s\n" , dev->name ,dev->used, dev->major  , dev->minor , dev->nodename);  
            }           
 
        }
        list_release_iterator(iter);
        if( total == 0)
        {
            PRINTK(WARN,"ls: cannot access '%s': No such file or directory.\n",argv[1]);
        }
        else
        {
            PRINTK(INFO,"total %d\n" , total);
        }
    }

    return 0;
}

SHELL_CMD_REG(
    ls, 4, 1, do_ls,
    "ls\t打印相应命令\n",
    "\t打印命令对应的输出\n\n\
ls arch\t打印所有有关平台的资源名的相信信息\n");
