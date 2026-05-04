#include "do_and_crash.h"

Le_af merger(char* argv, struct A_S_T* ast)
{
    char name_of_file[120] = {};
    snprintf(name_of_file, sizeof(name_of_file), "programs/%s.txt", argv);

    char tree_asm_name[150] = {};
    snprintf(tree_asm_name, sizeof(tree_asm_name), "A_skompile_result/%s.asm", argv);
    ast->end_file_name = tree_asm_name;

    char* tree_txt = file__read(name_of_file);  //прочитали файл + init num varioa and func
    char* save_ptr = tree_txt;

    Le_af root = create_base(&tree_txt, ast);   //создали дерево по файлу
    picture_of_root(root, "AST");
    ast->root_of_ast = root;

    lego_stack(ast->skope_stack, ast->max_func_user_num);
    
    free(save_ptr);

    ////////////////////////////
    // need init ast ???????? //
    ////////////////////////////

    return root;
}


void fell_a_root(struct leaf **root)
{
    if(*root != NULL)
    {
        if((*root)->left != NULL)
            cleaner(&((*root)->left));
        
        if((*root)->right != NULL)
            cleaner(&((*root)->right));
    }

    if(*root != NULL)
    {
        free(*root);
        *root = NULL;
    }
}


void cleaner(struct leaf **left_right)
{
    if(*left_right == NULL)
        return ;

    if((*left_right)->left != NULL)
        cleaner(&(*left_right)->left);
    
    if((*left_right)->right != NULL)
        cleaner(&(*left_right)->right);

    free(*left_right);
    *left_right = NULL;
}
