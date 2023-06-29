#ifndef _TERMINAL_H
#define _TERMINAL_H


#include "keyboard.h"
#include "lib.h"
#include "types.h"
#include "systemcalls.h"


extern int32_t terminal_open(const char* fname);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_write(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_init();
extern int32_t terminal_switch(uint32_t terminal_ID);
extern int32_t terminal_save(uint32_t terminal_ID);
extern int32_t terminal_return(uint32_t terminal_ID);
extern int32_t launch_first_terminal();

typedef struct terminal_t{

    int32_t pid;
    volatile uint8_t terminal_buffer[128];
    uint32_t x; //for cursor
    uint32_t y; //for cursor
    uint32_t id; //to access the terminal id of the current terminal
    uint32_t vidmem;
    volatile uint8_t terminal_buffer_ptr;
    volatile uint8_t read_the_terminal;
    int execute_run;
} terminal_t;

terminal_t multi_terminal[3];
uint32_t curr_terminal_ID;

terminal_t* runningTerminal;

#endif


