#ifndef _PIT_H
#define _PIT_H


#include "terminal.h"

/*Constants to set the PIT frequency*/
#define PIT_HZ        100       // frequency in Hz


void pit_init();
void pit_handler();

#endif
