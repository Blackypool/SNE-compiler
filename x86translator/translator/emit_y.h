#ifndef EMIT_S_TO_BIN_H
#define EMIT_S_TO_BIN_H

#include "../M_compile.h"

#define STANDART_ADR 0x400000
#define PAGE_SIZE 4096

void elf_generator(ar_get, FILE* bin_f, int start_offset);

void emit_include_st_f(ar_get, const char* name_of_file, FILE* bin_f);

const char* translate_regs_en(int e_n_u_m);

//____________________________________________________BASE_OF_BYTES_______________________________________________________________________//
void emit_byte(ar_get, FILE* bin_f, unsigned char byte);

void emit_4_byte(ar_get, FILE* bin_f, int fore_byte);

void emit_syscall(ar_get, FILE* bin_f);
//________________________________________________________________________________________________________________________________________//


//_____________________________________________________DEFAULT_F__________________________________________________________________________//
void emit_push_reg(ar_get, FILE* bin_f, int reg);

void emit_pop_reg(ar_get, FILE* bin_f, int reg);
//________________________________________________________________________________________________________________________________________//


//_______________________________________________________MOV_ES___________________________________________________________________________//
void emit_mov_rax_rax(ar_get, FILE* bin_f, int reg_left_in, int reg_right_out);

void emit_mov_rax_num(ar_get, FILE* bin_f, int reg, int num);

void emit_mov_eax_mem_offset_swap(ar_get, FILE* bin_f, int reg, int m_offset, int who_first);

void emit_mov_eax_mem_offset_swap_REL(ar_get, FILE* bin_f, int reg, const char* name_of_label, int who_first);
//________________________________________________________________________________________________________________________________________//


//_______________________________________________________CALC_____________________________________________________________________________//
void emit_calculate(ar_get, FILE* bin_f, unsigned char type_of_opera, int reg_left_in, int reg_right_out);

void emit_imul_eax_eax(ar_get, FILE* bin_f, int reg_left_in, int reg_right_out);

void emit_idiv_eax_eax(ar_get, FILE* bin_f, int reg_right_out);

void emit_add_rsp_num(ar_get, FILE* bin_f, int num);

void emit_sub_rsp_num(ar_get, FILE* bin_f, int num);

void emit_xor_rax_rax(ar_get, FILE* bin_f, int reg);
//________________________________________________________________________________________________________________________________________//


//_______________________________________________________OFSET_S__________________________________________________________________________//
int search_name_of_label(ar_get, const char* name_of_label);

void emit_cmp_rax_rax(ar_get, FILE* bin_f, int reg_left, int reg_right);

void emit_cmp_eax_eax(ar_get, FILE* bin_f, int reg_left, int reg_right);

void emit_jmp_call(ar_get, FILE* bin_f, const char* name_of_label, int type_of_jmp);

void emit_logical_jmp(ar_get, FILE* bin_f, char* name_of_label, int type_of_jmp);

void emit_func_init(ar_get, const char* name_of_label);

void emit_ret(ar_get, FILE* bin_f);

void emit_lea(ar_get, FILE* bin_f, const char* name_of_label);
//________________________________________________________________________________________________________________________________________//


#endif