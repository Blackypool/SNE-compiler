#ifndef LOGICAL_USLOVIE_H
#define LOGICAL_USLOVIE_H

#include "../M_compile.h"


void bear_gammy_jump_func(Arg_s);

void Asm_if_cmd(Arg_s);

void Asm_while_cmd(Arg_s);

char* preparation_if_while(Arg_s);

params_in_scope* search_info_for_IF_stk(ar_get, scope_table* stk_for_if_wh, const char* name);

void how_many_init(Le_af root_of_if, int* count);


#endif