/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"


/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    //magic numbers - 0xff is master and slave mask & port + 1 = data
    // mask out all interrupts on the PIC 
    outb(0xff, (MASTER_8259_PORT + 1)); 
    outb(0xff, (SLAVE_8259_PORT + 1)); 
    // initialize the PIC with ICW1,ICW2,ICW3,ICW4
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_MASTER, (MASTER_8259_PORT + 1)); 
    outb(ICW2_SLAVE, (SLAVE_8259_PORT + 1));
    outb(ICW3_MASTER, (MASTER_8259_PORT + 1));
    outb(ICW3_SLAVE, (SLAVE_8259_PORT + 1)); //Slave attached to IR line 2 
    outb(ICW4, (MASTER_8259_PORT + 1));
    outb(ICW4, (SLAVE_8259_PORT + 1));
    // Mask all interrupts
    outb(0xff, (MASTER_8259_PORT + 1));
    outb(0xff, (SLAVE_8259_PORT + 1));

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    if (irq_num > 8 || irq_num <0) // if out of range return
        return;
    if (irq_num < 8) // is master IRQ_num = 0-7
    {
        // it is master IRQ. Set the corresponding bit to 0 
        master_mask = inb(MASTER_8259_PORT + 1); 
        master_mask &= ~(1 << irq_num); 
        outb(master_mask, MASTER_8259_PORT + 1); 
        return;
    }
    // Set the corresponding bit to 0 
    slave_mask = inb(SLAVE_8259_PORT+1); 
    slave_mask &= ~(1 << (irq_num - (8)));
    outb(slave_mask, SLAVE_8259_PORT+1); 
    return;
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    if (irq_num > 8 || irq_num <0) // if out of range return
        return;
    if (irq_num < 8) // is master IRQ_num = 0-7
    {
       //Set the corresponding bit to 1 
        master_mask = inb(MASTER_8259_PORT + 1); 
        master_mask |= 1 << irq_num; 
        outb(master_mask, MASTER_8259_PORT + 1); 
        return;
    }
    slave_mask = inb(SLAVE_8259_PORT+1); 
    slave_mask |= 1 << (irq_num - 8); 
    outb(slave_mask, SLAVE_8259_PORT+1); 
    return;
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if (irq_num > 15 || irq_num < 0) //maximum possible irq number is 15 if not then invalid
        return;
    if (irq_num < 8) {// check whether it is master IRQ_num, i.e. 0-7
        outb(EOI | irq_num, MASTER_8259_PORT);
        return;
    }// it is slave IRQ_num, i.e. 8-15
    outb(EOI | (irq_num - 8), SLAVE_8259_PORT);
    outb(EOI | 2, MASTER_8259_PORT);
}

