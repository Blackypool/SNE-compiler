#include "user_insert.h"


//____________________________________________________INIT_VARIA__________________________________________________________________________//
void Asm_init_varia(Arg_s)
{
    DE_BUG(leaf);

    Le_af nick_name = leaf->left->left;
    Le_af val_ue = leaf->left->right;


    //////////////GL__LO////////////////////
    int global_or_not = ast->is_global_now;
    ////////////////////////////////////////

    size_t now_par = 0;
    const char* name_of_var = nick_name->value.x;
    scope_table* param_array = NULL;

    ///////////////ADD_IN_SCOPE/////////////
    if(ast->skope_stack->size == 0)         // need new scope
    {
        /////////////////////
        param_array = (scope_table*)calloc(1, sizeof(scope_table));
        AsserT(param_array == NULL, memory_aloca, );

        param_array->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
        AsserT(param_array->all_param_s == NULL, memory_aloca, );
        /////////////////////

        param_array->all_param_s[now_par].offset_in_loca_l = 0;
    }
    else                                    // scope alredy exist
    {
        param_array = stack_pop(ast->skope_stack);
        now_par = param_array->num_params;

        if(ast->id_of_now_func > -1)         // значит мы в функции и оффсеты прописаны в инфо о функции
        {
            int id_now_func = ast->id_of_now_func;
            
            param_array->all_param_s[now_par].offset_in_loca_l = ast->all_func[id_now_func].now_num_var;
            (ast->all_func[id_now_func].now_num_var)++;
        }

        if(global_or_not == GLOBA_L)
            param_array->all_param_s[now_par].offset_in_loca_l = 0;

        if(global_or_not == LOCA_L)        // значит иф в глобале и используем глобальный счетчик, который сам обнуляется при выходе обратно в глобал
        {
            param_array->all_param_s[now_par].offset_in_loca_l = ast->num_init_in_gl_if;
            (ast->num_init_in_gl_if)++;
        }
    }

    /////////////////INFO///////////////////
    param_array->all_param_s[now_par].is_global = global_or_not;
    param_array->all_param_s[now_par].name_of_var = strdup(name_of_var);
    param_array->all_param_s[now_par].is_it_func_param = NO_ITS_NO;   // тк init != func(a,b);
    param_array->all_param_s[now_par].call_number = 0;                // не имеет отношения к параметрам

    (param_array->num_params)++;
    stack_push(ast->skope_stack, param_array);
    ////////////////////////////////////////


    ////////////////DO_PARAM////////////////
    if(global_or_not == GLOBA_L)          // => need global metka
    {
        fprintf(fp, "\n;_________INIT_%s______________\n", name_of_var);


        Asm_expression(fp, val_ue, ast, bin_f);

        fprintf(fp, "      pop rax\n");
        emit_pop_reg(ast, bin_f, RAX);


        fprintf(fp, "\n   mov [rel %s], eax  ; global param <%s> takes from label", name_of_var, name_of_var);
        emit_mov_eax_mem_offset_swap_REL(ast, bin_f, RAX, (char*)name_of_var, B_R2M);


        fprintf(fp, ";_________________________________________\n\n");

        //////////FOR_DATA/////////////
        char name_for_global[128] = {};
        snprintf(name_for_global, sizeof(name_for_global), "\n%s:\n\tdd   %.0lf\n", name_of_var, val_ue->value.num); 

        ast->section_data[ast->n_omer_real_global_for_data_sec] = strdup(name_for_global);
        AsserT(ast->section_data[ast->n_omer_real_global_for_data_sec] == NULL, memory_aloca, );

        (ast->n_omer_real_global_for_data_sec)++;
        ///////////////////////////////
    }
    else                                  // => just place in stack
    {
        fprintf(fp, "\n ;_________INIT_%s______________\n", name_of_var);

        ///////////////////////
        Asm_expression(fp, val_ue, ast, bin_f);

        fprintf(fp, "\npop rax\n");
        emit_pop_reg(ast, bin_f, RAX);

        int off_set = 8 * param_array->all_param_s[now_par].offset_in_loca_l;
        fprintf(fp, "mov [rbp - 8 - %d], rax    ; init go ta kadr\n", off_set);
        emit_mov_eax_mem_offset_swap(ast, bin_f, RAX, (-8 - off_set), B_R2M);
        ///////////////////////

        fprintf(fp, " ;_________________________________________\n\n");
    }
    ////////////////////////////////////////
}
//________________________________________________________________________________________________________________________________________//



//_____________________________________________________VARIA_USE__________________________________________________________________________//
void var_printing(Arg_s)
{
    DE_BUG(leaf);

    char* name = strdup(leaf->value.x);
    
    params_in_scope* varia_stk = search_in_scope(ast, name);
    AsserT(varia_stk == NULL, error_in_deep, );

    if(varia_stk->is_it_func_param == YES_IT_IS)
    {
        int par_ams = (int)ast->all_func[ast->id_of_now_func].amount_of_params;
        int offset = par_ams - varia_stk->call_number;

        fprintf(fp, "\n      mov eax, [rbp + 8 + %d]        ; take [%d] param = <%s> for func from stack", (8 * offset), (varia_stk->call_number + 1), name);
        fprintf(fp, "\n      push rax                       ; var printing\n");

        emit_mov_eax_mem_offset_swap(ast, bin_f, RAX, (8 + 8*offset), B_M2R);
        emit_push_reg(ast, bin_f, RAX);

        free(name);

        return ;
    }


    if(varia_stk->is_global == GLOBA_L)
    {
        fprintf(fp, "\n      mov eax, [rel %s]  ; global param <%s> takes from label", name, name);
        emit_mov_eax_mem_offset_swap_REL(ast, bin_f, RAX, name, B_M2R);
    }
    else                    // func + gl_if
    {
        fprintf(fp, "\n      mov eax, [rbp - 8 - %d]        ; local param <%s> eat from stack mem\n", (8 * varia_stk->offset_in_loca_l), name);
        emit_mov_eax_mem_offset_swap(ast, bin_f, RAX, (-8 - 8*(varia_stk->offset_in_loca_l)), B_M2R);
    }

    fprintf(fp, "\n      push rax\n");
    emit_push_reg(ast, bin_f, RAX);
    
    free(name);
}

params_in_scope* search_in_scope(ar_get, char* name_var)
{
    AsserT(ast->skope_stack->size == 0, stack_errorr, NULL);

    /////////////////////MAIN_SCOPE///////////////////
    for(ssize_t i = ast->skope_stack->size - 1; i >= 0 ; i--)
    {
        for(size_t k = 0;       (k < ast->skope_stack->stack[i]->num_params) && \
                                (ast->skope_stack->stack[i]->all_param_s[k].name_of_var != NULL);     k++)
        {
            if(strcmp(name_var, ast->skope_stack->stack[i]->all_param_s[k].name_of_var) == 0)
                return &(ast->skope_stack->stack[i]->all_param_s[k]);
        }
    }
    //////////////////////////////////////////////////

    fprintf(stderr, "\nparam %s is not found in scope\n\n", name_var);

    return NULL;
}
//________________________________________________________________________________________________________________________________________//



//_____________________________________________________CALL_FUNC__________________________________________________________________________//
void func_user_ptinting(Arg_s)
{
    DE_BUG(leaf);

    char* func_name = strdup(leaf->value.us_op);

    user_func_info* func_struct = search_user_func(ast, func_name);
    AsserT(func_struct == NULL, error_in_deep, );

    stack_push(ast->skope_stack, func_struct->argument_s_for_scope_push);

    ////////PUSH_ARGS///////////
    fprintf(fp, "\n ;_______FUNC_USE_____");

    Asm_get_args_(fp, leaf->left, ast, bin_f);
    ////////////////////////////

    stack_pop(ast->skope_stack);

    ///////////CALL/////////////
    fprintf(fp, "   call %s\n", func_name);
    emit_jmp_call(ast, bin_f, func_name, B_CALL);


    fprintf(fp, "   add rsp, %zu        ; skip push params \n", (8 * func_struct->amount_of_params));
    emit_add_rsp_num(ast, bin_f, (int)(8 * func_struct->amount_of_params));


    fprintf(fp, "   push rax            ; вернули ret in stack\n");
    emit_push_reg(ast, bin_f, RAX);

    fprintf(fp, " ;____________________\n");
    ////////////////////////////

    free(func_name);
}

user_func_info* search_user_func(ar_get, char* name_func)
{
    AsserT(ast->max_func_user_num == 0, syntax_err, NULL);

    for(size_t i = 0; i < ast->max_func_user_num && ast->all_func[i].name_of_func != NULL; i++)
        if(strcmp(name_func, ast->all_func[i].name_of_func) == 0)
            return &(ast->all_func[i]);

    fprintf(stderr, "\nfunc %s is not found\n\n", name_func);

    return NULL;
}

void Asm_get_args_(Arg_s)
{
    DE_BUG(leaf);

    if(leaf == NULL)
        return ;

    if(leaf->type == OPERAT && leaf->value.oper == ZAPYTAYA)
    {
        Asm_get_args_(fp, leaf->left, ast, bin_f);
        Asm_get_args_(fp, leaf->right, ast, bin_f);
    }

    else
        Asm_expression(fp, leaf, ast, bin_f);
}
//________________________________________________________________________________________________________________________________________//



//_____________________________________________________INIT_FUNC__________________________________________________________________________//
void user_oper_init(Arg_s)
{
    //leaf == func == operat
    //leaf->left == name of func
    //leaf->right == connect
    //connect->left->left->left == params
    //connect->right == body

    DE_BUG(leaf);

    ast->is_global_now = LOCA_L;

    Le_af name_of_func = leaf->left;
    Le_af connect = leaf->right;
    Le_af body = connect->right;


    ////////////save____func////////////
    int func_id = (int)ast->index_of_last_add_func;
    ast->all_func[func_id].name_of_func = strdup(name_of_func->value.us_op);
    (ast->index_of_last_add_func)++;

    ast->id_of_now_func = func_id;
    ast->all_func[func_id].now_num_var = 0;
    ////////////////////////////////////


    ////////////NUM_PARAM///////////////
    Le_af param = connect->left;
    int num_params_in_func = 0;
    while(param != NULL)
    {
        ++num_params_in_func;

        param = param->left;
    }
    ast->all_func[func_id].amount_of_params = (size_t)num_params_in_func;
    ////////////////////////////////////


    ////////////////INFO////////////////
    char* name_ = (char*)name_of_func->value.us_op;

    fprintf(fp, "\n\njmp %s_skip_init\n", name_); 
    emit_jmp_call(ast, bin_f, name_, B_JMP);

    fprintf(fp, "\n;===============%s===============cdecle=======\
                \n\
                \n;   Entry:      (%zu) param\
                \n;   Exit:       ret in stack\
                \n;   Expected:   nothing\
                \n;   Destr:      ax, may be cx\
                \n\
                \n;=====================================================\n\n", ast->all_func[func_id].name_of_func, ast->all_func[func_id].amount_of_params);
    
    fprintf(fp, "%s:\n;{\n", name_);  

    emit_func_init(ast, name_);
    ////////////////////////////////////


    /////////////SCOPE_UPD//////////////
    ast->all_func[func_id].argument_s_for_scope_push = func_open_space(connect, ast, num_params_in_func);
    stack_push(ast->skope_stack, ast->all_func[func_id].argument_s_for_scope_push);
    ////////////////////////////////////


    //////////////PREFIX////////////////
    fprintf(fp, "   push rbp\n");
    fprintf(fp, "   mov rbp, rsp\n");

    emit_push_reg(ast, bin_f, RBP);
    emit_mov_rax_rax(ast, bin_f, RBP, RSP);


    int count_of_init = 0;
    how_many_init(connect, &count_of_init);

    fprintf(fp, "\nsub rsp, %d\n", 8 * count_of_init);
    emit_sub_rsp_num(ast, bin_f, (8 * count_of_init));
    ////////////////////////////////////


    ///////////////BODY/////////////////
    Asm_expression(fp, body, ast, bin_f);
    // pop rbp + ret
    ////////////////////////////////////


    ////////////////END/////////////////
    fprintf(fp, "\n;}\n%s_skip_init:\n\n;_____________________________________________________\n\n", name_); 
    emit_func_init(ast, name_);
    
    stack_pop(ast->skope_stack);
    ////////////////////////////////////

    ast->id_of_now_func = -123;
    ast->is_global_now = GLOBA_L;
}

scope_table* func_open_space(Le_af leaf, ar_get, int how_much_params)
{
    // leaf == connect;      
    // all params in ->left
    // need new steck

    DE_BUG(leaf);

    ///////////////NEW_SCOPE//////////////////////
    scope_table* new_space = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(new_space == NULL, memory_aloca, NULL);

    new_space->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space->all_param_s == NULL, memory_aloca, NULL);
    ////////////////////////////////////////////////

    ///////////////ADD_NEW_IN_SC///////////////////
    Le_af cur = leaf->left;
    for(int i = 0; i < how_much_params; i++) //добавляем новые элементы инициализированные (внутри) вызова
    {
        new_space->all_param_s[i].is_global = LOCA_L;
        new_space->all_param_s[i].name_of_var = strdup(cur->value.x);
        new_space->all_param_s[i].call_number = i;
        new_space->all_param_s[i].is_it_func_param = YES_IT_IS;

        cur = cur->left;
    }
    new_space->num_params = (size_t)how_much_params;

    return new_space;
}
//________________________________________________________________________________________________________________________________________//
