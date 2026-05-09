#include "logical.h"

//____________________________________________________JUMP_ERS____________________________________________________________________________//
static struct znaki jumpers[] = {

    {"   je", EQUAL_C},
    {"   jne", N_EQUAL_C},

    {"   jge", L_E_bigger},
    {"   jg", L_bigger_R},

    {"   jle", L_E_smaller},
    {"   jl", L_smaller_R},
    
};

void bear_gammy_jump_func(Arg_s)
{
    DE_BUG(leaf);

    fprintf(fp, "\n ;________CMP_________");

    Asm_expression(fp, leaf->left,  ast);
    Asm_expression(fp, leaf->right, ast);

    fprintf(fp, "\n   pop rcx");
    fprintf(fp, "\n   pop rax\n\n");

    fprintf(fp, "   cmp rax, rcx\n");

    ////////////////////////////////////

    scope_table* for_label_name_stk = stack_pop(ast->labels_names_for_if_while);
    stack_push(ast->labels_names_for_if_while, for_label_name_stk);

    char* label_name = for_label_name_stk->all_param_s[0].name_of_var;

    size_t size_of_JT_table = sizeof(jumpers) / sizeof(jumpers[0]);

    for(size_t i = 0; i < size_of_JT_table; ++i)
        if(leaf->value.oper == (size_t)jumpers[i].e_num)
            fprintf(fp, "%s    %s\n", jumpers[i].value, label_name);

    fprintf(fp, " ;____________________\n");
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________IF__WHILE___________________________________________________________________________//
void Asm_if_cmd(Arg_s)
{
    DE_BUG(leaf);    

    Le_af root_of_if = leaf;
    char* name_forr_if = preparation_if_while(arg_s);       
    AsserT(name_forr_if == NULL, error_in_deep, );
    

    /////////////////CONDITION/////////////////////
    Asm_expression(fp, root_of_if->left, ast);   
    ///////////////////////////////////////////////


    /////////////////////END//////////////////////
    char end_of_if_label[120] = {};
    snprintf(end_of_if_label, sizeof(end_of_if_label), "else_end_%s", name_forr_if);
    //////////////////////////////////////////////


    ////////////////////PRINTF/////////////////////
    if(leaf->right->value.oper == ELSE_C)
    {
        /////////////////ELSE//////////////////
        fprintf(fp, " ; else:\n");

        Asm_expression(fp, leaf->right->right, ast);
        
        fprintf(fp, " jmp %s\n", end_of_if_label);
        //////////////////////////////////////


        /////////////////_IF_/////////////////
        fprintf(fp, " %s:        ; if:\n\n", name_forr_if);

        Asm_expression(fp, leaf->right->left, ast);
        
        fprintf(fp, " %s:\n", end_of_if_label);
        //////////////////////////////////////
    }
    else
    {
        /////////////////_IF_/////////////////
        fprintf(fp, " jmp %s\n", end_of_if_label);
        fprintf(fp, " %s:        ; if:\n\n", name_forr_if);

        Asm_expression(fp, leaf->right, ast);
        
        fprintf(fp, " %s:\n", end_of_if_label);
        //////////////////////////////////////
    }
    ///////////////////////////////////////////////
    

    last_word_from_if_wh(arg_s, name_forr_if);
}

void Asm_while_cmd(Arg_s)
{
    DE_BUG(leaf);    

    Le_af root_of_while = leaf;
    char* name_forr_while = preparation_if_while(arg_s);  
    AsserT(name_forr_while == NULL, error_in_deep, );    


    ///////////////////START//////////////////////
    char start_of_while_[120] = {};
    snprintf(start_of_while_, sizeof(start_of_while_), "start_of_while_%s", name_forr_while);
    fprintf(fp, " %s:\n", start_of_while_);
    ///////////////////////////////////////////////


    /////////////////CONDITION/////////////////////
    Asm_expression(fp, root_of_while->left, ast);   
    ///////////////////////////////////////////////


    /////////////////////BODY//////////////////////
    Asm_expression(fp, root_of_while->right, ast);   
    fprintf(fp, " jmp %s", start_of_while_);
    ///////////////////////////////////////////////


    /////////////////////END//////////////////////
    fprintf(fp, " %s:\n", name_forr_while);
    ///////////////////////////////////////////////


    last_word_from_if_wh(arg_s, name_forr_while);
}
//________________________________________________________________________________________________________________________________________//

char* preparation_if_while(Arg_s)
{
    DE_BUG(leaf);

    if(ast->is_global_now == GLOBA_L)       // значит мы в ифе который в глобале => id func = -1
        ast->id_of_now_func = -1;

    ///////////////NEW_SCOPE///////////////////////
    scope_table* new_space = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(new_space == NULL, memory_aloca, NULL);

    new_space->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space->all_param_s == NULL, memory_aloca, NULL);
    ///////////////////////////////////////////////


    /////////////////INITial_IF/////////////////
    scope_table* stk_for_if_wh = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(stk_for_if_wh == NULL, memory_aloca, NULL);

    stk_for_if_wh->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(stk_for_if_wh->all_param_s == NULL, memory_aloca, NULL);
    ///////////////////////////////////////////////

    
    //////////////////LABEL_NAME///////////////////
    char name_forr_if[120] = {};
    snprintf(name_forr_if, sizeof(name_forr_if), "label_%d", ast->free_label_for_if);
    (ast->free_label_for_if)++;

    stk_for_if_wh->all_param_s[0].name_of_var = strdup(name_forr_if);
    ///////////////////////////////////////////////

    /////////////////////GL_LO/////////////////////
    stk_for_if_wh->all_param_s[0].is_global = ast->is_global_now;
    ast->is_global_now = LOCA_L;
    ///////////////////////////////////////////////

    if(stk_for_if_wh->all_param_s[0].is_global == GLOBA_L)  // если это иф в глобале и не ноль инитов => мини-функция => проверить обрабатывает ли это ret
    {
        int count_of_init = 0;   
        Le_af root_of_if = leaf;      
        how_many_init(root_of_if, &count_of_init);

        if(count_of_init != 0)
            fprintf(fp, "\n\n    push rbp       ; mini if/while in global\n    mov rbp, rsp\n\n");
    }

    stack_push(ast->labels_names_for_if_while, stk_for_if_wh);

    return strdup(name_forr_if);
}
    
void last_word_from_if_wh(Arg_s, char* label_name)
{
    DE_BUG(leaf);
    
    scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);

    // debug
    fprintf(stderr, "\n|GL = %d| |LO = %d|\n\ncheck NOW localnost = %d     \ncheck PREV localnost = %d     \n\n", GLOBA_L, LOCA_L, ast->is_global_now, stk_for_if_wh->all_param_s[0].is_global);
    //

    ast->is_global_now = stk_for_if_wh->all_param_s[0].is_global;

    if(ast->is_global_now == GLOBA_L)       // возвращаем бразды правления
    {
        ast->id_of_now_func = -123;
        ast->num_init_in_gl_if = 0;

        fprintf(fp, "\n ;______________;    return reins of power to global");
    }
    fprintf(fp, "    ;______________\n\n");

    free(label_name);
}

void how_many_init(Le_af root_of_if, int* count)
{
    Le_af next_ik = root_of_if;

    if(root_of_if == NULL)
        return ;

    if(next_ik->value.oper == IN_IT)
        (*count)++;

    how_many_init(root_of_if->left, count);
    how_many_init(root_of_if->right, count);
}

//________________________________________________________________________________________________________________________________________//
