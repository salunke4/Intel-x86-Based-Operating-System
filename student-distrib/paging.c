

#include "paging.h"
#include "terminal.h"


/* initializeTable()
 * 
 * initializes a page table
 * Inputs: None
 * Outputs: pass
 * Side Effects: sets address of video memory (0xB8) to present

 */

void initializeTable() {

    int i; 
    //1024 is the size of the table
    for (i=0; i < 1024; i++) {
       video_page_table[i].Present = 0;
       video_page_table[i].ReadWrite = 1;
       video_page_table[i].UserSupervisor = 0;
       video_page_table[i].WriteThrough= 0;
       video_page_table[i].CacheDisabled = 0;
       video_page_table[i].Accessed = 0;
       video_page_table[i].Dirty= 0;
       video_page_table[i].PageTableAttr = 0;
       video_page_table[i].GlobalPage = 1;
       video_page_table[i].ProgUse = 0;
       video_page_table[i].PageBaseAddr = 0x2;
    }
    //0xB8 is the virtual and physical address of video memory
    video_page_table[0xB8].PageBaseAddr = 0xB8;
    video_page_table[0xB8].Present = 1;


 video_page_table[0xB9].Present = 1;    
    video_page_table[0xB9].ReadWrite = 1;  
    video_page_table[0xB9].UserSupervisor = 1;
    video_page_table[0xB9].PageBaseAddr = 0xB9;



    video_page_table[0xBA].Present = 1;    
    video_page_table[0xBA].ReadWrite = 1;  
    video_page_table[0xBA].UserSupervisor = 1;
    video_page_table[0xBA].PageBaseAddr = 0xBA;


    video_page_table[0xBB].Present = 1;    
    video_page_table[0xBB].ReadWrite = 1;  
    video_page_table[0xBB].UserSupervisor = 1;
    video_page_table[0xBB].PageBaseAddr = 0xBB;

}

//function to initialize directories
/* initializeDirectory()
 * 
 * initializes the page directory
 * Side Effects: sets first entry to the page table defined above

 */
void initializeDirectory(){
    int i;
    //1024 is the size of the table
    for (i=0; i < 1024; i++) {
        PageDir[i].FourMB.Present = 0;
        PageDir[i].FourMB.ReadWrite = 1;
        PageDir[i].FourMB.UserSupervisor = 0;
        PageDir[i].FourMB.WriteThrough  = 0;
        PageDir[i].FourMB.CacheDisabled  = 0;
        PageDir[i].FourMB.Accessed  = 0;
        PageDir[i].FourMB.Dirty   = 0;
        PageDir[i].FourMB.PageSize  = 0;
        PageDir[i].FourMB.GlobalPage  = 0;
        PageDir[i].FourMB.ProgUse  = 0;
        PageDir[i].FourMB.PageTableAttr = 0;
        PageDir[i].FourMB.Reserved  = 0;

        PageDir[i].FourMB.PageBaseAddr  = 0x0; 
    }

    //initialize video memory

    PageDir[0].FourKB.Present = 1;
    PageDir[0].FourKB.ReadWrite = 0;
    PageDir[0].FourKB.UserSupervisor = 0;
    PageDir[0].FourKB.WriteThrough = 0;
    PageDir[0].FourKB.CacheDisabled = 0;
    PageDir[0].FourKB.Accessed= 0;
    PageDir[0].FourKB.Reserved = 0;
    PageDir[0].FourKB.PageSize = 0;
    PageDir[0].FourKB.GlobalPage = 0;
    PageDir[0].FourKB.ProgUse = 0;

    //shift by 12 because the last 12 bits don't matter
    PageDir[0].FourKB.PageBaseAddr = (uint32_t)video_page_table>>12;



    

    //map kernel
    PageDir[1].FourMB.Present = 1;
    PageDir[1].FourMB.ReadWrite = 1;
    PageDir[1].FourMB.UserSupervisor = 0;
    PageDir[1].FourMB.WriteThrough  = 0;
    PageDir[1].FourMB.CacheDisabled  = 0;
    PageDir[1].FourMB.Accessed  = 0;
    PageDir[1].FourMB.Dirty   = 0;
    PageDir[1].FourMB.PageSize    = 1;
    PageDir[1].FourMB.GlobalPage  = 1;
    PageDir[1].FourMB.ProgUse   = 0;
    PageDir[1].FourMB.PageTableAttr = 0;
    PageDir[1].FourMB.Reserved  = 0;

    //0x400000 is the virtual and physical address of the kernal
    //shift by 22 because the last 22 bits don't matter
    PageDir[1].FourMB.PageBaseAddr  = 0x400000 >>22;
    
    loadDirectory(PageDir); 
    enablePaging();
    return; 

}
//function to restore paging
/* sysCallPaging(uint32_t pid)
 * input: process id
 * 
 * restores paging

 */

void sysCallPaging(uint32_t pid){
    


    PageDir[32].FourMB.Present = 1;
    PageDir[32].FourMB.ReadWrite = 1;
    PageDir[32].FourMB.UserSupervisor = 1;
    PageDir[32].FourMB.WriteThrough  = 0;
    PageDir[32].FourMB.CacheDisabled  = 0;
    PageDir[32].FourMB.Accessed  = 0;
    PageDir[32].FourMB.Dirty   = 0;
    PageDir[32].FourMB.PageSize    = 1;
    PageDir[32].FourMB.GlobalPage  = 0; //mod
    PageDir[32].FourMB.ProgUse   = 0;
    PageDir[32].FourMB.PageTableAttr = 0;
    PageDir[32].FourMB.Reserved  = 0;


    PageDir[32].FourMB.PageBaseAddr = (0x800000 + (0x400000*pid)) >> 22;
    
    flush();
}
//terminal_t curr_term; // will have to use a curr term variable

int run_term =0;
int curr_term = 0; //have to use runningTerminal instead of this


unsigned int OFF = 0 ;     //have to calculate

//function to initialize vidmem
/* init_vidmem()
 

 */

void init_vidmem(){
    PageDir[0].FourKB.Present = 1;    // present
    PageDir[0].FourKB.ReadWrite = 1;
    PageDir[0].FourKB.UserSupervisor = 1;    // user mode
    PageDir[0].FourKB.PageBaseAddr   = (unsigned int)video_page_table >> 12;



    video_page_table[0xB9].Present = 1;    
    video_page_table[0xB9].ReadWrite = 1;  
    video_page_table[0xB9].UserSupervisor = 1;
    video_page_table[0xB9].PageBaseAddr = 0xB9;



    video_page_table[0xBA].Present = 1;    
    video_page_table[0xBA].ReadWrite = 1;  
    video_page_table[0xBA].UserSupervisor = 1;
    video_page_table[0xBA].PageBaseAddr = 0xBA;


    video_page_table[0xBB].Present = 1;    
    video_page_table[0xBB].ReadWrite = 1;  
    video_page_table[0xBB].UserSupervisor = 1;
    video_page_table[0xBB].PageBaseAddr = 0xBB;



    flush();
}

//function to remap vidmem
/* void remap_vidmem(uint32_t nextTerminalID)
 * input : next terminal id

 */

void remap_vidmem(uint32_t nextTerminalID)
{

//copy current video memory to current terminal's video memory
memcpy( (char*)(0xB9000 + curr_terminal_ID * 4*1024 ), (char*)0xB8000, 4*1024);

//copy next terminal's video memory to video memory
memcpy( (char*)0xB8000, (char*)(0xB9000 + nextTerminalID * 4*1024 ), 4*1024);




}
//function to remap vidmem for scheduler
/* void sch_vidmem()

 */


void sch_vidmem(){



uint32_t currentIndex = (0xB9000 + ((*runningTerminal).id) * 4*1024) >> 12;


if(curr_terminal_ID != ((*runningTerminal).id)){
 video_page_table[0xB8].PageBaseAddr  = currentIndex;
 
flush();

}
else{

video_page_table[0xB8].PageBaseAddr = 0xB8;
flush();

}


return;


}
