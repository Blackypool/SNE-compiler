#include "do_and_crash.h"

Le_af merger(char* argv, struct A_S_T* ast)
{
    ////////FILE_NAMES//////////
    char name_of_file[120] = {};
    snprintf(name_of_file, sizeof(name_of_file), "programs/%s.txt", argv);

    char tree_asm_name[150] = {};
    snprintf(tree_asm_name, sizeof(tree_asm_name), "A_skompile_result/%s.asm", argv);
    ast->end_file_name = tree_asm_name;
    ////////////////////////////


    /////////DO_AST/////////////
    char* tree_txt = file__read(name_of_file);  //прочитали файл
    char* save_ptr = tree_txt;

    Le_af root = create_base(&tree_txt, ast);   //создали дерево по файлу + init num varioa and func
    picture_of_root(root, "AST");
    ast->root_of_ast = root;
    ////////////////////////////


    //////DOUBLE_STACK//////////
    ast->skope_stack = (stk*)calloc(1, sizeof(stk));
    AsserT(ast->skope_stack == NULL, memory_aloca, NULL);

    ast->labels_names_for_if_while = (stk*)calloc(1, sizeof(stk));
    AsserT(ast->labels_names_for_if_while == NULL, memory_aloca, NULL);

    lego_stack(ast->skope_stack, (ssize_t)ast->max_func_user_num);
    lego_stack(ast->labels_names_for_if_while, (ssize_t)ast->max_func_user_num);
    ////////////////////////////


    ///////////FUNC/////////////
    ast->all_func = (user_func_info*)calloc(ast->max_func_user_num, sizeof(user_func_info));
    AsserT(ast->all_func == NULL, memory_aloca, NULL);

    ast->id_of_now_func = -123;      // чтобы изначально не быть в функции 
    ////////////////////////////


    ////////////DATA////////////
    ast->section_data = (char**)calloc(ast->max_varia_num, sizeof(*ast->section_data));
    AsserT(ast->section_data == NULL, memory_aloca, NULL);

    ast->n_omer_real_global_for_data_sec = 0;
    ////////////////////////////

    free(save_ptr);

    return root;
}


void fell_a_root(struct leaf **root)
{
    // очситка стеков тоже нужна будет потом
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
