#include "AST_to_asm.h"

// инклуд надо делать в самом низу типо выставлять флаг что да вызывали и только в конце дописывать это
// двух прохадка для бинарника

// ошибка плоского стека вместе с ифом 
// если инит в елсе тогда не надо учитывать иниты ифа бляяя // точнее пушить мусор если в ифе был какой-то инит а перешли в елс
// то есть изначально создавать мусор в стеке? типо выделятьб массив?

// создавать массив на стеке равный количесву инитов и обращаться к ним через мув [] и тогда будет общий стек для ифа и елса который не перемешаетс


//_____________________________________________________START______________________________________________________________________________//
int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    FILE* fp = fopen(ast->end_file_name, "w");
    AsserT(fp == NULL, file_errorr, file_errorr);

    FILE* bin_f = fopen("first.bin", "wb");
    AsserT(bin_f == NULL, file_errorr, file_errorr);


    //////////////////START////////////////////////
    ast->is_global_now = GLOBA_L;

    elf_generator(bin_f);

    fprintf(fp, "bits 64\n");
    fprintf(fp, "section .text\n\n\n");
    fprintf(fp, "global _start\n_start:\n\n");
    ///////////////////////////////////////////////


    ///////////////////BODY////////////////////////
    Asm_expression(fp, ast->root_of_ast, ast, bin_f);
    ///////////////////////////////////////////////


    ////////////////////END////////////////////////
    fprintf(fp, "\n\n; _______________________\n;return 0;\nmov rax, 60\nmov rdi, 0\nsyscall");
    ///////////////////////////////////////////////


    ////////////////////DATA///////////////////////
    fprintf(fp, "\n\nsection .data\n");
    for(size_t i = 0; i < ast->max_varia_num; i++)
        if(ast->section_data[i] != NULL)
            fprintf(fp, "%s", ast->section_data[i]);

    fprintf(fp, "\nformat db \"%%d\", 10, 0\n");        // format for printf+scanf

    fprintf(fp, "what_prntf times 1024 db 0\n\
                align 8\n\
            Springboard:\n\
                dq not_def_spec  ; 98\n\
                dq not_def_spec  ; 99\n\
                dq d_decimal     ; 100\n");


    fprintf(fp, "\n\nsection .bss\n");
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

            emit_mov_rax_num(ast, bin_f, RAX, (int)leaf->value.num);
            emit_push_reg(ast, bin_f, RAX);
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
            Asm_expression(fp, leaf->left, ast, bin_f);
            Asm_expression(fp, leaf->right, ast, bin_f);
        break;

        default:
            fprintf(stderr, "\nAsm_expression case ruined\n\n");
        break;
    }
}
//________________________________________________________________________________________________________________________________________//



//___________________________________________________DEFAULT_FUNC_________________________________________________________________________//
static struct znaki translet_to_ASM[] = {

    {"add",     ADD_C,      NULL,      B_ADD_R},
    {"imul",    MUL_C,      NULL,      0},
    {"sub",     SUB_C,      NULL,      B_SUB_R},
    {"idiv",    DIV_C,      NULL,      B_IDIV},
    {"and",     LOG_AND,    NULL,      B_AND_R},
    {"or",      LOG_OR,     NULL,      B_OR_R},

};

int asm_ready_commands(int enum_of_need)
{
    int sz_of_translete = (int)(sizeof(translet_to_ASM) / sizeof(translet_to_ASM[0]));

    for(int i = 0; i < sz_of_translete; i++)
        if(enum_of_need == translet_to_ASM[i].e_num)
            return i;

    return -1;
}

void operat_ptinting(Arg_s)
{
    DE_BUG(leaf);

    int index_in_translet = asm_ready_commands((int)leaf->value.oper);

    if(index_in_translet == -1)
    {
        int include_com = search_num_in_jt((int)leaf->value.oper);

        if(include_com == -1)
        {
            // not default func use
            Asm_another(fp, leaf, ast, bin_f);
            return ; 
        }

        // include func:
        _include_func_(arg_s, include_com);

        return ;
    }
    const char* default_com = translet_to_ASM[index_in_translet].value;
    

    // default func use:
    Asm_expression(fp, leaf->left, ast, bin_f);
    Asm_expression(fp, leaf->right, ast, bin_f);

    fprintf(fp, "      pop rcx\n");
    fprintf(fp, "      pop rax\n");

    emit_pop_reg(ast, bin_f, RCX);
    emit_pop_reg(ast, bin_f, RAX);


    if(strcmp(default_com, "imul") == 0)
    {
        fprintf(fp, "      %s eax, ecx\n", default_com);
        fprintf(fp, "      push rax\n");

        emit_imul_eax_eax(ast, bin_f, RAX, RCX);
        emit_push_reg(ast, bin_f, RAX);

        return ;
    }


    if(strcmp(default_com, "idiv") == 0)
    {
        fprintf(fp, "      cdq\n");           // расширяем EAX => EDX:EAX
        fprintf(fp, "      idiv ecx\n");
        fprintf(fp, "      push rax\n");

        emit_idiv_eax_eax(ast, bin_f, RCX);
        emit_push_reg(ast, bin_f, RAX);

        return ;
    }


    fprintf(fp, "      %s eax, ecx\n", default_com);
    fprintf(fp, "      push rax\n");

    emit_calculate(ast, bin_f, (unsigned char)translet_to_ASM[index_in_translet].bin_code, RAX, RCX);
    emit_push_reg(ast, bin_f, RAX);

    return ; 
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________STANDART____________________________________________________________________________//
static struct include_func include[] = {

    {"call M_printf_s", PRINT_F,    NO_ITS_NO,  "standart_func/printf.asm"  },
    {"call M_Scanf",    SCAN_C,     NO_ITS_NO,  "standart_func/scanf.asm"   },
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
            Asm_expression(fp, leaf->left, ast, bin_f);

            fprintf(fp, "\n      pop rsi");
            fprintf(fp, "\n      lea rdi, [rel format]");
            fprintf(fp, "\n      xor rax, rax");

            fprintf(fp, "\n%s\n\n", include[include_com].value);
        }
        break;

        case SCAN_C:
        {
            fprintf(fp, "\n%s\n", include[include_com].value);
            fprintf(fp, "\n      push rax\n");
        }
        break;

        case SQRT_C:
        {
            Asm_expression(fp, leaf->left, ast, bin_f);
            fprintf(fp, "\n      pop rdi");

            fprintf(fp, "\n%s\n", include[include_com].value);

            fprintf(fp, "\n      push rax");
        }
        break;

        case POW_C:
        {
            Asm_expression(fp, leaf->left, ast , bin_f);    // == base
            Asm_expression(fp, leaf->right, ast, bin_f);    // == degr

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
                Asm_expression(fp, leaf->left, ast, bin_f);
                Asm_expression(fp, leaf->right, ast, bin_f);
            break;

            case USER_OPER:     
                user_oper_init(fp, leaf, ast, bin_f);
            break;

            case IF_C:
                Asm_if_cmd(fp, leaf, ast, bin_f);
            break;

            case RET_C:
            {
                Asm_expression(fp, leaf->left, ast, bin_f);
                
                if(ast->id_of_now_func > -1)
                {
                    fprintf(fp, "\n   pop rax\n");
                    emit_pop_reg(ast, bin_f, RAX);
                }

                fprintf(fp, "\n   mov rsp, rbp");
                fprintf(fp, "\n   pop rbp\n\n");
                fprintf(fp, "    ret\n");

                emit_mov_rax_rax(ast, bin_f, RSP, RBP);
                emit_pop_reg(ast, bin_f, RBP);
                emit_ret(ast, bin_f);
            }
            break;

            case WHILE_C:
                Asm_while_cmd(fp, leaf, ast, bin_f);
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

            bear_gammy_jump_func(fp, leaf, ast, bin_f);

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
    Asm_expression(fp, leaf->right, ast, bin_f);
    fprintf(fp, "      pop rax\n");
    emit_pop_reg(ast, bin_f, RAX);
    ////////////////////////////////////

    if(varia_stk->is_it_func_param == YES_IT_IS)
    {
        int par_ams = (int)ast->all_func[ast->id_of_now_func].amount_of_params;
        int offset = par_ams - varia_stk->call_number;

        fprintf(fp, "\n      mov [rbp + 8 + %d], eax        ; take [%d] param = <%s> for func from stack", (8 * offset), (varia_stk->call_number + 1), name);
        emit_mov_eax_mem_offset_swap(ast, bin_f, RAX, (8 + 8*offset), B_R2M);

        free(name);

        return ;
    }

    if(varia_stk->is_global == GLOBA_L)
    {
        fprintf(fp, "\n      mov [rel %s], eax  ; global param <%s> takes from label", name, name);
        emit_mov_eax_mem_offset_swap_REL(ast, bin_f, RAX, name, B_R2M);
    }
    else                    // func + gl_if
    {
        int off_set = 8 * varia_stk->offset_in_loca_l;
        fprintf(fp, "\n      mov [rbp - 8 - %d], eax        ; local param <%s> eat from stack mem\n", off_set, name);
        emit_mov_eax_mem_offset_swap(ast, bin_f, RAX, (-8 - off_set), B_R2M);
    }

    free(name);
}
//________________________________________________________________________________________________________________________________________//
