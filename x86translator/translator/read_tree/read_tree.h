#ifndef CHTEC_H
#define CHTEC_H

#include "../../M_compile.h"


struct asm_dump
{   
    int enam_chik;

    const char* asm_asm;
};

struct command_s
{
    const char* value;

    size_t e_num;
};
extern struct command_s com_m_trnslte[];
extern const int com_struct_size;


char* file__read(const char* name_file);

size_t number__file(const char* name_file);

int what__opera_znak(const char* ans_que);

int what__type(const char* ans_que);

char* doing_word(char **text_rim_ptr, const char type_word);

void skip_pper(char **curent_in_txt);

Le_af new_leaf_chik(char** curent_in_txt_ptr, ar_get);

struct leaf* create_base(char** curent_in_txt_ptr, ar_get);


#endif