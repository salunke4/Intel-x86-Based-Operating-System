#define ASM 1


#include "x86_desc.h"

#define flagMask     0x200

.globl contextSwitch

// eip
//esp
//
contextSwitch:
    pushl %ebp
    movl %esp, %ebp

 
    
    # User DS first
    movl $USER_DS, %edx
    pushl %edx

    # ESP is first argument
    movl 12(%ebp), %edx
    pushl %edx

    # EFLAG, set IF to 1
    pushfl
    popl %eax
    movl $flagMask, %edx
    orl %edx, %eax 
    pushl %eax

    #  CS
    movl $USER_CS, %edx
    pushl %edx

    # EIP is second argument
    movl 8(%ebp), %edx
    pushl %edx

    iret
