#ifndef _IDT_H
#define _IDT_H

/* Initialize IDT */
extern void idt_init();
extern void divide_error();
extern void Reserved(); 
extern void NMI_INTERRUPT();
extern void BREAKPOINT();  
extern void OVERFLOW();
extern void BOUND();
extern void INVALID_OPCODE();
extern void DEV_NOT_AVAILABLE();
extern void DOUBLE_FAULT();
extern void SEGMENT_OVERRUN();
extern void INVALID_TSS();   
extern void SEGMENT_NOT_PRESENT();
extern void STACK_SEGMENT_FAULT();
extern void GENERAL_PROTECTION();
extern void PAGE_FAULT();
extern void MATH_FAULT();
extern void ALIGNMENT_CHECK();
extern void MACHINE_CHECK();
extern void SIMD_FLOATING_POINT_CHECK();
extern void systemcall();
extern void system_call_linkage();
#endif
