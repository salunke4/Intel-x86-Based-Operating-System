#ifndef _SYSTEMCALLS_H
#define _SYSTEMCALLS_H

#include "types.h"
#include "filesys.h"
#include "terminal.h"
#include "rtc.h"
#include "lib.h"
#include "paging.h"
#include "x86_desc.h"


#define VIDMAP_OFFSET           35          /* 140/4 */

//pcb_t* pcb_adress(uint32_t pid);

extern void sysCallPaging(uint32_t pid);

int32_t sys_open(const char* fname);

int32_t sys_close(int32_t fd);

int32_t sys_read(int32_t fd, void* buf, int32_t nbytes);

int32_t sys_write(int32_t fd, void* buf, int32_t nbytes);

int32_t system_execute(const uint8_t* cmd);

int32_t halt(int32_t status);

int32_t null_open();

int32_t null_read();

int32_t null_write();

int32_t null_close();

extern int32_t contextSwitch(uint32_t buf,uint32_t espArg);

int32_t vidmap(uint8_t** screen_start);

int32_t getargs(uint8_t* buf, int32_t nbytes);

int32_t set_handler(int32_t signum, void* handler_address);

int32_t sigreturn(void);

void file_op_table_init();

void scheduler();

int set_running_terminal();


/* struct for fops */


// pointer pointing to fd array
file_descriptor_t* file_descriptor_array;

typedef struct pcb_t {
    int32_t pid;    // file operator table 
    file_descriptor_t file_descriptor[8];   // inode index 
    uint32_t saved_ebp; 
    uint32_t saved_esp;   
    int32_t parent_id;
    uint32_t ss0;   
    uint32_t esp0;
    uint8_t arg[128];
    int active;

} pcb_t;




#endif




