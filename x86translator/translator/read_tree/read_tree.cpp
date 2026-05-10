#include "read_tree.h"


struct leaf* create_base(char** curent_in_txt_ptr, ar_get)
{
    char* curent_in_txt = *curent_in_txt_ptr;

    if(*curent_in_txt == '(')
    {
        curent_in_txt++;

        struct leaf* new_node = new_leaf_chik(&curent_in_txt, ast);
        AsserT(new_node == NULL, error_in_deep, NULL);

        new_node->left = create_base(&curent_in_txt, ast);
        if(new_node->left != NULL)
            new_node->left->prev = new_node;


        new_node->right = create_base(&curent_in_txt, ast);
        if(new_node->right != NULL)
            new_node->right->prev = new_node;


        skip_pper(&curent_in_txt);
        if(*curent_in_txt == ')')
            curent_in_txt++;

        *curent_in_txt_ptr = curent_in_txt;

        return new_node;
    }

    if(strncmp("nil", curent_in_txt, 3) == 0) 
    {
        curent_in_txt += 3;
        skip_pper(&curent_in_txt);

        while(*curent_in_txt == ')')
            curent_in_txt++;
            
        *curent_in_txt_ptr = curent_in_txt;

        return NULL;
    }

    return NULL;
}


Le_af new_leaf_chik(char** curent_in_txt_ptr, ar_get)
{
    char* cur = *curent_in_txt_ptr;

    Le_af leaf_ch = (leaf*)calloc(1, sizeof(leaf));
    AsserT(leaf_ch == NULL, memory_aloca, NULL);

    ////////////////what type is it
    const char* promeg_type = doing_word(&cur, ' ');
    int type_ti = what__type(promeg_type);
    
    if(type_ti < 0)
    {
        free((void*)promeg_type);
        free(leaf_ch);

        return NULL;
    }

    leaf_ch->type = (size_t)type_ti;
    ////////////////

    skip_pper(&cur);
    if(*cur != '"')
    {
        free((void*)promeg_type);
        free(leaf_ch);

        fprintf(stderr, "\n  where is otkrivauschie kavichki?!  \n\n");
        return NULL;
    }

    cur++; //skip first kavichki
    const char* promeg_value = doing_word(&cur, '"'); //прочитали слово в кавычках и пропустили ихх уже внутри
    if(promeg_value == NULL)
    {
        free((void*)promeg_type);
        free(leaf_ch);

        return NULL;
    }

    ////////////////
    switch(type_ti)
    {
        case VARIA:
            (ast->max_varia_num)++;
            leaf_ch->value.x = strdup(promeg_value);
            free((void*)promeg_value);
        break;

        case USER_OPER:
            (ast->max_func_user_num)++;
            leaf_ch->value.us_op = strdup(promeg_value);
            free((void*)promeg_value);
        break;

        case NUMBER:
            leaf_ch->value.num = atoi(promeg_value);
            free((void*)promeg_value);
        break;

        default: //плохой тайп уже отселяли значит это либо OPERAT _ Z_NAK
            leaf_ch->value.oper = (size_t)what__opera_znak(promeg_value);

            if(leaf_ch->value.oper == WHILE_C || leaf_ch->value.oper == IF_C)
                (ast->max_func_user_num)++;         // для стека области видимости || размер посчитать

            free((void*)promeg_value);
            if(leaf_ch->value.oper == 999 || leaf_ch->value.oper == 777)
            {
                free((void*)promeg_type);
                free(leaf_ch);

                return NULL;
            }

        break;
    }
    ////////////////

    skip_pper(&cur);

    *curent_in_txt_ptr = cur;

    free((void*)promeg_type);

    return leaf_ch;
}


void skip_pper(char **curent_in_txt)
{
    while(**curent_in_txt == ' ' || **curent_in_txt == '\t' || **curent_in_txt == '\n')
            (*curent_in_txt)++;
}


char* doing_word(char **text_rim_ptr, const char type_word)
{
    char* text_rim = *text_rim_ptr;

    skip_pper(&text_rim);

    if(*text_rim == '\0')
        return NULL;

    char* start = text_rim;
    
    while(*text_rim != '\0' && *text_rim != type_word && *text_rim != '\n')
            (text_rim)++;


    size_t len = (size_t)(text_rim - start);

    char* ans_que = (char*)calloc(len + 1, sizeof(char));
    AsserT(ans_que == NULL, memory_aloca, NULL);

    strncpy(ans_que, start, len);

    ans_que[len] = '\0';

    if(*text_rim == type_word)
        text_rim++;

    *text_rim_ptr = text_rim;

    return ans_que;
}


static struct command_s type_transl[] = {
    
    {"cnct:", Z_NAK},
    {"func:", USER_OPER},
    {"op:",   OPERAT},
    {"var:", VARIA},
    {"num:", NUMBER},
    {"unluck", NO_ARGS},
    {"init:", IN_IT},

};

int what__type(const char* ans_que)
{
    size_t size_of_type_transl = (int)(sizeof(type_transl) / sizeof(type_transl[0]));

    for(size_t i = 0; i < size_of_type_transl; ++i)
        if(strcmp(ans_que, type_transl[i].value) == 0)
            return (int)type_transl[i].e_num;

    return -1;
}


struct znaki com_transl[] = {

    {"(", SKOB_krug_L},
    {")", SKOB_krug_R},
    {"{", SKOB_figu_L},
    {"}", SKOB_figu_R},
    {";", TOCHKA_ZAP},
    {"==", EQUAL_C},
    {"=", RAVNO},
    {",", ZAPYTAYA},
    

    {">=", L_E_bigger},
    {">", L_bigger_R},

    {"<=", L_E_smaller},
    {"<", L_smaller_R},
    
    {"!=", N_EQUAL_C},
    {"&&", LOG_AND},
    {"||", LOG_OR},

    {"+", ADD_C},
    {"^", POW_C},
    {"/", DIV_C},
    {"if", IF_C},
    {"return", RET_C},
    {"*", MUL_C},
    {"while", WHILE_C},
    {"-", SUB_C},
    {"else", ELSE_C},
    {"init:", IN_IT},
    {"init", IN_IT},
    {"func", USER_OPER},
    {"print", PRINT_F},
    {"cnct", CONNEC_T},
    {"in", SCAN_C},
    {"sqrt", SQRT_C},

    {"unluck", -1},

};
const size_t znaki_struct_size = sizeof(com_transl) / sizeof(com_transl[0]);


int what__opera_znak(const char* ans_que)
{
    for(size_t i = 0; i < znaki_struct_size; ++i)
        if(strcmp(ans_que, com_transl[i].value) == 0)
            return (int)com_transl[i].e_num;

    return -1;
}


/////////////////////defolt_chitalka
size_t number__file(const char* name_file)
{
    struct stat file_info = {};

    if(stat(name_file, &file_info) != 0)
    {
        perror("stat error");
        return 1;
    }

    return (size_t)file_info.st_size;
}


char* file__read(const char* name_file)
{
    size_t number_char = number__file(name_file);

    FILE *fp = fopen(name_file, "rb");
    AsserT(fp == NULL, file_errorr, NULL);

    char *ptr = (char*)calloc((number_char + 2), sizeof(char));
    AsserT(ptr == NULL, memory_aloca, NULL);

    size_t count = fread(ptr, sizeof(char), number_char, fp);
    AsserT(count == 0, file_errorr, NULL);

    fclose (fp);

    return ptr;
}
