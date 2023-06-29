#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define RTC_IRQ 8  
#define SLAVE_IRQ 2

volatile int rtc_interrupt;

extern void rtc_init();
extern void rtc_handler();

extern int32_t rtc_open(const char* filename);

extern int32_t rtc_close(int32_t fd);

extern int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

extern int32_t rtc_write (int32_t fd, void* buf, int32_t nbytes);

extern int rtc_change_frequency(int frequency);






#endif 
