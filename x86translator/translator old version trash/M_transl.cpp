#include "../M_Diff.h"

int pre_order_print(Le_af root, ar_get);

void dump_tree(FILE* fp, Le_af leaf);

const char* what__printf(Le_af leaf);

void update_ast(A_S_T* ast);


int main(int argc, char* argv[])
{
    char name_of_skompilenni[120] = {};
    snprintf(name_of_skompilenni, sizeof(name_of_skompilenni), "%s.txt", argv[1]);

    char* tree_txt = file__read(name_of_skompilenni);  //прочитали файл

    Le_af root = create_base(&tree_txt);  //создали дерево по файлу

    char tree_asm_name[150] = {};
    snprintf(tree_asm_name, sizeof(tree_asm_name), "../procc_essor/z_asm_for_start/%s.asm", argv[1]);

    struct A_S_T ast = {.file_name = tree_asm_name};
    // int err = pre_order_print(root, &ast);
    // AsserT(err < 0, -1);
    update_ast(&ast);

    /////////////////
    FILE* fp = fopen(tree_asm_name, "w");
    AsserT(fp == NULL, -1);

    int err = asm_main(fp, root, &ast);
    AsserT(err < 0, -1);

    fclose(fp);
    clear_stk(&ast.var_ble_tabl);
    /////////////////


    return 0;
}


void update_ast(A_S_T* ast)
{
    ast->Adress_next = 0;
    ast->func_num = 0;
    ast->max_func_num = 100;
    ast->max_num = 100;
    ast->userz_func = (table_name*)calloc((size_t)ast->max_func_num, sizeof(table_name));
    AsserT(ast->userz_func == NULL, );

    lego_stack(&ast->var_ble_tabl, (ssize_t)ast->max_num);

    ast->var_num = 0;
}


int pre_order_print(Le_af root, ar_get)
{
    FILE* fp = fopen(ast->file_name, "w");
    AsserT(fp == NULL, -1);

    dump_tree(fp, root);

    fclose(fp);

    return 0;
}


void dump_tree(FILE* fp, Le_af leaf)
{
    if(leaf == NULL)
    {
        fprintf(fp, " nil");
        return ;
    }

    fprintf(fp, "(");

    switch(leaf->type)
    {

        case OPERAT:
            fprintf(fp, "op: \"%s\"", what__printf(leaf));
        break;

        case VARIA:
            fprintf(fp, "var: \"%s\"", leaf->value.x);
        break;

        case Z_NAK:
            fprintf(fp, "cnct: \"cnct\"");
        break;

        case USER_OPER:
            fprintf(fp, "func: \"%s\"", leaf->value.us_op);
        break;

        case NUMBER:
            fprintf(fp, "num: \"%.0lf\"", leaf->value.num);
        break;

        default: 
            fprintf(fp, " unluck");
        break;

    }

    dump_tree(fp, leaf->left );
    dump_tree(fp, leaf->right);

    fprintf(fp, ")");
}


struct znaki translit[] = {

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
    {"func", USER_OPER},
    {"init", IN_IT},
    {"print", PRINT_F},

    {"unluck", -1},

};

const char* what__printf(Le_af leaf)
{
    for(int i = 0; i < (int)(sizeof(translit) / sizeof(translit[0])); ++i)
        if(leaf->value.oper == (size_t)translit[i].e_num)
            return translit[i].value;

    return "unluck i dont know this func";
}
