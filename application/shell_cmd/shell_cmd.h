#ifndef __SHELL_CMD_H
#define __SHELL_CMD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"
#include "unit.h"
#include "miscdevice.h"
#include "priv_list.h"


#define	CFG_PROMPT			"Shell> "	/* Monitor Command Prompt	*/
#define CFG_CBSIZE	64		/* Console I/O Buffer Size	*/
#define	CFG_MAXARGS	16		/* max number of command args	*/

#define CLEAR() printf("\033[2J") 
  
#pragma section = "_shell_cmd"

#define SHELL @ "_shell_cmd"

struct cmd_tbl_s
{
    char *name;                 /* Command Name			*/
    int maxargs;                /* maximum number of arguments	*/
    int repeatable;             /* autorepeat allowed?		*/
    /* Implementation function	*/
    int (*cmd)(struct cmd_tbl_s *, int, int, char *[]);
    char *usage; /* Usage message	(short)	*/
    char *help;  /* Help  message	(long)	*/
};

typedef struct cmd_tbl_s cmd_tbl_t;

#define SHELL_CMD_REG(name,maxargs,rep,cmd,usage,help) \
__root cmd_tbl_t __shell_cmd_##name SHELL= {#name, maxargs, rep, cmd, usage, help}



//extern int run_command(const char *cmd, int flag);
//extern int readline(const char *const prompt);

#ifdef __cplusplus
}
#endif

#endif /* __SHELL_CMD_H */
