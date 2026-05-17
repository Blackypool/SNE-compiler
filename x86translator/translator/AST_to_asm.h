#ifndef A_S_T_TO_ASM_H
#define A_S_T_TO_ASM_H

#include "../M_compile.h"

void init_struct_of_include();


int asm_main(ar_get);

void epilog_nasm(ar_get, FILE* fp, FILE* bin_f);

void prolog_nasm(ar_get, FILE* fp, FILE* bin_f);

void data_nasm(ar_get, FILE* fp, FILE* bin_f);

void renessans_for_bin(ar_get);

void open_files(ar_get, FILE** fp, FILE** bin_f);



void Asm_expression(Arg_s);

int asm_ready_commands(int enum_of_need);

void operat_ptinting(Arg_s);

int search_num_in_jt(int enum_of_need);

void _include_func_(Arg_s, int include_com);

void Asm_another(Arg_s);

void embezzlement(Arg_s);


#endif