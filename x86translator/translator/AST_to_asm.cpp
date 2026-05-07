#include "AST_to_asm.h"


// правило: глобальные переменные не могут быть вызовом чего-то это сразу число == плохо а как же скан како-нибудь ????

// cdecle style ???? надо бы паскаль но это тестами проверим в падлу разбираться ща

// инициализировать изначально глобальную переменную нулем а потом сисколлом вызывать скан который сам потом поменяет по глобальной метке значение данной переменной
// мб правило что можно либо глобалбными сразу константой либо только скан без выражений типо других

// убрать копи стека и просто серчить по нему всему

// сделать вызов функции по аби
// добавить выравнивание???
// почему one = 0???
// Структура с регистрами и флаг занят/не занят / номер при вызове типа за какой параметр по номеру отвечает
// gjxtve ytne call?? функция не вызывается в конце почему

// сначала забирать рет значение потом очищать стек от параметров/????

// изменить логику работы с параметрами в ифе/вайле??


//_____________________________________________________START_____________________________________________________________________________//
int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    ast->is_global_now = GLOBA_L;

    FILE* fp = fopen(ast->end_file_name, "w");
    AsserT(fp == NULL, file_errorr, file_errorr);

    fprintf(fp, "section .text\n\n\n");
    fprintf(fp, "global _start\n_start:\n\n");

    ////////////////////////////

    Asm_expression(fp, ast->root_of_ast, ast);

    ////////////////////////////

    fprintf(fp, "HLT");

    fclose(fp);

    return 1;
}

void Asm_expression(Arg_s)          // types
{
    DE_BUG(leaf);


    if(leaf == NULL)
        return ;

    if(leaf->type == Z_NAK && leaf->value.oper == ZAPYTAYA)   
        return ;
    

    switch (leaf->type)
    {
        case NUMBER:
            fprintf(fp, "\n   mov rax, %.0lf      ; push number", leaf->value.num);
            fprintf(fp, "\n   push rax\n\n");
        break;

        case VARIA:
            var_printing(arg_s);
        break;

        case OPERAT:
            operat_ptinting(arg_s);
        break;

        case USER_OPER: //alresdy in use
            func_user_ptinting(arg_s);
        break;

        case Z_NAK:
            Asm_expression(fp, leaf->left, ast);
            Asm_expression(fp, leaf->right, ast);
        break;

        default:
            fprintf(stderr, "\n Asm_expression case ruined\n\n");
        break;
    }
}
//_______________________________________________________________________________________________________________________________________//



//___________________________________________________DEFAULT_FUNC________________________________________________________________________//
static struct znaki translet_to_ASM[] = {

    {"  add", ADD_C},
    {"  imul", MUL_C},
    {"  sub", SUB_C},
    {"  idiv", DIV_C},

    {"call pow_func\n", POW_C},     // добавить через инклуд как и принтф

    {"  and", LOG_AND},     
    {"  or", LOG_OR},     

};

const char* asm_ready_commands(Le_af leaf)
{
    AsserT(leaf == NULL, give_null_ptr, NULL);
    DE_BUG(leaf);

    size_t sz_of_translete = sizeof(translet_to_ASM) / sizeof(translet_to_ASM[0]);

    for(size_t i = 0; i < sz_of_translete; i++)
        if(leaf->value.oper == (size_t)translet_to_ASM[i].e_num)
            return translet_to_ASM[i].value;

    return NULL;
}

void operat_ptinting(Arg_s)
{
    DE_BUG(leaf);

    const char* command = asm_ready_commands(leaf);
    if(command == NULL)
    {
        Asm_another(fp, leaf, ast);

        return ;
    }

    Asm_expression(fp, leaf->left, ast);
    Asm_expression(fp, leaf->right, ast);

    
    if(strcmp(command, "call pow_func\n") == 0)    // параметры для функции уже в стеке
    {
        fprintf(fp, "       pop rsi\n");     //  - степень
        fprintf(fp, "       pop rdi\n");     // - что возводим  

        fprintf(fp, "       %s\n", command);

        fprintf(fp, "       push rax\n");

        return ;
    }


    fprintf(fp, "       pop rcx\n");
    fprintf(fp, "       pop rax\n");


    if(strcmp(command, "idiv") == 0)
    {
        fprintf(fp, "       cdq\n");           // расширяем EAX → EDX:EAX
        fprintf(fp, "       idiv ecx\n");

        fprintf(fp, "       push rax\n");

        return ;
    }


    fprintf(fp, "       %s eax, ecx\n", command);

    fprintf(fp, "       push rax\n");

    return ; 
}
//________________________________________________________________________________________________________________________________________//



//___________________________________________________NOT_DEF_FUNC_________________________________________________________________________//
void Asm_another(FILE* fp, Le_af leaf, ar_get)
{
    if(leaf == NULL)
        return ;

    if(leaf->value.oper == ZAPYTAYA)
        return ;

    if(leaf->type == OPERAT)
    {
        switch(leaf->value.oper)
        {
            case TOCHKA_ZAP:
                Asm_expression(fp, leaf->left, ast);
                Asm_expression(fp, leaf->right, ast);
            break;

            case USER_OPER:     
                user_oper_init(fp, leaf, ast);
            break;

            case IF_C:
                Asm_if_cmd(fp, leaf, ast);
            break;

            case RET_C:
            {
                Asm_expression(fp, leaf->left, ast);        // чтобы можно было возвращать выражения

                if(ast->id_of_now_func != 0) // => leave from func
                    fprintf(fp, "pop rbp\n");

                fprintf(fp, "ret\n");
            }
            break;

            case WHILE_C:
                Asm_while_cmd(fp, leaf, ast);
            break;

            case ELSE_C:
                fprintf(stderr, "\nOKAK, else without if?!\n\n");
            break;

            case IN_IT:
                Asm_init_varia(arg_s);
            break;

            case RAVNO:
                embezzlement(arg_s);
            break;

        //// standart functions vizov    
            case PRINT_F: //???
                fprintf(fp, "\n\ncall printf what about params?+ align\n\n");
                Asm_expression(fp, leaf->prev->right, ast);
            break;

            case SQRT_C:  //???
                fprintf(fp, "\n\ncall sqrt// свою написать надо\n\n");
                Asm_expression(fp, leaf->prev->right, ast);
            break;

            case SCAN_C:  //???
                fprintf(fp, "\n\ncall scan\n\n");
            break;

            case EQUAL_C:
            case L_E_bigger:
            case L_bigger_R:
            case L_E_smaller:
            case L_smaller_R:
            case N_EQUAL_C:

            bear_gammy_jump_func(fp, leaf, ast);

            break;

            default:
                fprintf(stderr, "\nOPERAT poteralsa with enum == %zu \n\n", leaf->value.oper);
            break;
        }
    }
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________ASSIGNMENT__________________________________________________________________________//
void embezzlement(Arg_s)    // присвоение, хищничество
{
    DE_BUG(leaf);

    char* name = strdup(leaf->left->value.x);
    // проверяем на есть ли вообще в области видимости такой элемент и достаем информацию о нем     
    // потом юзаем асм экспрессион для правого борта и сохраняем по достанному адремсу из структуры параметра 

    params_in_scope* varia_stk = search_in_scope(ast, name);
    AsserT(varia_stk == NULL, error_in_deep, );
    ////////////////////////////////////

    Asm_expression(fp, leaf->right, ast);   // in stack value of varia

    fprintf(fp, "       pop rax\n");

    ////////////////////////////////////
    if(varia_stk->is_it_func_param == YES_IT_IS)
    {
        fprintf(fp, "       mov [rbp + 16 + %d], eax       ; place to [%d] param = <%s> for func from stack\n", (8 * varia_stk->call_number), (varia_stk->call_number + 1), name);

        free(name);

        return ;
    }


    if(varia_stk->is_global == GLOBA_L)
        fprintf(fp, "       mov dword ptr [rel %s], eax     ; place to global label param <%s>\n", name, name);
    

    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func > - 1)     // => in func use init
        fprintf(fp, "       mov [rbp - %d], eax             ; place to stack mem <%s>\n", (8 * varia_stk->offset_in_loca_l), name);


    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func < 0)     // => if/while block
    {
        scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);

        params_in_scope* name_var_stk = search_info_for_IF_stk(ast, stk_for_if_wh, (const char*)name);
        
        fprintf(fp, "       mov [rsp + %d], eax             ; place to local massive <%s>\n", (8 * name_var_stk->offset_in_loca_l), name);
    
        stack_push(ast->labels_names_for_if_while, stk_for_if_wh);
    }
    ////////////////////////////////////

    free(name);
}
//________________________________________________________________________________________________________________________________________//
