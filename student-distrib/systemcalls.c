#include "systemcalls.h"


#define progImage 0x08048000
#define mb_8 0x800000
#define kb_8 0x2000
#define start 0xB8000
#define PHYS_MEM_BASE 0x08400000
#define PHYS_MEM_TOP 0x08000000
#define VID_MEM 0xB8000








static fops_table_t fopsarray[6];
int32_t pid = -1;

int32_t pidArray[6] = {0,0,0,0,0,0};



/* helper function to get the adress of your pcb
Input: PID number
Output: A pointer to your adress that has that PCB */

pcb_t* pcb_adress(uint32_t in){
    return (pcb_t*) (mb_8 - ((in+1) * kb_8));                  // mb_8 0x800000 kb_8 0x2000
}
/* sys_open
 * 
 * will call particulara device's open function according to the file type 
 * Inputs: fname is file name of the file that needs to be opened
 * Outputs: fd index for sucess and -1 if failed
 */
int32_t sys_open(const char* fname){
    //printf("2");
    //printf("sys_open \n");
    dentry_t dentry;
    int fd_id;
    if(fname == NULL || file_descriptor_array == NULL)
        return -1;
    if(strlen((char*)fname) == NULL){
        return -1;  /* empty string*/
    }
    // find unused file descriptor 
    for(fd_id = 2; fd_id < 8; fd_id++){
        if(file_descriptor_array[fd_id].flags == 0)
            break;
    }
    // fail if reach the max file number or could not find the file 
    if (fd_id >= 8 || read_dentry_by_name((uint8_t*)fname, &dentry) != 0)
        return -1;
    //if there are no errors call the fop
    file_descriptor_array[fd_id].op = &fopsarray[dentry.filetype];
    file_descriptor_array[fd_id].filetype = dentry.filetype;
    file_descriptor_array[fd_id].offset = 0;
    memcpy(file_descriptor_array[fd_id].filename, dentry.filename, sizeof(dentry.filename));
    if ((file_descriptor_array[fd_id].op->sys_open(fname)) == -1){
        return -1;
    }
    //set the file descriptor
    if(dentry.filetype == 2){
        file_descriptor_array[fd_id].inode =  dentry.inode_num;
    }
    else{
        file_descriptor_array[fd_id].inode = -1;
    }
    file_descriptor_array[fd_id].fileoffset = 0;
    file_descriptor_array[fd_id].flags = 1;

    return fd_id;
}
/* sys_close
 * 
 * will call particulara device's close function according to the file type 
 * Inputs: file discriptor array index of the file to be closed
 * Outputs: 0 for success -1 for fail
 */
int32_t sys_close(int32_t fd){
    //printf("4");
     //printf("sys_close \n");
    if (fd < 2 || fd > 8 || file_descriptor_array == NULL || file_descriptor_array[fd].flags == 0)
        return -1;
    
    if ((file_descriptor_array[fd].op->sys_close(fd)) == 0)// close the file and clear the file descriptor 
    {
        // if successfully closed, clear the file descriptor 
        file_descriptor_array[fd].op = NULL;
        file_descriptor_array[fd].inode = -1;
        file_descriptor_array[fd].fileoffset = 0;
        file_descriptor_array[fd].flags = 0;
    }
    return 0;
}
/* sys_read
 * 
 * will call particular device's read function according to the file type
 * Inputs: fd - file descripor; buf - a buffer that hold the terminal input; nbytes - the number of bytes to read from the keyboard buffer
 * Outputs: the actual number of bytes that are read successfully, if error then ret -1
 */
int32_t sys_read(int32_t fd, void* buf, int32_t nbytes){
    //printf("3");
    //printf("sys_read \n");
    if (fd < 0 || fd > 8 || buf == NULL || file_descriptor_array == NULL || file_descriptor_array[fd].flags == 0){
        return -1;
    }

    if(file_descriptor_array[fd].filetype == 1){
        int32_t bytesRead = file_descriptor_array[fd].op->sys_read((int32_t)&file_descriptor_array[fd], buf, nbytes);
        return bytesRead;
    }
    if(file_descriptor_array[fd].filetype == 2){
        int32_t bytesRead = file_descriptor_array[fd].op->sys_read((int32_t)&file_descriptor_array[fd], buf, nbytes);
    
        return bytesRead;
    }
    return file_descriptor_array[fd].op->sys_read(fd, buf, nbytes);
}
/* sys_write
 * 
 * will call particular device's write function according to the file type
 * Inputs: fd - file descripor; buf - a buffer that hold the terminal input; nbytes - the number of bytes to write from the  buffer to terminal
 * Outputs: the actual number of bytes that are written successfully, if error then ret -1
 */
int32_t sys_write(int32_t fd, void* buf, int32_t nbytes){
    //printf("4");
    //printf("sys_write \n");
    if (fd < 0 || fd > 8 || buf == NULL || file_descriptor_array == NULL || file_descriptor_array[fd].flags == 0 || file_descriptor_array[fd].op == NULL){
        return -1;
    }
    return file_descriptor_array[fd].op->sys_write(fd, buf, nbytes);
}

/* halt
 * 
 * The halt system call terminates a process, returning the specified value to its parent process
 * Inputs: status
 * Outputs: 0 for succes, -1 for failure
 */


int32_t halt(int32_t status){
    pcb_t* PCB;
    pcb_t* PCB_parent;
 //checking pcb
    PCB = pcb_adress(pid);
    if(!PCB){
        return -1;
    }
//checking pid
    if (pid<-1){
        return -1;
    }
    int fd;
    for(fd = 2; fd < 8; fd++){
        if(file_descriptor_array[fd].flags)
            sys_close(fd);
    }
    // clear stdin fd 
    file_descriptor_array[0].op = NULL;
    file_descriptor_array[0].flags = 0;
    // clear stdout fd 
    file_descriptor_array[0].op = NULL;
    file_descriptor_array[0].flags = 0;

   
    // clear flags 
    int i;
    for (i = 0; i < 8; i++) {
        PCB->file_descriptor[i].flags = 0;
       
    }


    pidArray[pid] = 0;
    pid = PCB->parent_id;
    
    multi_terminal[curr_terminal_ID].pid = pid;
    
    if (pid == -1) {
        system_execute((uint8_t *) "shell");
        return -1;
    }
    

    PCB_parent = pcb_adress(pid);
   

   
 

    file_descriptor_array = PCB_parent->file_descriptor;
    // pid = PCB_parent->pid;

    // restore parent paging 
    sysCallPaging(pid);
    tss.ss0 = KERNEL_DS;
    tss.esp0 = (mb_8 - ((pid) * kb_8) - 4); 

    uint32_t sendstatus = (uint8_t)status;
    putc('\n');
    asm volatile ("                 \n\
        movl    %0, %%esp           \n\
        movl    %1, %%ebp           \n\
        movl    %2, %%eax           \n\
        jmp end_of_execute          \n\
        "
        :
        : "a"(PCB->saved_esp), "b"(PCB->saved_ebp), "c"(sendstatus)
    );

    return 0;
}
   


//null_open
//empty function


int32_t null_open(){
    return -1;
   
}
//null_close
//empty function

int32_t null_close(){
    return -1;
   
}
//null_read
//empty function

int32_t null_read(){
    return -1;
   
}
//null_write
//empty function

int32_t null_write(){
    return -1;
   
}
/* file_op_table_init
 * 
 * initallize the fops table
 * Inputs: none
 * Outputs: none
 */
void file_op_table_init()
{
    // init rtc operation table 
    fopsarray[0].sys_open  = rtc_open;
    fopsarray[0].sys_close = rtc_close;
    fopsarray[0].sys_read  = rtc_read;
    fopsarray[0].sys_write = rtc_write;

    // init dir operation table 
    fopsarray[1].sys_open  = open_directory ;
    fopsarray[1].sys_close = close_directory;
    fopsarray[1].sys_read  = read_directory ;
    fopsarray[1].sys_write = write_directory;

    // init file operation table 
    fopsarray[2].sys_open  = open;
    fopsarray[2].sys_close = close;
    fopsarray[2].sys_read  = read_file;
    fopsarray[2].sys_write = write;

    // stdin
    fopsarray[3].sys_open  = terminal_open;
    fopsarray[3].sys_close = terminal_close;
    fopsarray[3].sys_read  = terminal_read;        // would be 1 cuz read only
    fopsarray[3].sys_write = null_write;         // would be 0 cuz read only

    //stdout
    fopsarray[4].sys_open  = terminal_open;
    fopsarray[4].sys_close = terminal_close;
    fopsarray[4].sys_read  = null_read;          // would be 1 cuz write only
    fopsarray[4].sys_write = terminal_write;          // would be 1 cuz write only

    //null
    
    fopsarray[5].sys_open  = null_open;
    fopsarray[5].sys_close = null_close;
    fopsarray[5].sys_read  = null_read;         
    fopsarray[5].sys_write = null_write;         
}

/* system_execute
 * 
 * The execute system call attempts to load and execute a new program, handing off the processor to the new program
   until it terminates.
 * Inputs: command -- The command is a space-separated sequence of words. 
 * Outputs: 0 for success, if error then ret -1
 */


int32_t system_execute(const uint8_t* command){
    //printf("1");
    //uint8_t argument[10];
   
    if(command == NULL){
        return -1;
    } 

    pcb_t* PCB;
    pcb_t* PCB_parent;
    dentry_t dentry;
    int32_t i;

    uint8_t filename[128];
    uint8_t argument[128];
    int length = strlen((const int8_t*)command);
    int file_start = 0;    //file_cmd first index
    int arg_start = 0;
    int blank_count = 0;
    int file_cmd_length = 0;
    int file_arg_length = 0;
    int j;

    for (i=0;i<128;++i){
      filename[i] = '\0';
      argument[i] = '\0';
    }

    //parse cmd
    for(i = 0; i < length; ++i){
        //Gets the args and file name from the command word.
        if(command[i] != ' '){
            filename[file_start] = command[i];
            ++file_cmd_length;
            ++file_start;
        }
        else{
            ++blank_count;
            if(file_cmd_length > 0)
                break;
        }
    }

    //parse arg
    for(i = file_start+blank_count; i< length; ++i){
        if(command[i] != ' '){
            for(j=i;j<length;j++){
                argument[arg_start] = command[j];
                ++arg_start;                
            }    
            break;
        }
        else{
            if(file_arg_length > 0)
                break;
        }
    }


    if (read_dentry_by_name(filename, &dentry) == -1){
        return -1;
    } 

    /* parsing arguments */
    int32_t parent_id = pid;
    PCB_parent = pcb_adress(pid); 

    uint32_t k;
    uint32_t total_process = 0;
    for(k = 0; k<6;k++){

        if(pidArray[k]==0){
            pid = k;
            total_process = 1;
            pidArray[k] = 1;
            break;
        }
        else{
            total_process+=1;
        }
    }

    if(total_process >= 6){
        printf("Max number of processes running \n");
        return -1;
    }

    runningTerminal->pid = pid;

    /* SET UP PAGING */
    sysCallPaging(pid);
    /* loading file */
    inode_t* file_inode = (inode_t*)(inode_start + dentry.inode_num);
    int32_t errCheck = read_file_2((int32_t)filename,(uint8_t*)progImage, file_inode->length );



    if (errCheck == -1){
        return -1;
    } 
    uint8_t* file = (uint8_t*)progImage;


    /* Checks ELF magic constant */     
    if ((file[0] != 0x7F) || (file[1] != 0x45) || (file[2] != 0x4C) || (file[3] != 0x46)){
        return -1;
    } 

    

    if(errCheck==-1)
    {
        return -1;
    }

    /* SET UP PCB */

    register uint32_t saved_ebp asm("ebp");
    register uint32_t saved_esp asm("esp");
    file_op_table_init();
    
    PCB = pcb_adress(pid);
    
   
    
    PCB->pid = pid;
    
    PCB->parent_id = parent_id;
    PCB->saved_ebp = saved_ebp;
    PCB->saved_esp = saved_esp;
    PCB->active = 1;
    
    for(i = 0; i < 8; i++){
        PCB->file_descriptor[i].op = &fopsarray[5];    // file operator table (in case of null)
        PCB->file_descriptor[i].inode = 0 ;     // inode index 
        PCB->file_descriptor[i].fileoffset = 0; // offset in current file 
        PCB->file_descriptor[i].flags = 0;
               
        if(i==0||i==1){
            PCB->file_descriptor[i].flags = 1;
            PCB->file_descriptor[i].op = &fopsarray[i+3]; //(terminal)
        }
    }

    
    file_descriptor_array =  PCB->file_descriptor;

    strncpy((int8_t*)PCB->arg,(int8_t*)argument, 128);

    // set TSS values in PCB 

    PCB->ss0 = tss.ss0;
    PCB->esp0 = tss.esp0;

    tss.ss0 = KERNEL_DS;
    tss.esp0 = (mb_8 - ((pid) * kb_8) - 4); // will point to the top of your stack

    uint32_t esp_context = 0x8000000 + 0x400000 - 4;
    

     // store EIP value for context switching
    uint8_t eip_loc[4];
    eip_loc[0]= file[24];
    eip_loc[1]= file[25];
    eip_loc[2]= file[26];
    eip_loc[3]= file[27];

    uint32_t eip_context = *((int*)eip_loc);    

    contextSwitch(eip_context,esp_context);
    

    
  
    asm volatile("end_of_execute:");
    return 0;
    
}
/* 
 * getargs
 * Description: get args from command and copy it to buffer
 * Input:   buf -- destination buffer's pointer
 *          nbytes -- number of bytes to copy
 * Output: 0 for success, -1 for failure
 * Side Effect: copy args to buffer
 */
int32_t getargs(uint8_t* buf, int32_t nbytes){
    pcb_t* cur_pcb = pcb_adress(pid);
    //buf null check
    if(buf==NULL){
        return 0;
    }
    //nbytes null check
    if(nbytes == 0){
        return -1;
    }
    if(cur_pcb->arg[0] == '\0'){
        return -1;
    }
    //copies into buf
    strncpy((int8_t*)buf, (int8_t*)(cur_pcb->arg), nbytes);
    return 0;
}
/* 
 *  vidmap
 *  Description: maps user space virtual vidmem to physical video memory 
 *               and return virtual vidmem address to user
 *  Input:  screen_start -- a pointer points to a place where to output virtual video memory addr for user
 *  Output: 0 for success, -1 for failure, virtual vidmem address
 */
int32_t vidmap(uint8_t** screen_start)
{
    //printf("6");
    /* check if the pointer is in user space */
    if ((unsigned int)screen_start <= PHYS_MEM_TOP || (unsigned int)screen_start >= PHYS_MEM_BASE)
		return -1;

    *screen_start = (uint8_t*)start;

    /* initialize the VIDMAP page */
    PageDir[0].FourKB.Present = 1;    // present
    PageDir[0].FourKB.ReadWrite = 1;
    PageDir[0].FourKB.UserSupervisor = 1;    // user mode
    PageDir[0].FourKB.PageBaseAddr   = (unsigned int)video_page_table >> 12;


    video_page_table[0xB8].Present = 1;    
    video_page_table[0xB8].ReadWrite = 1;  
    video_page_table[0xB8].UserSupervisor = 1;
    video_page_table[0xB8].PageBaseAddr = VID_MEM >> 12;
    flush();
    return 0;
}

int32_t set_handler(int32_t signum, void* handler_address){
    return -1;
}
int32_t sigreturn(void){
    return -1;
}



/* 
 *  Scheduler
 *  Description: Schedules processes in a multi terminal setting
 *  Input:  none
 *  Output: none
 */

void scheduler(){

//checking processid of current runnig terminal if -1 means that shell isn't running yet

if(runningTerminal->pid==-1){
return;
}
  cli();
//saves current pcb adress in previous pcb adress
pcb_t* prevPCB = pcb_adress(runningTerminal->pid);

//saving current ebp and esp to return to
register uint32_t saved_ebp asm("ebp");
register uint32_t saved_esp asm("esp");

prevPCB->saved_ebp = saved_ebp;
prevPCB->saved_esp = saved_esp;

//gets id for next terminal in round robin fashion

 int next_id = set_running_terminal();

 runningTerminal = &(multi_terminal[next_id]);
 pid = runningTerminal->pid;
 pcb_t* PCB = pcb_adress(runningTerminal->pid);

//setting up video page table
if (curr_terminal_ID == next_id){

    PageDir[0].FourKB.Present = 1;    // present
    PageDir[0].FourKB.ReadWrite = 1;
    PageDir[0].FourKB.UserSupervisor = 1;    // user mode
    PageDir[0].FourKB.PageBaseAddr   = (unsigned int)video_page_table >> 12;


    video_page_table[0xB8].Present = 1;    
    video_page_table[0xB8].ReadWrite = 1;  
    video_page_table[0xB8].UserSupervisor = 1;
    video_page_table[0xB8].PageBaseAddr = VID_MEM >> 12;
    flush();

}
else{

    PageDir[0].FourKB.Present = 1;    // present
    PageDir[0].FourKB.ReadWrite = 1;
    PageDir[0].FourKB.UserSupervisor = 1;    // user mode
    PageDir[0].FourKB.PageBaseAddr   = (unsigned int)video_page_table >> 12;


    video_page_table[0xB8].Present = 1;    
    video_page_table[0xB8].ReadWrite = 1;  
    video_page_table[0xB8].UserSupervisor = 1;
    video_page_table[0xB8].PageBaseAddr = (0xB8000+(next_id+2)*(4*1024)) >> 12;
    flush();
}

//restores paging
sysCallPaging(pid);

//context switching
tss.ss0 = KERNEL_DS;
tss.esp0 = (mb_8 - ((runningTerminal->pid) * kb_8)-4); 

 pid = runningTerminal->pid;

    asm volatile ("                 \n\
        movl    %0, %%esp           \n\
        movl    %1, %%ebp           \n\
        leave          \n\
        ret          \n\
        "
        :
        : "a"(PCB->saved_esp), "b"(PCB->saved_ebp)
    );
sti();
}

/* 
 *  int set_running_terminal()
 *  Description: sets runnig terminal
 *  Input:  none
 *  returns id of next terminal
 */
int set_running_terminal()
{
  int i, next_terminal_ID;

  for(i = 0 ; i < 3; i++)
  {
    next_terminal_ID = curr_terminal_ID + 1;
    next_terminal_ID = next_terminal_ID % 3;
    if(multi_terminal[next_terminal_ID].execute_run == 1)
      break;
  }

  return next_terminal_ID;
}


