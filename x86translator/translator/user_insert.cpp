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


    ///////////////ADD_IN_SCOPE/////////////
    if(ast->skope_stack->size == 0)
    {
        /////////////////////
        scope_table* param_array = (scope_table*)calloc(1, sizeof(scope_table));
        AsserT(param_array == NULL, memory_aloca, );

        param_array->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
        AsserT(param_array->all_param_s == NULL, memory_aloca, );
        /////////////////////

        param_array->all_param_s[0].is_global = global_or_not;
        param_array->all_param_s[0].name_of_var = strdup(nick_name->value.x);
        param_array->all_param_s[0].is_it_func_param = NO_ITS_NO;   // тк init != func(a,b);
        param_array->all_param_s[0].call_number = 0;                // не имеет отношения к параметрам 

        if(ast->id_of_now_func > - 1)                               // size = 0 => это первый параметр
        {
            int id_now_func = ast->id_of_now_func;
            (ast->all_func[id_now_func].now_num_var) = 1;           // при инициализации эта штука 0 а надо 1 для первого параметра чтобы было -8 к рбп

            param_array->all_param_s[0].offset_in_loca_l = ast->all_func[id_now_func].now_num_var;

            (ast->all_func[id_now_func].now_num_var)++;

            (param_array->num_params)++;
            stack_push(ast->skope_stack, param_array);

            // пока все есть сразу обрабатываем печать
            Asm_expression(fp, val_ue, ast);

            fprintf(fp, "pop rax\n");
            fprintf(fp, "mov [rbp - %d], eax    ; local func param <%s> eat from stack mem\n", (8 * param_array->all_param_s[0].offset_in_loca_l), nick_name->value.x);

            return ;
        }
        
        param_array->all_param_s[0].offset_in_loca_l = 0;       // global && if/while == local+<0
        (param_array->num_params)++;

        stack_push(ast->skope_stack, param_array);
    }
    else
    {
        scope_table* param_array = stack_pop(ast->skope_stack);

        size_t now_par = param_array->num_params;

        param_array->all_param_s[now_par].call_number = 0;
        param_array->all_param_s[now_par].is_global = global_or_not;
        param_array->all_param_s[now_par].is_it_func_param = NO_ITS_NO;
        param_array->all_param_s[now_par].name_of_var = strdup(nick_name->value.x);

        if(ast->id_of_now_func > - 1)                    // => func => default push => asm_expr
        {
            int id_now_func = ast->id_of_now_func;
            
            param_array->all_param_s[now_par].offset_in_loca_l = ast->all_func[id_now_func].now_num_var;
            (ast->all_func[id_now_func].now_num_var)++;

            (param_array->num_params)++;
            stack_push(ast->skope_stack, param_array);

            // пока все есть сразу обрабатываем печать
            Asm_expression(fp, val_ue, ast);

            fprintf(fp, "\n       pop rax                ; init ->");
            fprintf(fp, "\n       mov [rbp - %d], eax    ; local func param <%s> eat from stack mem\n", (8 * param_array->all_param_s[now_par].offset_in_loca_l), nick_name->value.x);

            return ;
        }
        else
            param_array->all_param_s[0].offset_in_loca_l = 0;       // global && if/while == local+<0

        (param_array->num_params)++;

        stack_push(ast->skope_stack, param_array);
    }
    ////////////////////////////////////////


    ////////////////DO_PARAM////////////////
    if(global_or_not == GLOBA_L)          // => need global metka
    {
        // обработать отдельно ввод с клавы
        fprintf(fp, "\n   jmp skip_global_label_%s\n", nick_name->value.x);

        fprintf(fp, "%s:            ; init ->\n", nick_name->value.x);
        fprintf(fp, "\t.long   %.0lf\n", val_ue->value.num); 

        fprintf(fp, "   skip_global_label_%s:\n\n", nick_name->value.x);

        Asm_expression(fp, val_ue, ast);

        fprintf(fp, "       pop rax\n");
        fprintf(fp, "       mov dword ptr [rel %s], eax\n", nick_name->value.x);
    
        return ;
    }

    if(global_or_not == LOCA_L)           // => while/if => local label <(if not func + LOCA_L)> 
    {
        Asm_expression(fp, val_ue, ast);  

//работу со стеком для ифа добавит ь
        scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);
        int now_init = stk_for_if_wh->all_param_s[0].call_number;

        params_in_scope* name_var_stk = &(stk_for_if_wh->all_param_s[now_init]);

        name_var_stk->offset_in_loca_l = now_init;
        name_var_stk->name_of_var = strdup(nick_name->value.x);

        stk_for_if_wh->all_param_s[now_init] = *name_var_stk;


        fprintf(fp, "\n       pop rax                ; init ->");           // с каждым новым блоком(if + while) обнулять номер инита
        fprintf(fp, "\n       mov [rsp + %d], eax    ; go to array on stack <%s>\n", (8 * name_var_stk->offset_in_loca_l), nick_name->value.x);


        (stk_for_if_wh->all_param_s[0].call_number)++;
        (stk_for_if_wh->num_params)++;

        stack_push(ast->labels_names_for_if_while, stk_for_if_wh);
    }
    ////////////////////////////////////////

//     в самом ифе ->> выделять массив в каждом блоке с количеством переменныз использующихся ???где??? 
//         посчитать заранее колво инитов в блоке и выделять под них массив на стеке через rsp - 

// то есть оффсет для вайлов и ифов также будет 
// только с доступом через rsp
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
        fprintf(fp, "\n     mov eax, [rbp + 16 + %d]       ; take [%d] param = <%s> for func from stack", (8 * varia_stk->call_number), (varia_stk->call_number + 1), name);
        fprintf(fp, "\n     push rax                       ; var printing\n");

        free(name);

        return ;
    }


    if(varia_stk->is_global == GLOBA_L)
        fprintf(fp, "\n     mov eax, dword ptr [rel %s]     ; global param <%s> takes from label\n", name, name);
    

    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func > - 1)     // => in func use init
        fprintf(fp, "\n     mov eax, [rbp - %d]            ; local param <%s> eat from stack mem\n", (8 * varia_stk->offset_in_loca_l), name);


    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func < 0)     // => if/while block
    {
        scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);

        params_in_scope* name_var_stk = search_info_for_IF_stk(ast, stk_for_if_wh, (const char*)name);
        AsserT(name_var_stk == NULL, error_in_deep, );

        // для каждого параметра нужно хранить свое смещение по имени а в нулевой ячейке лежит имя метки и количество используемых инитов для выделения памяти на стеке

        fprintf(fp, "\n     mov eax, [rsp + %d]            ; take from local massive <%s>\n", (8 * name_var_stk->offset_in_loca_l), name);
    
        stack_push(ast->labels_names_for_if_while, stk_for_if_wh);
    }

    fprintf(fp, "\n     push rax\n");
    
    free(name);
}

params_in_scope* search_in_scope(ar_get, char* name_var)
{
    AsserT(ast->skope_stack->size == 0, stack_errorr, NULL);

    ///////////////////IF_WHILE_SCOPE//////////////////
    for(ssize_t i = ast->labels_names_for_if_while->size - 1; i >= 0 ; i--)
    {
        for(size_t k = 0;       (k < ast->labels_names_for_if_while->stack[i]->num_params) && \
                                (ast->labels_names_for_if_while->stack[i]->all_param_s[k].name_of_var != NULL);     k++)
        {
            if(strcmp(name_var, ast->labels_names_for_if_while->stack[i]->all_param_s[k].name_of_var) == 0)
                return &(ast->labels_names_for_if_while->stack[i]->all_param_s[k]);
        }
    }
    //////////////////////////////////////////////////


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

    fprintf(stderr, "param %s is not found in scope\n\n", name_var);

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
    Asm_get_args_(fp, leaf->left, ast);
    ////////////////////////////

    stack_pop(ast->skope_stack);

    fprintf(fp, "call %s\n", func_name);
    fprintf(fp, "\npop rax           ; забрали ret\n");

    fprintf(fp, "add rsp, %zu        ; skip push params \n", (8 * func_struct->amount_of_params));

    fprintf(fp, "push rax          ; вернули ret in stack\n");
}

user_func_info* search_user_func(ar_get, char* name_func)
{
    AsserT(ast->max_func_user_num == 0, syntax_err, NULL);

    for(size_t i = 0; i < ast->max_func_user_num; i++)
        if(strcmp(name_func, ast->all_func[i].name_of_func) == 0)
            return &(ast->all_func[i]);

    fprintf(stderr, "func %s is not found\n\n", name_func);

    return NULL;
}

void Asm_get_args_(Arg_s)
{
    DE_BUG(leaf);

    if(leaf == NULL)
        return ;

    if(leaf->type == OPERAT && leaf->value.oper == ZAPYTAYA)
    {
        Asm_get_args_(fp, leaf->left, ast);
        Asm_get_args_(fp, leaf->right, ast);
    }

    else
        Asm_expression(fp, leaf, ast);
}
//________________________________________________________________________________________________________________________________________//



//_____________________________________________________INIT_FUNC__________________________________________________________________________//
void user_oper_init(Arg_s)
{
    DE_BUG(leaf);

    //leaf == func == operat
    //leaf->left == name of func
    //leaf->right == connect
    //connect->left->left->left == params
    //connect->right == body

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

    ////////////обход + иниt функции
    fprintf(fp, "\n\njmp %s_skip_init\n", name_of_func->value.us_op); 

    fprintf(fp, "\n;===============%s===============cdecle=======\
                \n\
                \n;   Entry:      (%zu) param\
                \n;   Exit:       ret in stack\
                \n;   Expected:   nothing\
                \n;   Destr:      ax, may be cx\
                \n\
                \n;=====================================================\n\n", ast->all_func[func_id].name_of_func, ast->all_func[func_id].amount_of_params);
    
    fprintf(fp, "%s:\n;{\n\n", name_of_func->value.us_op);  
    ////////////

    // (
    ast->all_func[func_id].argument_s_for_scope_push = func_open_space(connect, ast, num_params_in_func);   //парамс чек
    stack_push(ast->skope_stack, ast->all_func[func_id].argument_s_for_scope_push);
    // )

    //////////////////////////////////////prefix func
    fprintf(fp, "   push rbp\n");
    fprintf(fp, "   mov rbp, rsp\n");
    //////////////////////////////////////

    //[
    Asm_expression(fp, body, ast);
    //]

    fprintf(fp, "\n;}\n%s_skip_init:\n\n;_____________________________________________________\n\n", name_of_func->value.us_op);  
    
    stack_pop(ast->skope_stack);              //after ret go out see zone          

    ////////////////////////////////////
    ast->id_of_now_func = -10;   // -1 = вне функции чтобы не путать с локальностью от ифа и вайла
}

scope_table* func_open_space(Le_af leaf, ar_get, int how_much_params)
{
    DE_BUG(leaf);

    // leaf == connect;      
    // all params in ->left
    // need new steck

    ///////////////COPY_OLD_SCOPE///////////////////
    scope_table* new_space = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(new_space == NULL, memory_aloca, NULL);

    new_space->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space->all_param_s == NULL, memory_aloca, NULL);
    /////////////////////

    //так как мы не можем инициализировать функцию в других места кроме глобальных значит перекопироваться должеон именно глобальный стек если он есть а если есть только 1 стек то он точно глобальный
    
    ///////////////ADD_NEW_IN_SC///////////////////
    Le_af cur = leaf->left;
    for(int i = 0; i < how_much_params; i++) //добавляем новые элементы инициализированные (внутри) вызова
    {
        new_space->all_param_s[i].is_global = LOCA_L;
        new_space->all_param_s[i].name_of_var = strdup(cur->value.x);
        new_space->all_param_s[i].call_number = i;
        new_space->all_param_s[i].is_it_func_param = YES_IT_IS;

        // ast->all_func[func_id].argument_s.

        cur = cur->left;
    }
    new_space->num_params = (size_t)how_much_params;

    return new_space;
}
//________________________________________________________________________________________________________________________________________//
