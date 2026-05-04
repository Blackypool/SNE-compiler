#include "dump_gr.h"


void picture_of_root(struct leaf *root, const char* text)
{
    AsserT(root == NULL, give_null_ptr, );

    char str[256] = {};
    snprintf(str, sizeof(str), "dump_graph/Diff_dump_%s.dot", text);

    FILE *fp_out = fopen(str, "w");
    AsserT(fp_out == NULL, file_errorr, );

    fprintf(fp_out, "digraph\n{\nrankdir=TB;\nnodesep=0.4;\nranksep=1.0;\n");

    fprintf(fp_out, "  labelloc=\"t\";\n");
    fprintf(fp_out, "  label=\"ROOT: %p\n%s\";\n", root, text);


    draw_clear_leaf(fp_out, root);

    draw_stick(fp_out, root);


    fprintf(fp_out, "\n}");

    fclose(fp_out);


    call_terminal(text);
}


const char* translate_r(size_t oper)
{
    for(size_t i = 0; i < znaki_struct_size; ++i)
        if((int)oper == com_transl[i].e_num)
        {
            if(strcmp(com_transl[i].value, ">") == 0)  return "\\>";
            if(strcmp(com_transl[i].value, "<") == 0)  return "\\<";
            if(strcmp(com_transl[i].value, ">=") == 0) return "\\>=";
            if(strcmp(com_transl[i].value, "<=") == 0) return "\\<=";
            if(strcmp(com_transl[i].value, "&&") == 0) return "\\&\\&";
            if(strcmp(com_transl[i].value, "||") == 0) return "||"; 

            return com_transl[i].value;  
        }

    return "unluck";
}


void draw_clear_leaf(FILE *fp_out, struct leaf *leaf)
{
    if(leaf == NULL)
    {
        return ;
    }
    
    if(leaf->type == OPERAT || leaf->type == CONECTION)
    {
        fprintf(fp_out, "leaf_%p [label = \"%s\", \
                     style = \"rounded, filled\", \
                        fillcolor = \"#e356a4ff\", fontcolor = \"#000000\", shape = Mrecord];\n", leaf, translate_r(leaf->value.oper));
    }
    
    if(leaf->type == NUMBER)
    {
        fprintf(fp_out, "leaf_%p [label = \"%lf\", \
                     style = \"rounded, filled\", \
                        fillcolor = \"#a2a6f1ff\", fontcolor = \"#000000\", shape = Mrecord];\n", leaf, leaf->value.num);
    }
        
    if(leaf->type == VARIA)
    {
        fprintf(fp_out, "leaf_%p [label = \"%s\", \
                     style = \"rounded, filled\", \
                        fillcolor = \"#feea54ff\", fontcolor = \"#000000\", shape = Mrecord];\n", leaf, leaf->value.x);
    }

    if(leaf->type == USER_OPER)
    {
        fprintf(fp_out, "leaf_%p [label = \"%s\", \
                     style = \"rounded, filled\", \
                        fillcolor = \"#e849faff\", fontcolor = \"#000000\", shape = Mrecord];\n", leaf, leaf->value.us_op);
    }

    if(leaf->type == Z_NAK)
    {
        fprintf(fp_out, "leaf_%p [label = \"%s\", \
                     style = \"rounded, filled\", \
                        fillcolor = \"#49fafaff\", fontcolor = \"#000000\", shape = Mrecord];\n", leaf, translate_r(leaf->value.oper));
    }

    if(leaf->left != NULL)
        draw_clear_leaf(fp_out, leaf->left);

    if(leaf->right != NULL)
        draw_clear_leaf(fp_out, leaf->right);
}


void draw_stick(FILE *fp_out, struct leaf *leaf)
{
    if(leaf != NULL)
    {
        if(leaf->left != NULL)
        {
            fprintf(fp_out, "leaf_%p -> leaf_%p;\n", leaf, leaf->left);
            draw_stick(fp_out, leaf->left);
        }

        if(leaf->right != NULL)
        {
            fprintf(fp_out, "leaf_%p -> leaf_%p;\n", leaf, leaf->right);
            draw_stick(fp_out, leaf->right);
        }
    }
}


void call_terminal(const char* text) 
{
    char apple_script[512] = {};

    snprintf(apple_script, sizeof(apple_script), "dot -Tpng dump_graph/Diff_dump_%s.dot -o dump_graph/dump_%s.png", text, text);
    
    system(apple_script);
}
