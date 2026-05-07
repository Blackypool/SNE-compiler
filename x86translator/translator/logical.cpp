#include "logical.h"

//____________________________________________________JUMP_ERS____________________________________________________________________________//
static struct znaki jumpers[] = {

    {"je", EQUAL_C},
    {"jne", N_EQUAL_C},

    {"jge", L_E_bigger},
    {"jg", L_bigger_R},

    {"jle", L_E_smaller},
    {"jl", L_smaller_R},
    
};

void bear_gammy_jump_func(Arg_s)
{
    DE_BUG(leaf);

    Asm_expression(fp, leaf->left,  ast);
    Asm_expression(fp, leaf->right, ast);

    fprintf(fp, "       pop rcx\n");
    fprintf(fp, "       pop rax\n\n");

    fprintf(fp, "       cmp rax, rcx\n");

    ////////////////////////////////////

    scope_table* for_label_name_stk = stack_pop(ast->labels_names_for_if_while);
    stack_push(ast->labels_names_for_if_while, for_label_name_stk);

    char* label_name = for_label_name_stk->all_param_s[0].name_of_var;

    size_t size_of_JT_table = sizeof(jumpers) / sizeof(jumpers[0]);

    for(size_t i = 0; i < size_of_JT_table; ++i)
        if(leaf->value.oper == (size_t)jumpers[i].e_num)
            fprintf(fp, "%s %s\n", jumpers[i].value, label_name);
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________IF__WHILE___________________________________________________________________________//
void Asm_if_cmd(Arg_s)
{

// сначала нужно поставить метку а потом вызвать Asm_expression а то внутри опять ченить вызовется такое и мы потеряе номер метки ? сохранять его
// возвращть функцией препарейшен надо и сохранять потоом экспрессион а потом все остальное 
    DE_BUG(leaf);    

    Le_af root_of_if = leaf;
    char* name_forr_if = preparation_if_while(arg_s);
    AsserT(name_forr_if == NULL, error_in_deep, );
    
    /////////////////CONDITION/////////////////////
    Asm_expression(fp, root_of_if->left, ast);   
    ///////////////////////////////////////////////

// сверху произошла обработка кмп
// значит сейчас обрабатываем елсе 
// потом ставим метку которые из елса перепрыгиваем и пускаем иф после которого ставим метку на которыую должен прыгнуть елсе а она получается путем снпринтфа с припиской елсе


    char end_of_if_label[120] = {};
    snprintf(end_of_if_label, sizeof(end_of_if_label), "else_end_%s", name_forr_if);

    if(leaf->right->value.oper == ELSE_C)
    {
        /////////////////ELSE/////////////////////////
        fprintf(fp, "; else:\n");

        Asm_expression(fp, leaf->right->right, ast);
        
        fprintf(fp, "jmp %s\n", end_of_if_label);
        ///////////////////////////////////////////////

        /////////////////_IF_//////////////////////////
        fprintf(fp, "; if:\n");

        Asm_expression(fp, leaf->right->left, ast);
        
        fprintf(fp, "%s:\n", end_of_if_label);
        ///////////////////////////////////////////////
    }
    else
        {
            /////////////////_IF_//////////////////////////
            fprintf(fp, "jmp %s\n", end_of_if_label);
            fprintf(fp, "\n; if:\n");
            fprintf(fp, "%s:\n\n", name_forr_if);

            Asm_expression(fp, leaf->right, ast);
        
            fprintf(fp, "%s:\n", end_of_if_label);
            ///////////////////////////////////////////////

        }
    // копиурем область видимости создаем свою также как в функопенспей 
    // ставим флаги что мы находимся именно в ифе или вайле - нет не надо уже есть на это логика //
    // при встрече инита увеличивать смещение 
    // на старте положить в нулевую ячецку нового стека название, для названий нужна глобальное число чтобы не было повторений 
    
// посчитать сколько инитов для инициализации массива на стеке

    scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);
    ast->is_global_now = stk_for_if_wh->all_param_s[0].is_global;

    free(name_forr_if);
}

void Asm_while_cmd(Arg_s)
{
    DE_BUG(leaf);    

    Le_af root_of_while = leaf;
    char* name_forr_while = preparation_if_while(arg_s);  
    AsserT(name_forr_while == NULL, error_in_deep, );    

    ///////////////////////////////////////////////
    char start_of_while_[120] = {};
    snprintf(start_of_while_, sizeof(start_of_while_), "start_of_while_%s", name_forr_while);

    fprintf(fp, "%s:\n", start_of_while_);

    /////////////////CONDITION/////////////////////
    Asm_expression(fp, root_of_while->left, ast);   
    ///////////////////////////////////////////////


    /////////////////////BODY//////////////////////
    Asm_expression(fp, root_of_while->right, ast);   
    fprintf(fp, "jmp %s", start_of_while_);
    ///////////////////////////////////////////////


    /////////////////////END//////////////////////
    fprintf(fp, "%s:\n", name_forr_while);
    ///////////////////////////////////////////////

    scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);
    ast->is_global_now = stk_for_if_wh->all_param_s[0].is_global;

    free(name_forr_while);
}
//________________________________________________________________________________________________________________________________________//

char* preparation_if_while(Arg_s)
{
    DE_BUG(leaf);

    ///////////////COPY_OLD_SCOPE///////////////////
    scope_table* new_space = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(new_space == NULL, memory_aloca, NULL);

    new_space->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space->all_param_s == NULL, memory_aloca, NULL);
    ///////////////////////////////////////////////


    /////////////////INITial_IF/////////////////
    scope_table* stk_for_if_wh = (scope_table*)calloc(1, sizeof(scope_table));
    AsserT(stk_for_if_wh == NULL, memory_aloca, NULL);

    stk_for_if_wh->all_param_s = (params_in_scope*)calloc(ast->max_varia_num, sizeof(params_in_scope));
    AsserT(stk_for_if_wh->all_param_s == NULL, memory_aloca, NULL);
    ///////////////////////////////////////////////

    stk_for_if_wh->all_param_s[0].call_number = 1;          // чтобы не затереть при добавлении в rsp + 8 * call_number
    

    /////// всего инитов
    int i_i = 0;   
    Le_af root_of_if = leaf;      
    how_many_init(root_of_if, &i_i);
    fprintf(fp, "\n              ;[  ]");
    fprintf(fp, "\n       sub rsp, %d        ; massiva for senyora if / while\n", (8 * i_i));
    ///////


    /////// name label
    char name_forr_if[120] = {};
    snprintf(name_forr_if, sizeof(name_forr_if), "label_%d", ast->free_label_for_if);

    stk_for_if_wh->all_param_s[0].name_of_var = strdup(name_forr_if);
    (ast->free_label_for_if)++;
    ///////

    /////// сохраняем предыдущюю область видимости
    stk_for_if_wh->all_param_s[0].is_global = ast->is_global_now;
    ///////

    stack_push(ast->labels_names_for_if_while, stk_for_if_wh);

    return strdup(name_forr_if);
}

params_in_scope* search_info_for_IF_stk(ar_get, scope_table* stk_for_if_wh, const char* name)
{              // тк в 0 тех инфо об ифе
    for(size_t i = 1; i < ast->max_varia_num && stk_for_if_wh->all_param_s[i].name_of_var != NULL; i++)
        if(strcmp(name, stk_for_if_wh->all_param_s[i].name_of_var) == 0)
            return &(stk_for_if_wh->all_param_s[i]);

    fprintf(stderr, "varia <%s>not found in oblast if / while\n\n", name);
    return NULL;
}
    
void how_many_init(Le_af root_of_if, int* count)
{
    Le_af next_ik = root_of_if;

    if(root_of_if == NULL)
        return ;

    if(next_ik->value.oper == IN_IT)
        (*count)++;

    how_many_init(root_of_if->left, count);
    how_many_init(root_of_if->right, count);
}

//________________________________________________________________________________________________________________________________________//
