#ifndef AUTI_PATCHER_H
#define AUTI_PATCHER_H

#include "../../M_compile.h"


// g++ auto_patch.cpp -o auto_pch
// ./auto_pch facto_rial.bin

// docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./facto_rial.bin


#define NUM_NOP 5
#define MAX_CMP_SYMB 2

#define START_OF_JT 0x7D    // включая elf
#define CONST_FOR_JT 0x52


size_t number__file(const char* name_file);

void search_func(FILE* fp);

void check_nop_nop(FILE* fp, unsigned char *mid_buf, unsigned char *nop_cmp);

void auto_patcher_bin(const char* name_of_file);

void chmod_bin_file(const char* nameof_file);


#endif