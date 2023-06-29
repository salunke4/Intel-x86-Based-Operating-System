#include "keyboard.h"


static unsigned int caps_flag = 0;
static unsigned int shift_flag = 0;
static unsigned int control_flag = 0;
static unsigned int alt_flag = 0;

void init_keyboard()
{
    enable_irq(KEYBOARD_IRQ);
}
//nothing-----shift only----caps only----shift and caps
// used the scan code given on https://wiki.osdev.org/PS/2_Keyboard for a "US QWERTY" keyboard only
unsigned char keyboard_map[128] =
{
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8','9', '0', '-', '=', '\b', // 0 isnt mapped--- 0 = esc key---- 0-9 ----- symbols---- backspace
    ' ','q', 'w', 'e', 'r','t', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // tab---qwerty--- brackets---enter
    0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',   0,  //left control (responds to nothing on the kernel)----qwerty----symbols--- left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n','m', ',', '.', '/', 0,'*', //symbol----qwerty----right shift
    0,  // Alt 
    ' ',  // Space bar 
    0,  // Caps lock 
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,  // F1 - F10 
    0,  // 69 - Num lock
    0,  // Scroll Lock 
    0,  // Home key 
    0,  // Up Arrow 
    0,  // Page Up 
    '-',
    0,  // Left Arrow 
    0,
    0,  // Right Arrow 
    '+',
    0,  // 79 - End key
    0,  // Down Arrow 
    0,  // Page Down 
    0,  // Insert Key 
    0,  // Delete Key 
    0,   0,   0,
    0,  // F11 Key 
    0,  // F12 Key 
    0,  // All other keys are undefined 
    };
unsigned char keyboard_shift_map[128] =
{
    0,  0, '!', '@', '#', '$', '%', '^', '&', '*','(', ')', '_', '+', '\b', // 0 isnt mapped--- 0 = esc key---- 0-9 ----- symbols---- backspace
    ' ','Q', 'W', 'E', 'R','T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', // tab---qwerty--- brackets---enter
    0,'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~',   0,  //left control (responds to nothing on the kernel)----qwerty----symbols--- left shift
    '|', 'Z', 'X', 'C', 'V', 'B', 'N','M', '<', '>', '?', 0,'*', //symbol----qwerty----right shift
    0,  // Alt 
    ' ',  // Space bar 
    0,  // Caps lock 
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,  // F1 - F10 
    0,  // 69 - Num lock
    0,  // Scroll Lock 
    0,  // Home key 
    0,  // Up Arrow 
    0,  // Page Up 
    '-',
    0,  // Left Arrow 
    0,
    0,  // Right Arrow 
    '+',
    0,  // 79 - End key
    0,  // Down Arrow 
    0,  // Page Down 
    0,  // Insert Key 
    0,  // Delete Key 
    0,   0,   0,
    0,  // F11 Key 
    0,  // F12 Key 
    0,  // All other keys are undefined     
};
unsigned char keyboard_caps_map[128] =
{
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8','9', '0', '-', '=', '\b', // 0 isnt mapped--- 0 = esc key---- 0-9 ----- symbols---- backspace
    ' ','Q', 'W', 'E', 'R','T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', // tab---qwerty--- brackets---enter
    0,'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';','\'', '`',   0,   //left control (responds to nothing on the kernel)----qwerty----symbols--- left shift
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N','M', ',', '.', '/', 0,'*',//symbol----qwerty----right shift
    0,  // Alt 
    ' ',  // Space bar 
    0,  // Caps lock 
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,  // F1 - F10 
    0,  // 69 - Num lock
    0,  // Scroll Lock 
    0,  // Home key 
    0,  // Up Arrow 
    0,  // Page Up 
    '-',
    0,  // Left Arrow 
    0,
    0,  // Right Arrow 
    '+',
    0,  // 79 - End key
    0,  // Down Arrow 
    0,  // Page Down 
    0,  // Insert Key 
    0,  // Delete Key 
    0,   0,   0,
    0,  // F11 Key 
    0,  // F12 Key 
    0,  // All other keys are undefined     
};
unsigned char keyboard_both_map[128] =
{
    0,  0, '!', '@', '#', '$', '%', '^', '&', '*','(', ')', '_', '+', '\b', // 0 isnt mapped--- 0 = esc key---- 0-9 ----- symbols---- backspace
    ' ','q', 'w', 'e', 'r','t', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', // tab---qwerty--- brackets---enter
    0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':','\"', '~',   0,  //left control (responds to nothing on the kernel)----qwerty----symbols--- left shift
    '|', 'z', 'x', 'c', 'v', 'b', 'n','m', '<', '>', '?', 0,'*', //symbol----qwerty----right shift
    0,  // Alt 
    ' ',  // Space bar 
    0,  // Caps lock 
    0,  0,   0,   0,   0,   0,   0,   0,   0,  0,  // F1 - F10 
    0,  // 69 - Num lock
    0,  // Scroll Lock 
    0,  // Home key 
    0,  // Up Arrow 
    0,  // Page Up 
    '-',
    0,  // Left Arrow 
    0,
    0,  // Right Arrow 
    '+',
    0,  // 79 - End key
    0,  // Down Arrow 
    0,  // Page Down 
    0,  // Insert Key 
    0,  // Delete Key 
    0,   0,   0,
    0,  // F11 Key 
    0,  // F12 Key 
    0,  // All other keys are undefined 
    };
/* handler_keyboard
 * 
 * helps parse through the keyboard interupts and turns it into ascii
 * Inputs: none
 * Outputs: none
 */
void handler_keyboard(){
    unsigned char keydata = 0;
    int i;
    // mask interrupt
    //cli();
    send_eoi(1); //enable the pic interupt
    terminal_t* current_terminal = &multi_terminal[curr_terminal_ID];
    volatile uint8_t* key_buffer = current_terminal->terminal_buffer;
    // wait for interrupt
    while(1){
        if (inb(EOI)){
            keydata = inb(EOI);
            break;
        }
    }
    // check if key is valid
    if(keydata == 0x3A){ //capslock
        caps_flag = ~caps_flag;
    }  
    else if(keydata == 0x2A || keydata == 0x36){//left shift pressed or right shift pressed
        shift_flag = 1;
    }  
    else if(keydata == 0xAA || keydata == 0xB6){//left shift released or right shift released
        shift_flag = 0;
    }    
    else if(keydata == 0x1D){ //control pressed
        control_flag = 1;
    }  
    else if(keydata == 0x9D){ //control released
        control_flag = 0;
    }   
    else if(keydata == 0x38){ //alt pressed
        alt_flag = 1;
    }  
    else if(keydata == 0xB8){ //alt released
        alt_flag = 0;
    }  
    else if(keydata == 0x1C){ //enter pressed
        key_buffer[current_terminal->terminal_buffer_ptr] = '\n';
        (current_terminal->terminal_buffer_ptr) = (current_terminal->terminal_buffer_ptr) + 1; 
        current_terminal->read_the_terminal = 1;
        terminal_newline();
    }  
    else if(keydata == 0x0E){ //backspace
        if (current_terminal->terminal_buffer_ptr>0){
            current_terminal->terminal_buffer_ptr = current_terminal->terminal_buffer_ptr - 1 ;
            key_buffer[current_terminal->terminal_buffer_ptr] = ' ';
            terminal_backspace();
        }
    } 
    else if(keydata == 0x0F){ //tab
        for (i=0; i<4; i++){
            key_buffer[current_terminal->terminal_buffer_ptr] = ' ';
            current_terminal->terminal_buffer_ptr = current_terminal->terminal_buffer_ptr + 1; 
            putc(' ');
        }
    } 
    else{
        if(alt_flag){
            if (keydata == 0x3B){
                terminal_switch(0);
            }
            else if (keydata == 0x3C){
                terminal_switch(1);
            }
            else if (keydata == 0x3D){
                terminal_switch(2);
            }
        }
        if(keydata<128){
            unsigned char keyprint;
            //print_key(keydata);
            if (shift_flag & caps_flag){
                keyprint = keyboard_both_map[keydata];
            }
            else if (shift_flag & (~caps_flag)){
                keyprint =keyboard_shift_map[keydata]; 
            }
            else if ((~shift_flag) & caps_flag){
                keyprint = keyboard_caps_map[keydata];
            }
            else{
                keyprint = keyboard_map[keydata];
            }       
            //print out the keys 

            if (current_terminal->terminal_buffer_ptr < 128){
                if (control_flag){
                    if (keyprint == 'l' || keyprint == 'L'){
                        terminal_reset();
                        clear();
                    }
                }
                if(keyprint != 0){
                    key_buffer[current_terminal->terminal_buffer_ptr] = keyprint;
                    current_terminal->terminal_buffer_ptr += 1;
                    putc(keyprint);                    
                }
            }
        }
    }
    // end interrupt
    send_eoi(KEYBOARD_IRQ);
    // enable interrupt
    sti();
}

void clear_buffer(){
    terminal_t* current_terminal = &multi_terminal[curr_terminal_ID];
    volatile uint8_t* key_buffer = current_terminal->terminal_buffer;
    int i;
    for (i=0; i<127; i++){
        key_buffer[i] = 0;  
    }  
    current_terminal->terminal_buffer_ptr = 0;
	current_terminal->read_the_terminal = 0;
    return;
}
