#include "filesys.h"
static uint32_t file_position = 0;



/* init_filesys
 * initializes file system 
 * 
 * returns 0
 * Inputs: None
 * 


 */
int init_filesys(){
    boot_block_A = (boot_block_t*) file_system_base;
    num_dentry = boot_block_A -> data_count;
    num_inode = boot_block_A -> inode_count;

    inode_start = (inode_t*)(boot_block_A+1);

    data_block_start = (uint8_t*)(inode_start + num_inode);
    return 0;

}


/* open
 * opens files/directories
 * 
 * returns 0
 * Inputs: None
 * 


 */
int open(){
    return 0;
}
/* close
 * closes files/directories
 * 
 * returns 0
 * Inputs: None
 * 


 */
int close(){
    return 0;

}
/* write
 * writes files/directories
 * 
 * returns 0
 * Inputs: None
 * 


 */

int write(){
    return -1;
}

/* read_dentry_by_name
 * 
 * returns dentry of file based on provided file name
 * Inputs: fname - name of file, dentry - dentry to be written to
 * Outputs: return -1 if fail, 0 if success


 */
uint32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){

int i;

uint32_t fname_length = strlen((const char*)fname);

uint8_t* temp_name;

for(i = 0; i<63;i++){

temp_name = boot_block_A->dentry_start[i].filename;

uint32_t fileSize = strlen((const char*)temp_name);



if(fileSize > 32){
    fileSize = 32;
}

if(fname_length == fileSize){
    
    if(strncmp((const char*)fname,(const char*)temp_name,32) == 0){

        read_dentry_by_index(i,dentry);

        return 0;

    }

}
}

return -1;
}

/* read_dentry_by_index
 * 
 * returns dentry of file based on provided index
 * Inputs: index - index of file, dentry - dentry to be written to
 * Outputs: return -1 if fail, 0 if success


 */
uint32_t read_dentry_by_index(uint32_t index,dentry_t* dentry){

if(index < 63){


dentry_t temp = (dentry_t)(boot_block_A->dentry_start[index]);


strncpy((char*)(dentry->filename),(const char*)(temp.filename),32);

dentry->filetype = temp.filetype;

dentry->inode_num = temp.inode_num;

return 0;
}
return -1;
}



/* read_directory
 * 
 * reads a file in the directory
 * Inputs:buf - character array where file data is written to
 * Outputs: returns 0 if end of directory was reached, number of bytes read if end of file was not reached
 * Side Effect: increment directory_file to move to next file

 */



int32_t read_directory(int32_t fd, void* buf, int32_t nbytes){

dentry_t file_entry;
uint32_t directory_file = ((file_descriptor_t*)fd)->fileoffset;
//printf("%d \n", directory_file);
int32_t err = read_dentry_by_index(directory_file,&file_entry);
//printf("%s", file_entry.filename);
if(err == -1){
    return -1;
}



if(directory_file == 17){
    return 0;
}

int i;
int j = 0;
for(i = 0;i<32;i++){
    ((uint8_t*)buf)[i] = file_entry.filename[i];
    j++;
}

((file_descriptor_t*)fd)->fileoffset+=1;

return j;

}

int32_t open_directory(const char* fname){
   
    return 0;
}
int32_t close_directory(int32_t fd){
    return 0;
}

int32_t write_directory(int32_t fd, void* buf, int32_t nbytes){
    return -1;
}











/* read_file
 * 
 * reads the given file based on the specified file name
 * Inputs: fname - name of file, buf - character array where file data is written to, length - length of the file
 * Outputs: return -1 if fail, 0 if end of file was reached, number of bytes read if end of file was not reached


 */

int32_t read_file( int32_t fd,void* buf, int32_t length){ 
file_position = 0; 

dentry_t file_entry;

read_dentry_by_name((const uint8_t *)((file_descriptor_t*)fd)->filename,&file_entry);

if(buf == NULL){
    return -1;
}



inode_t* file_inode = (inode_t*)(inode_start + file_entry.inode_num);

if(file_inode < 0){
    return -1;
}

if(((file_descriptor_t*)fd)->fileoffset >= file_inode->length){
    return 0;
}

uint32_t val = read_data((uint32_t)file_inode,((file_descriptor_t*)fd)->fileoffset,(uint8_t*)buf,(uint32_t) length);



if(val == 0){

    ((file_descriptor_t*)fd)->fileoffset = file_inode->length;
    //return 0;
    return file_inode->length;
}else if(val == -1){


    return -1;
}
else{
    ((file_descriptor_t*)fd)->fileoffset +=val;
    return val;
}

return -1;


}
/* read_data
 * 
 * reads the given file based on the specified inode
 * Inputs: inode - file inode, offset - place in file to start reading, buf - character array where file data is written to, length - length of the file
 * Outputs: return -1 if fail, 0 if end of file was reached, number of bytes read if end of file was not reached


 */


uint32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){


inode_t* node = (inode_t*)inode;
uint32_t file_length = ((inode_t*)inode)->length;
uint32_t current_block_index;
uint32_t current_block;
uint8_t* mem_offset;
uint8_t* block_start;
uint32_t original = offset;
//uint32_t end_of_file = 0;
if(buf == NULL){
    return -1;
}
int i;
for(i = 0;i<length;i++){

    if(offset== file_length){
        return offset-original ;
    }

    current_block_index = (uint32_t)(offset/4096);


    current_block = node->data_block_num[current_block_index];

   block_start = (uint8_t*) (data_block_start+4096*current_block);

    mem_offset = block_start + offset%4096;
   
    buf[i] = *mem_offset;
    
    offset++;

}


return length;

}



int32_t read_file_2( int32_t  fname,void* buf, int32_t length){ 
file_position = 0; 

dentry_t file_entry;

read_dentry_by_name((const uint8_t *)fname,&file_entry);

if(buf == NULL){
    return -1;
}



inode_t* file_inode = (inode_t*)(inode_start + file_entry.inode_num);

if(file_inode < 0){
    return -1;
}

uint32_t val = read_data((uint32_t)file_inode,file_position,(uint8_t*)buf,(uint32_t) length);


if(val == 0){

    file_position = file_inode->length;

    return 0;
}else if(val == -1){


    return -1;
}
else{
    file_position +=val;
    return val;
}

return -1;


}

