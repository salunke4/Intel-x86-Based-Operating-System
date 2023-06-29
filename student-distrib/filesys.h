#ifndef _FILESYS_H
#define _FILESYS_H



#include "types.h"
#include "lib.h"



//struct for dentry
typedef struct dentry{
    uint8_t filename[32];
    uint32_t filetype;
    uint32_t inode_num;
    uint8_t reserved[24];
}dentry_t;

//struct for booot_block
typedef struct boot_block {
uint32_t dir_count;
uint32_t inode_count;
uint32_t data_count;
uint8_t reserved[52];
dentry_t dentry_start[63];


}boot_block_t;

//struct for inode
typedef struct inode {
    uint32_t length;
    uint32_t data_block_num[1023];
}inode_t;

//variable for base address defines as an extern
extern uint32_t file_system_base;

//initialization and declaration of variables
boot_block_t* boot_block_A;
uint32_t num_dentry;
uint32_t num_inode;
uint32_t num_data_blocks;
inode_t* inode_start;
uint8_t*  data_block_start;

typedef struct fops_table_t {
   int32_t (*sys_read)(int32_t fd, void* buf, int32_t nbytes);
   int32_t (*sys_write)(int32_t fd, void* buf, int32_t nbytes);
   int32_t (*sys_open)(const char* fname);
  int32_t (*sys_close)(int32_t fd);
} fops_table_t;

typedef struct file_descriptor_t {
    fops_table_t* op;    // file operator table 
    uint32_t inode;     // inode index 
    uint32_t fileoffset;   // offset in current file 
    uint32_t flags;    
    uint32_t offset;   
    uint32_t filetype; 
    uint8_t filename[32];
} file_descriptor_t;

//calling functions
int init_filesys();
int open();
int close();
uint32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
uint32_t read_dentry_by_index(uint32_t index,dentry_t* dentry);
uint32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
int write();
int32_t read_file(int32_t fd,void* buf, int32_t length);
int32_t read_directory(int32_t fd, void* buf, int32_t nbytes);
int32_t open_directory(const char* fname);
int32_t close_directory(int32_t fd);
int32_t write_directory(int32_t fd, void* buf, int32_t nbytes);
int32_t read_file_2(int32_t  fname,void* buf, int32_t length);

#endif
