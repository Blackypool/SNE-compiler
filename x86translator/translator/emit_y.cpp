#include "emit_y.h"


//____________________________________________________BASE_OF_BYTES_______________________________________________________________________//
void emit_byte(ar_get, FILE* bin_f, unsigned char byte)
{
    fwrite(&byte, 1, 1, bin_f);
    (ast->cur_ip)++;
}

void emit_4_byte(ar_get, FILE* bin_f, int fore_byte)
{
    fwrite(&fore_byte, 4, 1, bin_f);
    ast->cur_ip = ast->cur_ip + 4;
}
//________________________________________________________________________________________________________________________________________//



//_____________________________________________________DEFAULT_F__________________________________________________________________________//
void emit_push_reg(ar_get, FILE* bin_f, int reg) 
{
    emit_byte(ast, bin_f, (unsigned char)(B_PUSH + reg));
    fprintf(stderr, "push %s    ; rip = %d\n", translate_regs_en(reg), ast->cur_ip);
}

void emit_pop_reg(ar_get, FILE* bin_f, int reg) 
{
    emit_byte(ast, bin_f, (unsigned char)(B_POP + reg));
    fprintf(stderr, "pop %s    ; rip = %d\n", translate_regs_en(reg), ast->cur_ip);
}

void emit_syscall(ar_get, FILE* bin_f)
{
    emit_byte(ast, bin_f, 0x0F);
    emit_byte(ast, bin_f, 0x05);

    fprintf(stderr, "syscall\n");
}
//________________________________________________________________________________________________________________________________________//



//_______________________________________________________MOV_ES___________________________________________________________________________//
void emit_mov_rax_rax(ar_get, FILE* bin_f, int reg_left_in, int reg_right_out)
{
    emit_byte(ast, bin_f, (unsigned char)B_64_BIT_DIR);
    emit_byte(ast, bin_f, (unsigned char)B_R2R);

    unsigned char regs = (unsigned char)(B_WORK_RR | (reg_right_out << 3) | reg_left_in);
    emit_byte(ast, bin_f, regs);

    fprintf(stderr, "mov %s, %s     ; rip = %d\n", translate_regs_en(reg_left_in), translate_regs_en(reg_right_out), ast->cur_ip);
}


void emit_mov_rax_num(ar_get, FILE* bin_f, int reg, int num)
{
    emit_byte(ast, bin_f, (unsigned char)B_64_BIT_DIR);
    emit_byte(ast, bin_f, (unsigned char)B_R2N);

    unsigned char regs = (unsigned char)(B_WORK_RR | (0 << 3) | reg);
    emit_byte(ast, bin_f, regs);

    emit_4_byte(ast, bin_f, num);

    fprintf(stderr, "mov %s, %d     ; rip = %d\n", translate_regs_en(reg), num, ast->cur_ip);
}


void emit_mov_eax_mem_offset_swap(ar_get, FILE* bin_f, int reg, int m_offset, int who_first) // B_R2M // B_M2R
{
    emit_byte(ast, bin_f, (unsigned char)who_first);

    //                  адресс + 8битное смещение
    unsigned char regs = (unsigned char)(B_ADR_OFS | (reg << 3) | RBP);
    emit_byte(ast, bin_f, regs);

    //                              rbp + m_offset
    emit_byte(ast, bin_f, (unsigned char)m_offset);

    fprintf(stderr, "mov %s, %d = offset     ; rip = %d\n", translate_regs_en(reg), m_offset, ast->cur_ip);
}


void emit_mov_eax_mem_offset_swap_REL(ar_get, FILE* bin_f, int reg, const char* name_of_label, int who_first) // B_R2M // B_M2R
{
    int m_offset = search_name_of_label(ast, name_of_label) - (ast->cur_ip + 6);

    emit_byte(ast, bin_f, (unsigned char)who_first);

    //              work with mem 00 != 11
    unsigned char regs = (unsigned char)(0x00 | (reg << 3) | B_FOR_EAX_REL);
    emit_byte(ast, bin_f, regs);

    emit_4_byte(ast, bin_f, m_offset);

    fprintf(stderr, "mov %s, %d = rel     ; rip = %d\n", translate_regs_en(reg), m_offset, ast->cur_ip);
}
//________________________________________________________________________________________________________________________________________//



//_______________________________________________________CALC_____________________________________________________________________________//
void emit_calculate(ar_get, FILE* bin_f, unsigned char type_of_opera, int reg_left_in, int reg_right_out)
{
    emit_byte(ast, bin_f, (unsigned char)(type_of_opera));

    unsigned char regs = (unsigned char)(B_WORK_RR | (reg_right_out << 3) | reg_left_in);
    emit_byte(ast, bin_f, regs);

    fprintf(stderr, "%s %s, %s     ; rip = %d\n", translate_regs_en(type_of_opera), translate_regs_en(reg_left_in), \
                                                           translate_regs_en(reg_right_out), ast->cur_ip);
}


void emit_imul_eax_eax(ar_get, FILE* bin_f, int reg_left_in, int reg_right_out)
{
    emit_byte(ast, bin_f, (unsigned char)B_IMUL_1);
    emit_byte(ast, bin_f, (unsigned char)B_IMUL_2);

    unsigned char regs = (unsigned char)(B_WORK_RR | (reg_left_in << 3) | reg_right_out);
    emit_byte(ast, bin_f, regs);

    fprintf(stderr, "imul %s, %s     ; rip = %d\n", translate_regs_en(reg_left_in), translate_regs_en(reg_right_out),  ast->cur_ip);
}


void emit_idiv_eax_eax(ar_get, FILE* bin_f, int reg_right_out)
{
    emit_byte(ast, bin_f, (unsigned char)B_CDQ);
    emit_byte(ast, bin_f, (unsigned char)B_FOR_DIV);

    unsigned char regs = (unsigned char)(B_WORK_RR | (B_IDIV  << 3) | reg_right_out);
    emit_byte(ast, bin_f, (unsigned char)regs);

    fprintf(stderr, "idiv %s, %s     ; rip = %d\n", translate_regs_en(RAX), translate_regs_en(reg_right_out),  ast->cur_ip);
}


void emit_add_rsp_num(ar_get, FILE* bin_f, int num)
{
    emit_byte(ast, bin_f, (unsigned char)(B_64_BIT_DIR));       // 0x48    -> 64 bit
    emit_byte(ast, bin_f, (unsigned char)(B_REG_MIN_NUM));      // 0x83    -> take reg + min_num

    // mod: 11 -> work with reg 
    // 000     -> num of opera 
    // 000     -> num of reg
    unsigned char glue = B_WORK_RR | (B_ADD_R_N << 3) | RSP;
    emit_byte(ast, bin_f, (glue));

    emit_byte(ast, bin_f, (unsigned char)(num));                // -> what add

    fprintf(stderr, "add %s, %d     ; rip = %d\n", translate_regs_en(RSP), num,  ast->cur_ip);
}


void emit_sub_rsp_num(ar_get, FILE* bin_f, int num)
{
    emit_byte(ast, bin_f, (unsigned char)(B_64_BIT_DIR));       // 0x48    -> 64 bit
    emit_byte(ast, bin_f, (unsigned char)(B_REG_MIN_NUM));      // 0x83    -> take reg + min_num

    // mod: 11 -> work with reg 
    // 000     -> num of opera 
    // 000     -> num of reg
    unsigned char glue = B_WORK_RR | (B_SUB_R_N << 3) | RSP;
    emit_byte(ast, bin_f, (glue));

    emit_byte(ast, bin_f, (unsigned char)(num));                // -> what add

    fprintf(stderr, "sub %s, %d     ; rip = %d\n", translate_regs_en(RSP), num,  ast->cur_ip);
}


void emit_xor_rax_rax(ar_get, FILE* bin_f, int reg)
{
    emit_byte(ast, bin_f, (unsigned char)(B_64_BIT_DIR));
    emit_byte(ast, bin_f, (unsigned char)(B_XOR_R));

    unsigned char glue = (unsigned char)(B_WORK_RR | (reg << 3) | reg);
    emit_byte(ast, bin_f, (glue));

    fprintf(stderr, "xor RAX, RAX     ; rip = %d\n",  ast->cur_ip);
}
//________________________________________________________________________________________________________________________________________//



//_______________________________________________________OFSET_S__________________________________________________________________________//
int search_name_of_label(ar_get, const char* name_of_label)
{
    size_t m_a_x = ast->max_func_user_num * 5;

    for(size_t i = 0; ast->labels_bin_rip[i].name_ != NULL && i < m_a_x; i++)
        if(strcmp(name_of_label, ast->labels_bin_rip[i].name_) == 0)
            return (int)ast->labels_bin_rip[i].rip;

    return -1;
}


void emit_cmp_rax_rax(ar_get, FILE* bin_f, int reg_left, int reg_right)
{
    emit_byte(ast, bin_f, (unsigned char)(B_64_BIT_DIR));
    emit_byte(ast, bin_f, (unsigned char)(B_CMP_R));

    unsigned char glue = (unsigned char)(B_WORK_RR | (reg_right << 3) | reg_left);
    emit_byte(ast, bin_f, (glue));

    fprintf(stderr, "cmp %s, %s     ; rip = %d\n", translate_regs_en(reg_left), translate_regs_en(reg_right),  ast->cur_ip);
}


void emit_cmp_eax_eax(ar_get, FILE* bin_f, int reg_left, int reg_right)
{
    emit_byte(ast, bin_f, (unsigned char)(B_CMP_R));

    unsigned char glue = (unsigned char)(B_WORK_RR | (reg_right << 3) | reg_left);
    emit_byte(ast, bin_f, (glue));

    fprintf(stderr, "cmp %s, %s     ; rip = %d\n", translate_regs_en(reg_left), translate_regs_en(reg_right),  ast->cur_ip);
}


void emit_jmp_call(ar_get, FILE* bin_f, const char* name_of_label, int type_of_jmp)
{
    int offset = search_name_of_label(ast, name_of_label) - (ast->cur_ip + 5);

    emit_byte(ast, bin_f, (unsigned char)type_of_jmp);
    emit_4_byte(ast, bin_f, offset);

    fprintf(stderr, "%s %s     ; rip = %d   // offset = %d\n", translate_regs_en(type_of_jmp), name_of_label, ast->cur_ip, offset);
}


void emit_logical_jmp(ar_get, FILE* bin_f, char* name_of_label, int type_of_jmp)
{
    int offset = search_name_of_label(ast, name_of_label) - (ast->cur_ip + 6);

    emit_byte(ast, bin_f, 0x0F);    // for long kmp
    emit_byte(ast, bin_f, (unsigned char)type_of_jmp);

    emit_4_byte(ast, bin_f, offset);

    fprintf(stderr, "%s %s     ; rip = %d\n", translate_regs_en(type_of_jmp), name_of_label, ast->cur_ip);
}


void emit_func_init(ar_get, const char* name_of_label)
{
    size_t indx = ast->free_label_for_bin_rip;
    (ast->free_label_for_bin_rip)++;

    ast->labels_bin_rip[indx].rip = (size_t)ast->cur_ip;
    ast->labels_bin_rip[indx].name_ = strdup(name_of_label);

    fprintf(stderr, "%s:     ; rip = %d\n", name_of_label, ast->cur_ip);
}


void emit_ret(ar_get, FILE* bin_f)
{
    emit_byte(ast, bin_f, (unsigned char)B_RET);
    fprintf(stderr, "RET     ; rip = %d\n", ast->cur_ip);
}

void emit_lea(ar_get, FILE* bin_f, const char* name_of_label)
{
    int offset = search_name_of_label(ast, name_of_label) - (ast->cur_ip + 7);

    emit_byte(ast, bin_f, B_64_BIT_DIR);
    emit_byte(ast, bin_f, B_LEA);

    unsigned char regs = B_MOD_LEA | (RDI << 3) | B_FOR_LEA;
    emit_byte(ast, bin_f, regs);

    emit_4_byte(ast, bin_f, offset);
}
//________________________________________________________________________________________________________________________________________//



//________________________________________________________INCLUDE_________________________________________________________________________//
void emit_include_st_f(ar_get, const char* name_of_file, FILE* bin_f)
{
    char bin_name[128] = {};
    snprintf(bin_name, sizeof(bin_name), "%s.bin",name_of_file);

    char* include_d = file__read((const char*)bin_name);

    size_t number_char = number__file(bin_name);
    fwrite(include_d, sizeof(char), number_char, bin_f);

    free(include_d);

    ast->cur_ip = ast->cur_ip + (int)number_char;
}
//________________________________________________________________________________________________________________________________________//



//________________________________________________________DE_BUG__________________________________________________________________________//
#define FIND_REG(need) {#need, need}

struct znaki regs_transl[] = {

    FIND_REG(RAX),
    FIND_REG(RCX),
    FIND_REG(RDX),
    FIND_REG(RBX),
    FIND_REG(RSP),
    FIND_REG(RBP),
    FIND_REG(RSI),
    FIND_REG(RDI),

    FIND_REG(B_ADD_R),
    FIND_REG(B_SUB_R),
    FIND_REG(B_AND_R),
    FIND_REG(B_XOR_R),
    FIND_REG(B_CMP_R),
    FIND_REG(B_OR_R),

    FIND_REG(B_JMP),
    FIND_REG(B_CALL),

    FIND_REG(B_RET),
    FIND_REG(B_JMP),
    FIND_REG(B_JE),
    FIND_REG(B_JNE),
    FIND_REG(B_JG),
    FIND_REG(B_JGE),
    FIND_REG(B_JL),
    FIND_REG(B_JLE),

};

const char* translate_regs_en(int e_n_u_m)
{
    size_t siiiize = (sizeof(regs_transl) / sizeof(regs_transl[0]));

    for(size_t i = 0; i < siiiize; i++)
        if(e_n_u_m == regs_transl[i].e_num)
            return regs_transl[i].value;

    return NULL;
}
//________________________________________________________________________________________________________________________________________//
