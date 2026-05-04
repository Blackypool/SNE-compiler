#include "logical_summ.h"

#define PR_ASM \
        Asm_expression(fp, leaf->left, ast); \
        Asm_expression(fp, leaf->right, ast);

#define Debug \
        fprintf(stderr, "\n====FUNC_%s====  type = -%zu-\n\n", __func__, leaf->type);


static struct znaki translit[] = {

    {"ADD\n", ADD_C},
    {"POW\n", POW_C},
    {"DIV\n", DIV_C},
    {"MUL\n", MUL_C},
    {"SUB\n", SUB_C},

};


int asm_post_or(Le_af leaf)
{
    Debug;

    for(int i = 0; i < (int)(sizeof(translit) / sizeof(translit[0])); ++i)
        if(leaf->value.oper == (size_t)translit[i].e_num)
            return i;

    return -1;
}


void operat_ptinting(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    int i = asm_post_or(leaf);      // if i >= 0 => func from translit
    if(i != -1)
    {
        PR_ASM; //требующие постордер функции
        fprintf(fp, "%s", translit[i].value);
        return ;    // result in stack
    }

    Asm_another(fp, leaf, ast); //остальные оперции
}


void Asm_logical_or(FILE* fp, Le_af leaf, ar_get)// переписать бы
{
    Debug;

    (ast->func_num)++;

    int true_labl = (ast->func_num);
    (ast->func_num)++;

    int end_lbl = (ast->func_num);
    (ast->func_num)++;

    Asm_expression(fp, leaf->left, ast);  //in steck left param 
    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JNE :%d\n", true_labl);


    Asm_expression(fp, leaf->right, ast); //right
    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JNE :%d\n", true_labl);


    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JMP :%d\n", end_lbl);


    fprintf(fp, ":%d\n", true_labl);
    fprintf(fp, "PUSH 1\n");
    fprintf(fp, "JMP :%d\n", end_lbl);

    fprintf(fp, ":%d\n", end_lbl);
}


void Asm_logical_and(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    (ast->func_num)++;

    int false_label = (ast->func_num);
    (ast->func_num)++;

    int end_label = (ast->func_num);
    (ast->func_num)++;

    Asm_expression(fp, leaf->left, ast);  //in steck left param 
    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JE :%d\n", false_label);

    Asm_expression(fp, leaf->right, ast); //right
    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JE :%d\n", false_label);

    fprintf(fp, "PUSH 1\n");
    fprintf(fp, "JMP :%d\n", end_label);

    fprintf(fp, ":%d\n", false_label);
    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JMP :%d\n", end_label);

    fprintf(fp, ":%d\n", end_label);
}
