#include "AST_to_asm.h"


//_____________________________________________________START______________________________________________________________________________//
int first_zapusk = 0;

static struct include_func include[] = {

    {"A_0_0_0",   PRINT_F,    NO_ITS_NO,  "standart_func/printf"  , "M_printf_s", 576},
    {"A_1_1_1",   SCAN_C,     NO_ITS_NO,  "standart_func/scanf"   , "M_Scanf"   , 89},
    {"A_2_2_2",   SQRT_C,     NO_ITS_NO,  "standart_func/sq_rt"   , "sq_rt"     , 16},
    {"A_3_3_3",   POW_C,      NO_ITS_NO,  "standart_func/pow_func", "pow_func"  , 21},
    {"A_4_4_4",   DRAW_C,     NO_ITS_NO,  "standart_func/draw_y"  , "draw_y"    , 25},

};

void init_struct_of_include()
{
    size_t size_of_include_func = sizeof(include) / sizeof(include[0]);

    for(size_t i = 0; i < size_of_include_func; i++)
    {
        char bin_name[128] = {};
        snprintf(bin_name, sizeof(bin_name), "%s.bin", include[i].name_of_file);
    
        include[i].size_of_bin_file = (int)number__file((const char*)bin_name);
    }
}


int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    struct A_S_T ast_reserve = {0};
    memcpy(&ast_reserve, ast, sizeof(*ast));


    /////////////////FILE_O////////////////////////
    FILE* fp = NULL;
    FILE* bin_f = NULL;
    open_files(ast, &fp, &bin_f);
    ///////////////////////////////////////////////


    //////////////////START////////////////////////
    prolog_nasm(ast, fp, bin_f);
    ///////////////////////////////////////////////


    ///////////////////BODY////////////////////////
    Asm_expression(fp, ast->root_of_ast, ast, bin_f);
    ///////////////////////////////////////////////


    ////////////////////END////////////////////////
    epilog_nasm(ast, fp, bin_f);
    ///////////////////////////////////////////////


    ////////////////////DATA///////////////////////
    data_nasm(ast, fp, bin_f);
    fclose(fp);
    fclose(bin_f);
    ///////////////////////////////////////////////


    ///////////////////RE_START////////////////////
    if(first_zapusk == 0)
    {
        for(size_t i = 0; i < ast->free_label_for_bin_rip; i++) // save labels for second go
        {
            ast_reserve.labels_bin_rip[i].name_ = strdup(ast->labels_bin_rip[i].name_);
            ast_reserve.labels_bin_rip[i].rip = ast->labels_bin_rip[i].rip;
        }
        ast_reserve.cur_ip = ast->cur_ip;

        renessans_for_bin(&ast_reserve);
    }
    else    // уже вторая проходка была => патчим
    {
        auto_patcher_bin(ast->end_file_name);
        chmod_bin_file(ast->end_file_name);
    }
    ///////////////////////////////////////////////


    return 1;
}


void open_files(ar_get, FILE** fp, FILE** bin_f)
{
    char end_file[150] = {};
    snprintf(end_file, sizeof(end_file), "A_skompile_result/%s.asm", ast->end_file_name);

    *fp = fopen((const char*)end_file, "w");
    AsserT(*fp == NULL, file_errorr, );

    snprintf(end_file, sizeof(end_file), "%s.bin", ast->end_file_name);
    *bin_f = fopen(end_file, "wb");
    AsserT(*bin_f == NULL, file_errorr, );
}

void prolog_nasm(ar_get, FILE* fp, FILE* bin_f)
{
    ast->is_global_now = GLOBA_L;

    int need_add_size = 0;
    size_t size_of_include_func = sizeof(include) / sizeof(include[0]); // need add in jt if use in first go + add dop size in eld
    
    for(size_t i = 0; i < size_of_include_func; i++)
        if(include[i].is_use_in_program == YES_IT_IS)
            need_add_size = need_add_size + include[i].size_of_bin_file;

    elf_generator(ast, bin_f, need_add_size);
    ast->cur_ip = 0; // чтобы он сохранился при создании эльфа

    fprintf(fp, "bits 64\n");
    fprintf(fp, "section .text\n\n\n");
    fprintf(fp, "global _start\n_start:\n\n");
    

    ///////////////////J___T///////////////////////
    fprintf(fp, "jmp skip_jmp_table\n");
    emit_jmp_call(ast, bin_f, "skip_jmp_table", B_JMP);

    for(size_t i = 0; i < size_of_include_func; i++)
    {
        char for_JT[64] = {};
        snprintf(for_JT, sizeof(for_JT), "A_%zu_%zu_%zu", i, i, i);
        emit_func_init(ast, (const char*)for_JT);

        //////////////  add later in patcher
        emit_byte(ast, bin_f, B_JMP);
        emit_4_byte(ast, bin_f, CONST_FOR_JT + (int)i);
        //////////////

        if(include[i].is_use_in_program == YES_IT_IS)   // для насма важно существавание меток
            fprintf(fp, "%s:\n   jmp %s\n", for_JT, include[i].name_for_jmp_table);

        include[i].is_use_in_program = NO_ITS_NO;
    }

    fprintf(fp, "skip_jmp_table:\n\n");
    emit_func_init(ast, "skip_jmp_table");
    ///////////////////////////////////////////////
}

void epilog_nasm(ar_get, FILE* fp, FILE* bin_f)
{
    fprintf(fp, "\n\n; _______________________\n;return 0;\nmov rax, 60\nmov rdi, 0\nsyscall");
    emit_mov_rax_num(ast, bin_f, RAX, 60);
    emit_mov_rax_num(ast, bin_f, RDI, 0);
    emit_syscall(ast, bin_f);
}

void data_nasm(ar_get, FILE* fp, FILE* bin_f)
{
    if(include[0].is_use_in_program == YES_IT_IS)       // инклудим перед датой 
            emit_include_st_f(ast, include[0].name_of_file, bin_f);

    fprintf(fp, "\n\nsection .data\n");

    for(size_t i = 0; i < ast->max_varia_num; i++)
        if(ast->section_data[i] != NULL)
        {
            fprintf(fp, "\n%s:\n\tdd   %d\n", ast->section_data[i], 0);

            emit_func_init(ast, ast->section_data[i]);
            emit_4_byte(ast, bin_f, 0);     // reserve for shr data for gl par
        }
}

void renessans_for_bin(ar_get)
{
    first_zapusk = 228;

    int err = asm_main(ast);
    AsserT(err < 0, err, );
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
    
    if(strcmp(default_com, "sub") == 0)
    {
        if(leaf->right == NULL)     // унарный минус
        {
            Asm_expression(fp, leaf->left, ast, bin_f);
            fprintf(fp, "      pop rax\n");
            emit_pop_reg(ast, bin_f, RAX);

            fprintf(fp, "      mov rcx, 0\n");
            emit_mov_rax_num(ast, bin_f, RCX, 0);

            fprintf(fp, "      sub rcx, rax\n");
            emit_calculate(ast, bin_f, B_SUB_R, RCX, RAX);

            fprintf(fp, "      push rcx\n\n");
            emit_push_reg(ast, bin_f, RCX);

            return ;
        }
    }

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
        char asm_name[128] = {};
        snprintf(asm_name, sizeof(asm_name), "%s.asm", include[include_com].name_of_file);
    
        char* include_d = file__read((const char*)asm_name);
        fprintf(fp, "%s", include_d);
        free(include_d);

        include[include_com].is_use_in_program = YES_IT_IS;

        if(include[include_com].e_num != PRINT_F)   // у всех инклуд функций кроме принтфа нету даты, поэтому принт в конце вставим чтобы не перемешивать дату и текст
            emit_include_st_f(ast, include[include_com].name_of_file, bin_f);
    }

    switch(include[include_com].e_num)
    {
        case PRINT_F:
        {
            // rdi = ptr to format
            // rsi = , num);
            Asm_expression(fp, leaf->left, ast, bin_f);


            fprintf(fp, "\n      pop rsi"); // в rdi всегда %d из формата принтфа инклуднутого
            fprintf(fp, "\n      xor rax, rax");

            emit_pop_reg(ast, bin_f, RSI);
            emit_xor_rax_rax(ast, bin_f, RAX);


            fprintf(fp, "\ncall %s   ; %s\n\n", include[include_com].value, include[include_com].name_for_jmp_table);

            emit_jmp_call(ast, bin_f, include[include_com].value, B_CALL);
        }
        break;

        case SCAN_C:
        {
            fprintf(fp, "\ncall %s   ; %s\n", include[include_com].value, include[include_com].name_for_jmp_table);
            fprintf(fp, "\n      push rax\n");

            emit_jmp_call(ast, bin_f, include[include_com].value, B_CALL);
            emit_push_reg(ast, bin_f, RAX);
        }
        break;

        case SQRT_C:
        {
            Asm_expression(fp, leaf->left, ast, bin_f);

            fprintf(fp, "\n      pop rdi");
            emit_pop_reg(ast, bin_f, RDI);


            fprintf(fp, "\ncall %s   ; %s\n", include[include_com].value, include[include_com].name_for_jmp_table);
            emit_jmp_call(ast, bin_f, include[include_com].value, B_CALL);


            fprintf(fp, "\n      push rax\n");
            emit_push_reg(ast, bin_f, RAX);
        }
        break;

        case POW_C:
        {
            Asm_expression(fp, leaf->left, ast , bin_f);    // == base
            Asm_expression(fp, leaf->right, ast, bin_f);    // == degr


            fprintf(fp, "\n      pop rsi");
            fprintf(fp, "\n      pop rdi");

            emit_pop_reg(ast, bin_f, RSI);
            emit_pop_reg(ast, bin_f, RDI);


            fprintf(fp, "\ncall %s   ; %s\n", include[include_com].value, include[include_com].name_for_jmp_table);
            emit_jmp_call(ast, bin_f, include[include_com].value, B_CALL);


            fprintf(fp, "\n      push rax\n");
            emit_push_reg(ast, bin_f, RAX);
        }
        break;

        case DRAW_C:
        {
            Asm_expression(fp, leaf->left, ast, bin_f);

            fprintf(fp, "\n      pop rdi");
            emit_pop_reg(ast, bin_f, RDI);


            fprintf(fp, "\ncall %s   ; %s\n", include[include_com].value, include[include_com].name_for_jmp_table);
            emit_jmp_call(ast, bin_f, include[include_com].value, B_CALL);
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
    
// strdup delete + free no

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
