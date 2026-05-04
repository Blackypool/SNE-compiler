#ifndef MAIN_COMPILATOR_H
#define MAIN_COMPILATOR_H


#define GLOBA_L 23                          // для переменных
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

#define Arg_s FILE* fp, Le_af leaf, ar_get
#define arg_s fp, leaf, ast


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
};
extern struct znaki com_transl[];
extern const size_t znaki_struct_size;


//______________________________________________________SCOPE______________________________________________________________________________//
struct params_in_scope
{
    char* name_of_var;              

    size_t is_global;   // к глобальным обращение по меткам 

    int call_number;    // номер параметра при вызове

    int is_it_func_param;   // параметр функции?

    int offset_in_loca_l;   // чтобы доставть инит параметры из стека 
};

struct scope_table
{
    size_t num_params;            // кол-во параметров в данной области

    size_t is_global;

    params_in_scope* all_param_s; // стек с массивом структур на параметры в данной области видимости
};

// размер стека = колво функций + if + while + 2
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
// в структуре функции хранить количество инитов чтобы складывать их перед сохраненным рсп

// struct table_name
// {
//     char* name;
    
//     int est_net;

//     size_t size_tabl;

//     int adress;

//     int num_param;

//     table_name* init_params;//массив указателей на перевеменные которые при вызове функции участвуют
// };

// typedef table_name* tip_lac;







// struct A_S_T
// {
//     leaf* tok_mas;
//     int pose;
//     int max_pose;

//     stk var_ble_tabl;
//     int var_num;
//     int max_num;

//     table_name* userz_func;
//     int func_num;
//     int max_func_num;
//     int init_num;

//     const char* file_name; 
    
//     int Glob_Locl;

//     int Adress_next;
// };
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

    int num_init_in_block;
};


struct user_func_info
{
    size_t amount_of_params;       

    char* name_of_func;

    int now_num_var;    // сколько на данный момент было проинициализировано переменных фнутри структуры чтобы считать оффсеты для доставания локальных переменных из стека 
    //?? mb еще чето
    // массив с использующимися параметрами в функции короче те что при вызове участвуют
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

//_______________________________________________DE_B_U_G_SS_______________________________________________________________________________//

#define DE_BUG(leaf) fprintf(stderr, "%s:%d ___ leaf = %s\n\n", __FILE__, __LINE__, translate_r(leaf->type));


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
//////////////////////////////////////////////////

#endif