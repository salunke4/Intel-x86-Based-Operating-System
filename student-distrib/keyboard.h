#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"
#include "i8259.h"
#include "terminal.h"

#define KEYBOARD_IRQ 1       //the irq num for keybaord 


extern void init_keyboard();
extern void handler_keyboard();
extern void clear_buffer();


#endif

