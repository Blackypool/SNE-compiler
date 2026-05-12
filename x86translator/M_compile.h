#ifndef MAIN_COMPILATOR_H
#define MAIN_COMPILATOR_H


#define GLOBA_L 23
#define LOCA_L  419

#define YES_IT_IS 10        // параметр функции?
#define NO_ITS_NO 88


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>


#define Le_af struct leaf*
#define ar_get struct A_S_T* ast

#define Arg_s FILE* fp, Le_af leaf, ar_get, FILE* bin_f
#define arg_s fp, leaf, ast, bin_f


typedef double typ_of_x;
union un_ited
{
    const char* x;

    size_t oper;

    typ_of_x num;

    const char* us_op;
};

struct leaf
{
    size_t type;
    union un_ited value;

    struct leaf* prev;
    struct leaf* left;
    struct leaf* right;
};

struct znaki
{
    const char* value;

    int e_num;

    const char* for_dump;

    int bin_code;
};
extern struct znaki com_transl[];
extern const size_t znaki_struct_size;

struct include_func
{
    const char* value;

    int e_num;

    int is_use_in_program;

    const char* name_of_file;
};

struct for_bin_w_adr
{
    char* name_;

    size_t rip;
};

//______________________________________________________SCOPE______________________________________________________________________________//
struct params_in_scope
{
    char* name_of_var;      // имена переменных                         // название меток в 0 ячейке в остальном названия инитов внутри ифа        

    int is_global;          // к глобальным обращение по меткам         // сохраняем чем было глобал/локал перед вызовом ифа в [0]

    int call_number;        // номер параметра при вызове               

    int is_it_func_param;   // параметр функции?                      

    int offset_in_loca_l;   // чтобы доставть инит параметры из стека
};

struct scope_table
{
    size_t num_params;            // кол-во параметров в данной области

    size_t is_global;

    params_in_scope* all_param_s; // стек с массивом структур на параметры в данной области видимости
};

typedef scope_table* tip_lac;
struct stk
{
    int Kana_ = 0;

    tip_lac* stack = 0;
    ssize_t size = 0;
    ssize_t capacity = 0; // = summ of all varia in program need save in main AST struct and calculate in read

    int _reikA = 0;
};
//________________________________________________________________________________________________________________________________________//



struct user_func_info
{
    size_t amount_of_params;       // колво параметров

    char* name_of_func;

                        // при заходе в функцию все переменные подчиняются нау_нам_вар абсолютно все / при ините в функции выдавать именно этот параметр 
    int now_num_var;    // сколько на данный момент было проинициализировано переменных фнутри структуры чтобы считать оффсеты для доставания локальных переменных из стека 
    
    scope_table* argument_s_for_scope_push;
};

struct A_S_T
{
    const char* end_file_name;      

    Le_af root_of_ast;             

    size_t max_varia_num;          

    size_t max_func_user_num;      

    user_func_info* all_func;     
    
    size_t index_of_last_add_func;

    stk* skope_stack;    
    
    int id_of_now_func;     // в какой функции сейчас для сохранения оффсетов для use <now_num_var>     + need for ret + need for now in local or global?

    int is_global_now;

    stk* labels_names_for_if_while;         // стек имен меток для рекурсивного вызова ифа и вайла 
    
    int free_label_for_if;                  // номер свободной метки для ифа

    int num_init_in_gl_if;                  // номер инита в ифе который в глобале, обнуляется при выходе оттуда
    
    char** section_data;                    // для сохранения глобальных переменных в дате а не посередине тиэксти
    int n_omer_real_global_for_data_sec;    // для хранения строк для печати

    int cur_ip;                          // адрес в бинарнике // возможно потом надо учесть в нем эпиграф бинарника
    for_bin_w_adr* labels_bin_rip;
    size_t free_label_for_bin_rip;
};



//_________________________________________________ENUM_ES__________________________________________________________________________________//
enum type_t
{
    OPERAT = 1223,

    VARIA = 1,

    NUMBER = 0,

    END_OF_FILE = 333,

    USER_OPER = 888,

    Z_NAK = 999,

    CALL_F = 1111,
    
    NO_ARGS = 287,

    CONECTION = 777,
};

enum ALL_OPer 
{

    ADD_C  = 1,
    SUB_C  = 2,
    MUL_C  = 3,   
    DIV_C  = 4,    
    POW_C  = 5,   
    SQRT_C = 6,

    PRINT_F  = 7,
    CONNEC_T = 8,
    FUNC_C   = 9,
    
    
    L_smaller_R   = 10, 
    L_E_smaller   = 11,  
    L_bigger_R    = 12, 
    L_E_bigger    = 13, 
    EQUAL_C       = 14, 
    N_EQUAL_C     = 15,  
    

    LOG_AND = 16,   
    LOG_OR  = 17,    
    
  
    IF_C    = 18,   
    ELSE_C  = 19,    
    WHILE_C = 20,   
    
    OUT_C   = 21,    
    SCAN_C  = 22,    
    DRAW_C  = 23,    
    
   
    PUSHM_C = 24,  
    RET_C   = 25,   
    HLT_C   = 26,  
    
   
    SKOB_krug_L = 27, 
    SKOB_krug_R = 28, 
    SKOB_figu_L = 29, 
    SKOB_figu_R = 30,  
    ZAPYTAYA    = 31, 
    TOCHKA_ZAP  = 32,  
    
    
    RAVNO = 33,     
    IN_IT = 34,
};

enum errors_
{
    memory_aloca = 1,

    oversize_any = 2,
    
    syntax_err   = 3,

    give_null_ptr = 4,
    
    file_errorr = 5,

    error_in_deep = 6,

    stack_errorr = 7,
};

enum regi_sters
{
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
};

enum bin_code
{
    //__DIRECTIVES___//
    B_64_BIT_DIR  = 0x48,
    B_WORK_RR     = 0xC0,
    B_NOP_NOP     = 0x90,
    B_ADR_OFS     = 0x40,
    B_MOD_LEA     = 0x00,
    //_______________//

    //____OPCODES____//
    B_REG_MIN_NUM = 0x83,
    B_FOR_DIV     = 0xF7,
    B_FOR_LEA     = 0x05,
    //_______________//


    //______DEF______//
    B_PUSH = 0x50,
    B_POP  = 0x58,
    B_CDQ  = 0x99,
    B_LEA  = 0x8D,
    //_______________//


    //______MATH_____// with regs
    B_ADD_R  = 0x01,
    B_SUB_R  = 0x29,
    B_AND_R  = 0x21,
    B_XOR_R  = 0x31,
    B_CMP_R  = 0x39,
    B_OR_R   = 0x09,

    B_IMUL_1 = 0x0F,
    B_IMUL_2 = 0xAF,

    B_IDIV   = 0b111,
    //_______________//


    //______MATH_____// with nums
    B_ADD_R_N = 0x00,
    B_SUB_R_N = 0x05,
    //_______________//
    
    
    //______JMP______//
    B_CALL = 0xE8,
    B_RET  = 0xC3,
    B_JMP  = 0xEB,

    B_JE   = 0x84,
    B_JNE  = 0x85,
    B_JG   = 0x8F,
    B_JGE  = 0x8D,
    B_JL   = 0x8C,
    B_JLE  = 0x8E,
    //_______________//

    //______MOV______//
    B_R2R = 0x89,
    B_R2N = 0xC7,   // for rax

    B_M2R = 0x8B,
    B_R2M = 0x89,

    B_FOR_EAX_REL = 0x05,
    //_______________//
};


//_______________________________________________DE_B_U_G_SS_______________________________________________________________________________//

#define DE_BUG(leaf) \
            // if(leaf != NULL) \
            //     fprintf(stderr, "%s:%d ___ leaf = %s (%zu)\n\n", __FILE__, __LINE__, translate_r(leaf->value.oper), leaf->type);


#define AsserT(what_need, type_err, retern)                                                   \
            if(what_need) [[unlikely]]                                                         \
            {                                                                                   \
                fprintf(stderr, "\n\nerror in line, %s:%d. Problem with:", __FILE__, __LINE__);  \
                switch(type_err)                                                                  \
                {                                                                                  \
                    case memory_aloca:                                                              \
                        fprintf(stderr, " memory allocation \n");                                    \
                    break;                                                                            \
                                                                                                       \
                    case oversize_any:                                                                  \
                        fprintf(stderr, " oversize \n");                                                 \
                    break;                                                                                \
                                                                                                           \
                    case syntax_err:                                                                        \
                        fprintf(stderr, " some syntax error \n");                                            \
                    break;                                                                                    \
                                                                                                               \
                    case give_null_ptr:                                                                         \
                        fprintf(stderr, " you give here NULL ptr \n");                                           \
                    break;                                                                                        \
                                                                                                                   \
                    case file_errorr:                                                                               \
                        fprintf(stderr, " problem with openning file \n");                                           \
                    break;                                                                                            \
                                                                                                                       \
                    case error_in_deep:                                                                                 \
                        fprintf(stderr, " error in return of upper func \n");                                            \
                    break;                                                                                                \
                                                                                                                           \
                    case stack_errorr:                                                                                      \
                        fprintf(stderr, "some problem with stack (mb size)\n");                                              \
                    break;                                                                                                    \
                                                                                                                               \
                    default:                                                                                                    \
                        fprintf(stderr, " undefined error \n");                                                                  \
                    break;                                                                                                        \
                }                                                                                                                  \
                                                                                                                                    \
                return retern;                                                                                                       \
            }                                                                                                                     
//_________________________________________________________________________________________________________________________________________//

//___________________________________________________HEADERS_______________________________________________________________________________//

///////////////////___READER___///////////////////
#include "translator/read_tree/dump_gr.h"
#include "translator/read_tree/read_tree.h"
//////////////////////////////////////////////////


///////////////////___STACK___////////////////////
#include "translator/stack_func/colores.h"
#include "translator/stack_func/constructor.h"
#include "translator/stack_func/prAverti.h"
#include "translator/stack_func/push_pop.h"
//////////////////////////////////////////////////

///////////////////___MAIN___/////////////////////
#include "translator/do_and_crash.h"
#include "translator/AST_to_asm.h"
#include "translator/user_insert.h"
#include "translator/logical.h"
#include "translator/emit_y.h"
//////////////////////////////////////////////////

#endif