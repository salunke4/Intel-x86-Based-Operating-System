#define ASM 1
#include "wrapper.h"

/* macro for push all genral registers and struct pt regs */
#define pushall     \
    pushl   %fs;    \
    pushl   %es;    \
    pushl   %ds;    \
    pushl   %eax;   \
    pushl   %ebp;   \
    pushl   %edi;   \
    pushl   %esi;   \
    pushl   %edx;   \
    pushl   %ecx;   \
    pushl   %ebx;

/* macro for restore all genral registers and struct pt regs */
#define popall      \
    popl    %ebx;   \
    popl    %ecx;   \
    popl    %edx;   \
    popl    %esi;   \
    popl    %edi;   \
    popl    %ebp;   \
    popl    %eax;   \
    popl    %ds;    \
    popl    %es;    \
    popl    %fs;

/* keyboard interrupt linkage code */
#define INTERRUPT_WRAPPER(name,function) \
        .globl name               ;\
        name:                     ;\
                pushall           ;\
                call function     ;\
                popall            ;\
                iret         


/*call the functions*/
INTERRUPT_WRAPPER(pit_wrap,pit_handler)
INTERRUPT_WRAPPER(rtc_wrap,rtc_handler)
INTERRUPT_WRAPPER(keyboard_wrap, handler_keyboard)



