#include "AST_to_asm.h"

///////     AX, CX = trash
///////     ecx = take varia from memory>????
///////
///////

// при инициализации переменной проверять если стек пустой значит надо каллочить
// при заходе в скобочки то есть в иф вайл функцию создается новая область видимости которая забирается либо закрывающимися скобками вайла и ифа или при инициализации функции

int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    ast->is_global_now = GLOBA_L;

    FILE* fp = fopen(ast->end_file_name, "w");
    AsserT(fp == NULL, file_errorr, file_errorr);

    fprintf(fp, "section .text\n");
    fprintf(fp, "global _start\n_start:\n");

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
            fprintf(fp, "mov rax, %.0lf\n", leaf->value.num);
            fprintf(fp, "push rax");
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



//____________________________________________DEFAULT_FUNC_______________________________________________________________________________//
static struct znaki translet_to_ASM[] = {

    {"add", ADD_C},
    {"imul", MUL_C},
    {"sub", SUB_C},
    {"idiv", DIV_C},

    {"call pow_func\n", POW_C},     // добавить через инклуд как и принтф

    {"and", LOG_AND},     
    {"or", LOG_OR},     
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
        fprintf(fp, "pop rsi\n");     //  - степень
        fprintf(fp, "pop rdi\n");     // - что возводим  

        fprintf(fp, "%s\n", command);

        fprintf(fp, "push rax\n");

        return ;
    }


    fprintf(fp, "pop rcx\n");
    fprintf(fp, "pop rax\n");


    if(strcmp(command, "idiv") == 0)
    {
        fprintf(fp, "cdq\n");           // расширяем EAX → EDX:EAX
        fprintf(fp, "idiv ecx\n");

        fprintf(fp, "push rax\n");

        return ;
    }


    fprintf(fp, "%s eax, ecx\n");

    fprintf(fp, "push rax\n");

    return ; 
}
//________________________________________________________________________________________________________________________________________//


//___________________________________________NOT_DEF_FUNC_________________________________________________________________________________//
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
                Asm_ass_ignment(arg_s);
            break;

            case LOG_AND:
                Asm_logical_and(arg_s);
            break;

            case LOG_OR:
                Asm_logical_or(arg_s);
            break;

            case PRINT_F:
                Asm_expression(fp, leaf->left, ast);
                fprintf(fp, "OUT\n");
            break;

            case SQRT_C:
                Asm_expression(fp, leaf->left, ast);
                fprintf(fp, "SQRT\n");
            break;

            case SCAN_C:
                fprintf(fp, "INT\n");
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

