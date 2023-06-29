#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "idt.h"
#include "rtc.h"
#include "filesys.h"
#include "keyboard.h"
#include "terminal.h"
#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
// int idt_test(){
// 	TEST_HEADER;

// 	int i;
// 	int result = PASS;
// 	for (i = 0; i < 10; ++i){
// 		if ((idt[i].offset_15_00 == NULL) && 
// 			(idt[i].offset_31_16 == NULL)){
// 			assertion_failure();
// 			result = FAIL;
// 		}
// 	}

// 	return result;
// }

// // add more tests here
// /* exceptions Test
//  * 
//  * checks for division by 0 test case
//  * Inputs: None
//  * Outputs: exception/FAIL
//  * Side Effects: None

//  */

// int exceptions_test(){
// 	TEST_HEADER;
// 	/* Use exception #15 for assertions, otherwise
// 	   reserved by Intel */
// 	int i = 1 / 0;
// 	// asm volatile("int $00");
// 	return FAIL;
// }


// /* Video_Memory_Test
//  * 
//  * checks for video memory paging case
//  * Inputs: None
//  * Outputs: pass
//  * Side Effects: None

//  */


// int video_memory_test(){

// 	TEST_HEADER;

// 	char* temp_pointer = (char*)0xB8000;
// 	char final;
// 	final = *temp_pointer;


// 	return PASS;

// }

// /* Null_Test
//  * 
//  * checks for null value case
//  * Inputs: None
//  * Outputs: PAGE_FAULT/fail
//  * Side Effects: None

//  */
// int null_test(){
// 	TEST_HEADER;
//     int result;
// 	int* a = NULL;
	
// 	result = *(a);
// 	return FAIL;

// }



// int kernel_lower__fail_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0x3FFFFF;
//     char final = *temp_pointer;
//     return FAIL;
// }


// int kernel_upper__fail_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0x800000;
//     char final = *temp_pointer;
//     return FAIL;
// }


// int video_lower__fail_test(){

//     TEST_HEADER;
//     char* temp_pointer = (char*)0xB7FFF;
//     char final = *temp_pointer;
//     return FAIL;

// }


// int video_upper__fail_test(){

//     TEST_HEADER;
//     char* temp_pointer = (char*)0xB9000;
//     char final = *temp_pointer;
//     return FAIL;

// }

// void system__call_test(){
// 	TEST_HEADER;
//     // __asm__("int 0x80");
// 	printf("syscall");
// 	return FAIL;
// }


/* Checkpoint 2 tests */
// int init_dir_test(){

// uint8_t* temp_name;
// int i;
// temp_name = boot_block_A->dentry_start[8].filename;
// printf("%s",temp_name);

// 	return PASS;
// }


// int read_dentry_by_index_test(){
// dentry_t temp;
// uint8_t* temp_name;
// read_dentry_by_index(3,&temp);


// temp_name = temp.filename;
// printf("%s",temp_name);
// return PASS;
// }

// int read_dentry_by_name_test(){
// dentry_t temp;
// uint8_t* temp_test;
// uint8_t temp_name[11] = "frame1.txt";
// uint32_t val = read_dentry_by_name(temp_name,&temp);


// temp_test = temp.filename;
// if(val==-1){
// 	printf("%d",val);
// }
// else{
// printf("%s",temp_test);}
// return PASS;
// }

// int read_text_file_test(){

// uint8_t temp_name[11] = "frame0.txt";

// uint8_t buf[8000];
// uint32_t length = 8000;

// read_file(temp_name,buf,length);
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");

// printf("%s",buf);
// return PASS;

// }

// int read_large_file_test(){

// uint8_t temp_name[11] = "fish";

// uint8_t buf[8000];
// uint32_t length = 8000;

// read_file(temp_name,buf,length);
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// int i;
// for(i = 0; i<8000;i++){
// 	putc(buf[i]);
// }

// return PASS;

// }

// int read_directory_test(){



// uint8_t buf[500];
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("\n");
// printf("-------------\n");
// int val = 1;
// while(val!=0){
// val = read_directory(buf);
// printf("%s\n",buf);

// }

// return PASS;

// }

// int rtc_test() {
//     int32_t a;
//     int b;
//     int test = 0;

//     test += rtc_open(0);
//     for(a = 7; a <= 1024; a*=2) {
//         test += rtc_write(0, &a, sizeof(uint32_t));
//         for(b = 0; b < a; b++) {
//             test += rtc_read(0, 0, 0);
// 			if(test!=0){
// 				printf("rtc failed");
// 				return 0;
// 			}
//             printf("1");
//         }
// 		terminal_newline();
//     }


// 	return 1;

// }

// int rtc_close_test(){
// 	int ret = rtc_close(0);
// 	if(ret == 0){
// 		printf("rtc closed");

// 	}
// 	return 1;
// }

// int test_terminal(){
// 	char buffer[127];
// 	int r = 0, w = 0;
// 	terminal_open(0);
// 	printf("terminal driver test begins\n");
// 	while (1)
// 	{
// 		r = terminal_read(buffer, 127);
// 		if(r >= 0){
// 			w = terminal_write(buffer, 127);
// 		}
// 		if(r != w)
// 			break;
// 	}
// 	terminal_close(0);
// 	return -1;
// }


/* Checkpoint 3 tests */
// int program1_paging_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0xB8000;
//     char final = *temp_pointer;
//     return PASS;
// }
// int program2_paging_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0xB9000;
//     char final = *temp_pointer;
//     return PASS;
// }
// int program3_paging_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0xBA000;
//     char final = *temp_pointer;
//     return PASS;
// }
// int program4_paging_test(){
//     TEST_HEADER;
//     char* temp_pointer = (char*)0xBB000;
//     char final = *temp_pointer;
//     return PASS;
// }

/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */

void launch_tests(){
	//clear();
	//printf("WEHITTHIS");
	//division_by_zero_test();
	//idt_init();
	// TEST_OUTPUT("program1_paging_test()", program1_paging_test());
	// TEST_OUTPUT("program2_paging_test()", program2_paging_test());
	// TEST_OUTPUT("program3_paging_test()", program3_paging_test());
	// TEST_OUTPUT("program4_paging_test()", program4_paging_test());
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("read_text_file_test()", read_text_file_test());
	//TEST_OUTPUT("read_large_file_test()", read_large_file_test());
	//TEST_OUTPUT("read_directory_test()", read_directory_test());
	//TEST_OUTPUT("read_dentry_by_name_test()", read_dentry_by_name_test());
	//TEST_OUTPUT("read_dentry_by_index_test()", read_dentry_by_index_test());

	//TEST_OUTPUT("video_memory_test", video_memory_test());
	//TEST_OUTPUT("kernel_lower__fail_test", kernel_lower__fail_test());
	//TEST_OUTPUT("division_by_zero_test", exceptions_test());
	//TEST_OUTPUT("null_test", null_test());
	//TEST_OUTPUT("_test", rtc_test());
	//rtc_test();
	//rtc_close_test();
	//test_terminal();
	// launch your tests here
}
