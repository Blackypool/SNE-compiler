#ifndef A_S_T_TO_ASM_H
#define A_S_T_TO_ASM_H

#include "../M_compile.h"

int asm_main(ar_get);

void Asm_expression(Arg_s);

params_in_scope* search_info_for_IF_stk(ar_get, scope_table* stk_for_if_wh, char* name);


#endif