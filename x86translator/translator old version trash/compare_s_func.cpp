#include "compare_s_func.h"

#define PR_ASM \
        Asm_expression(fp, leaf->left, ast); \
        Asm_expression(fp, leaf->right, ast);

#define Debug \
        fprintf(stderr, "\n====FUNC_%s====  type = -%zu-\n\n", __func__, leaf->type);

        
void Asm_if_cmd(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    (ast->func_num)++;

    int end_if = (ast->func_num);
    (ast->func_num)++;

    int start_else = (ast->func_num);
    (ast->func_num)++;

    Asm_expression(fp, leaf->left, ast); //resultat usloviya ifa

    if(leaf->right->type == OPERAT && leaf->right->value.oper == ELSE_C)
    {
        // if(
        fprintf(fp, "PUSH 0\n"); //cmp with zero
        fprintf(fp, "JE :%d\n", start_else); //if == 0 ->> else jump to else
        // )
        
        // {
        Asm_expression(fp, leaf->right->left, ast);
        fprintf(fp, "JMP :%d\n", end_if);
        // }

        //else
        // {
        fprintf(fp, ":%d\n", start_else);
        Asm_expression(fp, leaf->right->right, ast);
        fprintf(fp, "JMP :%d\n", end_if);
        // }
    }

    else  //without else
    {
        // if(
        fprintf(fp, "PUSH 0\n"); //cmp with zero
        fprintf(fp, "JE :%d\n", end_if); //if == 0 ->> jump to endif
        // )


        // {
        Asm_expression(fp, leaf->right, ast);
        fprintf(fp, "JMP :%d\n", end_if);
        // }
    }

    fprintf(fp, ":%d\n", end_if);
}


void Asm_while_cmd(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    (ast->func_num)++;

    int sam_while = (ast->func_num);
    (ast->func_num)++;

    int end_while = (ast->func_num);
    (ast->func_num)++;

    fprintf(fp, "JMP :%d\n", sam_while);
    fprintf(fp, ":%d\n", sam_while);

    // while(
    Asm_expression(fp, leaf->left, ast); //resultat usloviya ifa
    fprintf(fp, "PUSH 0\n"); //compare with zero
    fprintf(fp, "JE :%d\n", end_while); //if == 0 ->> jump out
    // )

    // {
    Asm_expression(fp, leaf->right, ast);
    fprintf(fp, "JMP :%d\n", sam_while);
    // }

    fprintf(fp, "JMP :%d\n", end_while);
    fprintf(fp, ":%d\n", end_while);
}


static struct znaki jumpers[] = {

    {"JE ", EQUAL_C},

    {"JAE ", L_E_bigger},
    {"JA ", L_bigger_R},

    {"JBE ", L_E_smaller},
    {"JB ", L_smaller_R},
    
    {"JNE ", N_EQUAL_C},

};


void bear_gammy_jump_func(FILE* fp, Le_af leaf, ar_get)
{            
    Asm_expression(fp, leaf->left,  ast); ///////////////////////////////
    Asm_expression(fp, leaf->right, ast);

    (ast->func_num)++;
    int end_of_JJ = (ast->func_num);

    (ast->func_num)++;

    int true_usl = (ast->func_num);
    (ast->func_num)++;

    for(int i = 0; i < (int)(sizeof(jumpers) / sizeof(jumpers[0])); ++i)
        if(leaf->value.oper == (size_t)jumpers[i].e_num)
            fprintf(fp, "%s:%d\n", jumpers[i].value, true_usl);


    fprintf(fp, "PUSH 0\n");
    fprintf(fp, "JMP :%d\n", end_of_JJ);
    
    fprintf(fp, ":%d\n", true_usl);
    fprintf(fp, "PUSH 1\n");
    fprintf(fp, "JMP :%d\n", end_of_JJ);

    fprintf(fp, ":%d\n", end_of_JJ);
}
