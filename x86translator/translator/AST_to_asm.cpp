#include "AST_to_asm.h"


//_____________________________________________________START______________________________________________________________________________//
int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    FILE* fp = fopen(ast->end_file_name, "w");
    AsserT(fp == NULL, file_errorr, file_errorr);


    //////////////////START////////////////////////
    ast->is_global_now = GLOBA_L;

    fprintf(fp, "section .text\n\n\n");
    fprintf(fp, "global _start\n_start:\n\n");
    ///////////////////////////////////////////////


    ///////////////////BODY////////////////////////
    Asm_expression(fp, ast->root_of_ast, ast);
    ///////////////////////////////////////////////


    ////////////////////END////////////////////////
    fprintf(fp, "\n\n; _______________________\n;return 0;\nmov rax, 60\nmov rdi, 0\nsyscall");
    ///////////////////////////////////////////////


    ////////////////////DATA///////////////////////
    fprintf(fp, "\n\n.section .data\n");
    for(size_t i = 0; i < ast->max_varia_num; i++)
        if(ast->section_data[i] != NULL)
            fprintf(fp, "%s", ast->section_data[i]);

    fprintf(fp, "\nformat db \"%%d\", 10, 0\n");        // format for printf+scanf

    fprintf(fp, "\n\n.section .bss\n");
    fprintf(fp, "\n    num resd 1\n");                  // for scanf 
    ///////////////////////////////////////////////

    
    fclose(fp);

    return 1;
}

void Asm_expression(Arg_s)
{
    DE_BUG(leaf);

    if(leaf == NULL)
        return ;

    if(leaf->type == Z_NAK && leaf->value.oper == ZAPYTAYA)   
        return ;
    
    switch (leaf->type)
    {
        case NUMBER:
            fprintf(fp, "\n      mov rax, %.0lf      ; push number", leaf->value.num);
            fprintf(fp, "\n      push rax\n\n");
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
            fprintf(stderr, "\nAsm_expression case ruined\n\n");
        break;
    }
}
//________________________________________________________________________________________________________________________________________//



//___________________________________________________DEFAULT_FUNC_________________________________________________________________________//
static struct znaki translet_to_ASM[] = {

    {"add", ADD_C},
    {"imul", MUL_C},
    {"sub", SUB_C},
    {"idiv", DIV_C},
    {"and", LOG_AND},     
    {"or", LOG_OR},     

};

const char* asm_ready_commands(int enum_of_need)
{
    int sz_of_translete = (int)(sizeof(translet_to_ASM) / sizeof(translet_to_ASM[0]));

    for(int i = 0; i < sz_of_translete; i++)
        if(enum_of_need == translet_to_ASM[i].e_num)
            return translet_to_ASM[i].value;

    return NULL;
}

void operat_ptinting(Arg_s)
{
    DE_BUG(leaf);

    const char* default_com = asm_ready_commands((int)leaf->value.oper);
    if(default_com == NULL)
    {
        int include_com = search_num_in_jt((int)leaf->value.oper);

        if(include_com == -1)
        {
            // not default func use
            Asm_another(fp, leaf, ast);
            return ; 
        }

        // include func:
        _include_func_(arg_s, include_com);

        return ;
    }

    // default func use:
    Asm_expression(fp, leaf->left, ast);
    Asm_expression(fp, leaf->right, ast);


    fprintf(fp, "      pop rcx\n");
    fprintf(fp, "      pop rax\n");


    if(strcmp(default_com, "idiv") == 0)
    {
        fprintf(fp, "      cdq\n");           // расширяем EAX → EDX:EAX
        fprintf(fp, "      idiv ecx\n");

        fprintf(fp, "      push rax\n");

        return ;
    }


    fprintf(fp, "      %s eax, ecx\n", default_com);
    fprintf(fp, "      push rax\n");

    return ; 
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________STANDART____________________________________________________________________________//
static struct include_func include[] = {

    {"call M_printf_s", PRINT_F,    NO_ITS_NO,  "standart_func/printf.asm"  },
    {"call scan",       SCAN_C,     NO_ITS_NO,  "standart_func/aligned.asm" },
    {"call sq_rt",      SQRT_C,     NO_ITS_NO,  "standart_func/sq_rt.asm"   },
    {"call pow_func",   POW_C,      NO_ITS_NO,  "standart_func/pow_func.asm"},

};

int search_num_in_jt(int enum_of_need)  // возвращает номер в структуре, он же джамп и тп
{
    int num_of_include = (int)(sizeof(include) / sizeof(include[0]));

    for(int i = 0; i < num_of_include; i++)
        if(enum_of_need == include[i].e_num)
            return i;

    return -1;
}

void _include_func_(Arg_s, int include_com)
{
    if(include[include_com].is_use_in_program == NO_ITS_NO)
    {
        char* include_d = file__read(include[include_com].name_of_file);
        fprintf(fp, "%s", include_d);
        free(include_d);

        include[include_com].is_use_in_program = YES_IT_IS;
    }

    switch(include[include_com].e_num)
    {
        case PRINT_F:
        {
            // rdi = ptr to format
            // rsi = , num);
            Asm_expression(fp, leaf->left, ast);

            fprintf(fp, "\n      pop rsi");
            fprintf(fp, "\n      lea rdi, [rip + format]");
            fprintf(fp, "\n      xor rax, rax");

            fprintf(fp, "\n%s\n\n", include[include_com].value);
        }
        break;

        case SCAN_C:
        {
            // rdi = ptr to format
            // rsi = ptr where save
            fprintf(fp, "\n      lea rdi, [rip + format]\n");
            fprintf(fp, "\n      lea rsi, [rip + num]\n");

            fprintf(fp, "\n      call aligned_stack\n");

            fprintf(fp, "\n%s\n", include[include_com].value);

            fprintf(fp, "\n      mov eax, dword ptr [rip + num]");
            fprintf(fp, "\n      push rax\n");
        }
        break;

        case SQRT_C:
        {
            Asm_expression(fp, leaf->left, ast);
            fprintf(fp, "\n      pop rdi");

            fprintf(fp, "\n%s\n", include[include_com].value);

            fprintf(fp, "\n      push rax");
        }
        break;

        case POW_C:
        {
            Asm_expression(fp, leaf->left, ast );    // == base
            Asm_expression(fp, leaf->right, ast);    // == degr

            fprintf(fp, "\n      pop rsi");
            fprintf(fp, "\n      pop rdi");

            fprintf(fp, "\n%s\n", include[include_com].value);

            fprintf(fp, "\n      push rax\n");
        }
        break;
    
        default:
            fprintf(stderr, "\nenum of include is not found\n\n");
        break;
    }
}
//________________________________________________________________________________________________________________________________________//



//___________________________________________________NOT_DEF_FUNC_________________________________________________________________________//
void Asm_another(Arg_s)
{
    DE_BUG(leaf);

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
                Asm_expression(fp, leaf->left, ast);
                
                if(ast->id_of_now_func > -1)
                    fprintf(fp, "\n   pop rax\n");

                fprintf(fp, "\n   mov rsp, rbp");
                fprintf(fp, "\n   pop rbp\n\n");

                fprintf(fp, "    ret\n");
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

    params_in_scope* varia_stk = search_in_scope(ast, name);
    AsserT(varia_stk == NULL, error_in_deep, );

    ////////////////////////////////////
    Asm_expression(fp, leaf->right, ast);
    fprintf(fp, "      pop rax\n");
    ////////////////////////////////////

    if(varia_stk->is_it_func_param == YES_IT_IS)
    {
        fprintf(fp, "\n      mov [rbp + 8 + %d], eax        ; take [%d] param = <%s> for func from stack", (8 * varia_stk->call_number), (varia_stk->call_number + 1), name);

        free(name);

        return ;
    }

    if(varia_stk->is_global == GLOBA_L)
    {
        fprintf(fp, "\n      lea rcx, [rip + %s]        ; global param <%s> takes from label", name, name);
        fprintf(fp, "\n      mov dword ptr [rcx], eax\n");
    }
    else                    // func + gl_if
        fprintf(fp, "\n      mov [rbp - 8 - %d], eax        ; local param <%s> eat from stack mem\n", (8 * varia_stk->offset_in_loca_l), name);
    
    free(name);
}
//________________________________________________________________________________________________________________________________________//
