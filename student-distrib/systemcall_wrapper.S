#define ASM 1

.globl system_call_linkage

system_call_linkage:
    pushfl
    pushl %ebp          /* save your registers */
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
   
    pushl %edx             /* third arg */
    pushl %ecx             /* second arg */
    pushl %ebx            /* first arg */


    cmp $1, %eax          /* check value of system call is less than 10 and more than 1 to see if it is out of bounds */
    jl invalid
    cmp $10, %eax
    jg invalid

    call *jumptable(, %eax, 4)   
    jmp pop_registers

invalid:
    movl $-1, %eax
    
pop_registers:
    addl    $12, %esp
    popl %ebx
    popl %ecx
    popl %edx 
    popl %esi     
    popl %edi
    popl %ebp
    popfl
    iret
   

jumptable:
    .long 0x0                             /* there is no system call number 0 */
    .long halt
    .long system_execute
    .long sys_read
    .long sys_write
    .long sys_open
    .long sys_close
    .long getargs
    .long vidmap
