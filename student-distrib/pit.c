#include "pit.h"
#include "lib.h"
#include "systemcalls.h"
#include "i8259.h"

/* pit_init
 *
 * initialize pit
 * Inputs: none
 * Outputs: none
 * Return: none
 */
void pit_init()
{   

    

    int divisor = 1193180 / PIT_HZ;       /* Calculate our divisor */
    outb(0x36, 0x43);             /* Set our command byte 0x36 */
    outb(divisor && 0xFF, 0x40);   /* Set low byte of divisor */
    outb(divisor >> 8, 0x40);     /* Set high byte of divisor */
    enable_irq(0x0);
  
    
}
/* pit_handler
 *
 * sends end of interrupt to pit and calls scheduler
 * Inputs: none
 * Outputs: none
 * Return: none
 */

void pit_handler()
{   
    send_eoi(0x0);
    //scheduler();
}
