#ifndef USER_INSERT_HHH
#define USER_INSERT_HHH

#include "../M_compile.h"


void var_printing(Arg_s);

params_in_scope* search_in_scope(ar_get, char* name_var);

void func_user_ptinting(Arg_s);

user_func_info* search_user_func(ar_get, char* name_func);

void Asm_get_args_(Arg_s);

void user_oper_init(Arg_s);

scope_table* func_open_space(Le_af leaf, ar_get, int how_much_params);

void Asm_init_varia(Arg_s);


#endif