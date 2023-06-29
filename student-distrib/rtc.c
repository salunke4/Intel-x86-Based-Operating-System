#include "rtc.h"
#include "lib.h"
#include "i8259.h"
//void test_interrupts();


volatile int rtc_interrupt;

volatile int rtc_count = 512;

volatile int rtc_max = 512;



/* rtc_init
 * 
 * initializes rtc
 */
void rtc_init()
{   

    cli();	
    outb(0x8B, 0x70);		// select register B, and disable NMI
    char prev = inb(0x71);	// read the current value of register B
    outb(0x8B, 0x70);		// set the index again (a read will reset the index to register D)
    outb(prev  | 0x40, 0x71);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    rtc_max = 512;





    
   

    enable_irq(RTC_IRQ);
    enable_irq(SLAVE_IRQ); 
    rtc_change_frequency(1024);
    sti();
    
}
/* rtc_handler
 * 
 * handles rtc interrupts
 */
void rtc_handler() {
    /* mask the interrupts*/
    cli();
    //test_interrupts();
    outb(0x0C, 0x70); // select register C
    inb(0x71); // just throw away contents

    rtc_count--;
    if(rtc_count == 0){
        rtc_interrupt = 1;
        rtc_count = rtc_max;
    }

    //rtc_interrupt = 1;
    // end interrupt
    send_eoi(RTC_IRQ);
    
    sti();
}



// RTC open() 
/* 
 * 
 * input: filename
 */
//initializes RTC frequency to 2HZ, return 0
int32_t rtc_open (const char* filename) {
    // rtc_change_frequency(2);             // set frequency to 2 mod
    // int rate = 0x0F;			// rate must be above 2 and not over 15
    // outb(0x8A, 0x70);		// set index to register A, disable NMI
    // char prev = inb(0x71);	// get initial value of register A
    // outb(0x8A, 0x70);		// reset index to A
    // outb((prev & 0xF0) | rate, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.

    rtc_max = 512;
    
    return 0;
}

//RTC close() 
//input: fd
//does nothing, return 0
int32_t rtc_close(int32_t fd) {
    return 0;                        // do nothing 
}
//rtc_read()
//input: fd,buf,nbytes
// RTC read() should block until the next interrupt, 
//return 0
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes) {
    sti();
    rtc_interrupt = 0;
    sti();
    while(rtc_interrupt == 0);                       
    return 0;
}


//RTC write() must be able to change frequency, return 0 or -1
int32_t rtc_write (int32_t fd, void* buf, int32_t nbytes) {
    int32_t frequency;
    
    if (buf == NULL) {                      // if buffer is null write will fail
        return -1;
    }
    
    frequency = *((int32_t*) buf);
    //rtc_change_frequency(*((int32_t*) buf));  // change frequency but first convert input to int32_t
    // if(rtc_change_frequency(*((int32_t*) buf)) == -1){
    //     return -1;
    // }


    cli();
    rtc_max = 1024/frequency;
    sti();
    return 0;                           // else return success
    

    
}

//  RTC change frequency() changes the frequency of the RTC by converting frequency to rate and then modifying appropriate registers
int rtc_change_frequency(int frequency) {
    char rate;

    switch(frequency) {       // convert frequency to hex value of rate
        case 2:
			rate = 0x0F;
			break;
		case 4:
			rate = 0x0E;
			break;
		case 8:
			rate = 0x0D;
			break;
		case 16:
			rate = 0x0C;
			break;
		case 32:
			rate = 0x0B;
			break;
		case 64:
			rate = 0x0A;
			break;
		case 128:
			rate = 0x09;
			break;
		case 256:
			rate = 0x08;
			break;
		case 512:
			rate = 0x07;
			break;
		case 1024:
			rate = 0x06;
			break;
		default:
			return -1;
			break;
    }
    
    
   
    cli();                 //mask interrupt
	
    outb(0x8A, 0x70);		// set index to register A, disable NMI
    char prev = inb(0x71);	// get initial value of register A
    outb(0x8A, 0x70);		// reset index to A
    outb(((prev & 0xF0) | rate), 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.

	sti();                    // enable interrupt 
	return 1;

}




