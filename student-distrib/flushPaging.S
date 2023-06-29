

#define ASM 1



.globl flush
.align 4
flush:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

