#include "AST_to_asm.h"

///////     AX, CX = trash
///////     ecx = take varia from memory>????
///////
///////

// при инициализации переменной проверять если стек пустой значит надо каллочить
// при заходе в скобочки то есть в иф вайл функцию создается новая область видимости которая забирается либо закрывающимися скобками вайла и ифа или при инициализации функции

// правило: глобальные переменные не могут быть вызовом чего-то это сразу число == плохо а как же скан како-нибудь ????

// cdecle style ???? надо бы паскаль но это тестами проверим в падлу разбираться ща


// на каждый вызов ифа/ вайла нужно пушить номмер метки + название В НУЛЕВОЙ ЯЧЕЙКЕ

// нельзя при рекурсивном вызове ифа определить потом куда мы вернулись в глобал или локал
// можно сделать функцию которая автоматически определяет местоположение и устанавливает нужный флаг
// делает она это через  област видимости
// функцию ставить после вызова в асм эгозер ифа и вайла при инициализации функцуии все норм тк нельзя инициализировать функцию в функции 
// если стек области видимости пустой это еще не значит что мы глобал

//_____________________________________________________START_____________________________________________________________________________//
int asm_main(ar_get)
{
    AsserT(ast == NULL, give_null_ptr, give_null_ptr);

    ast->is_global_now = GLOBA_L;

    FILE* fp = fopen(ast->end_file_name, "w");
    AsserT(fp == NULL, file_errorr, file_errorr);

    fprintf(fp, "section .text\n");
    fprintf(fp, "global _start\n_start:\n");

    ////////////////////////////

    Asm_expression(fp, ast->root_of_ast, ast);

    ////////////////////////////

    fprintf(fp, "HLT");

    fclose(fp);

    return 1;
}

void Asm_expression(Arg_s)          // types
{
    DE_BUG(leaf);

    if(leaf == NULL)
        return ;

    if(leaf->type == Z_NAK && leaf->value.oper == ZAPYTAYA)   
        return ;
    

    switch (leaf->type)
    {
        case NUMBER:
            fprintf(fp, "mov rax, %.0lf\n", leaf->value.num);
            fprintf(fp, "push rax");
        break;

        case VARIA:
            var_printing(arg_s);
        break;

        case OPERAT:
            operat_ptinting(arg_s);
        break;

        case USER_OPER: //alresdy in use
            func_user_ptinting(arg_s);
        break;

        case Z_NAK:
            Asm_expression(fp, leaf->left, ast);
            Asm_expression(fp, leaf->right, ast);
        break;

        default:
            fprintf(stderr, "\n Asm_expression case ruined\n\n");
        break;
    }
}
//_______________________________________________________________________________________________________________________________________//



//___________________________________________________DEFAULT_FUNC________________________________________________________________________//
static struct znaki translet_to_ASM[] = {

    {"add", ADD_C},
    {"imul", MUL_C},
    {"sub", SUB_C},
    {"idiv", DIV_C},

    {"call pow_func\n", POW_C},     // добавить через инклуд как и принтф

    {"and", LOG_AND},     
    {"or", LOG_OR},     

};

const char* asm_ready_commands(Le_af leaf)
{
    AsserT(leaf == NULL, give_null_ptr, NULL);
    DE_BUG(leaf);

    size_t sz_of_translete = sizeof(translet_to_ASM) / sizeof(translet_to_ASM[0]);

    for(size_t i = 0; i < sz_of_translete; i++)
        if(leaf->value.oper == (size_t)translet_to_ASM[i].e_num)
            return translet_to_ASM[i].value;

    return NULL;
}

void operat_ptinting(Arg_s)
{
    DE_BUG(leaf);

    const char* command = asm_ready_commands(leaf);
    if(command == NULL)
    {
        Asm_another(fp, leaf, ast);

        return ;
    }

    Asm_expression(fp, leaf->left, ast);
    Asm_expression(fp, leaf->right, ast);

    
    if(strcmp(command, "call pow_func\n") == 0)    // параметры для функции уже в стеке
    {
        fprintf(fp, "pop rsi\n");     //  - степень
        fprintf(fp, "pop rdi\n");     // - что возводим  

        fprintf(fp, "%s\n", command);

        fprintf(fp, "push rax\n");

        return ;
    }


    fprintf(fp, "pop rcx\n");
    fprintf(fp, "pop rax\n");


    if(strcmp(command, "idiv") == 0)
    {
        fprintf(fp, "cdq\n");           // расширяем EAX → EDX:EAX
        fprintf(fp, "idiv ecx\n");

        fprintf(fp, "push rax\n");

        return ;
    }


    fprintf(fp, "%s eax, ecx\n");

    fprintf(fp, "push rax\n");

    return ; 
}
//________________________________________________________________________________________________________________________________________//



//___________________________________________________NOT_DEF_FUNC_________________________________________________________________________//
void Asm_another(FILE* fp, Le_af leaf, ar_get)
{
    if(leaf == NULL)
        return ;

    if(leaf->value.oper == ZAPYTAYA)
        return ;

    if(leaf->type == OPERAT)
    {
        switch(leaf->value.oper)
        {
            case TOCHKA_ZAP:
                Asm_expression(fp, leaf->left, ast);
                Asm_expression(fp, leaf->right, ast);
            break;

            case USER_OPER:     
                user_oper_init(fp, leaf, ast);
            break;

            case IF_C:
                Asm_if_cmd(fp, leaf, ast);
            break;

            case RET_C:
            {
                Asm_expression(fp, leaf->left, ast);        // чтобы можно было возвращать выражения

                if(ast->id_of_now_func != 0) // => leave from func
                    fprintf(fp, "pop rbp\n");

                fprintf(fp, "ret\n");
            }
            break;

            case WHILE_C:
                Asm_while_cmd(fp, leaf, ast);
            break;

            case ELSE_C:
                fprintf(stderr, "\nOKAK, else without if?!\n\n");
            break;

            case IN_IT:
                Asm_init_varia(arg_s);
            break;

            case RAVNO:
                embezzlement(arg_s);
            break;

        //// standart functions vizov    
            // case PRINT_F:???
            //     Asm_expression(fp, leaf->left, ast);
            //     fprintf(fp, "OUT\n");
            // break;
            //
            // case SQRT_C:???
            //     Asm_expression(fp, leaf->left, ast);
            //     fprintf(fp, "SQRT\n");
            // break;
            //
            // case SCAN_C:???
            //     fprintf(fp, "INT\n");
            // break;
        ////

            case EQUAL_C:
            case L_E_bigger:
            case L_bigger_R:
            case L_E_smaller:
            case L_smaller_R:
            case N_EQUAL_C:

            bear_gammy_jump_func(fp, leaf, ast);

            break;

            default:
                fprintf(stderr, "\nOPERAT poteralsa with enum == %zu \n\n", leaf->value.oper);
            break;
        }
    }
}
//________________________________________________________________________________________________________________________________________//



//____________________________________________________ASSIGNMENT__________________________________________________________________________//
void embezzlement(Arg_s)    // присвоение, хищничество
{
    DE_BUG(leaf);

    char* name = strdup(leaf->left->value.x);
    // проверяем на есть ли вообще в области видимости такой элемент и достаем информацию о нем     
    // потом юзаем асм экспрессион для правого борта и сохраняем по достанному адремсу из структуры параметра 

    params_in_scope* varia_stk = search_in_scope(ast, name);
    AsserT(varia_stk == NULL, error_in_deep, );
    ////////////////////////////////////

    Asm_expression(fp, leaf->right, ast);   // in stack value of varia

    fprintf(fp, "pop rax\n");

    ////////////////////////////////////
    if(varia_stk->is_it_func_param == YES_IT_IS)
    {
        fprintf(fp, "mov [rbp + 16 + %d], eax       ; place to [%d] param = <%s> for func from stack\n", (8 * varia_stk->call_number), (varia_stk->call_number + 1), name);

        free(name);

        return ;
    }


    if(varia_stk->is_global == GLOBA_L)
        fprintf(fp, "mov dword ptr [rel %s], eax     ; place to global label param <%s>\n", name, name);
    

    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func > - 1)     // => in func use init
        fprintf(fp, "mov [rbp - %d], eax             ; place to stack mem <%s>\n", (8 * varia_stk->offset_in_loca_l), name);


    if(varia_stk->is_global == LOCA_L && ast->id_of_now_func < 0)     // => if/while block
    {
        scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);

        params_in_scope* name_var_stk = search_info_for_IF_stk(ast, stk_for_if_wh, name);
        
        fprintf(fp, "mov [rsp + %d], eax             ; place to local massive <%s>\n", (8 * name_var_stk->offset_in_loca_l), name);
    
        stack_push(ast->labels_names_for_if_while, stk_for_if_wh);
    }
    ////////////////////////////////////

    free(name);
}
//________________________________________________________________________________________________________________________________________//



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

    fprintf(fp, "pop rcx\n");
    fprintf(fp, "pop rax\n");

    fprintf(fp, "cmp rax, rcx\n");

    ////////////////////////////////////

    scope_table* for_label_name_stk = stack_pop(ast->skope_stack);
    stack_push(ast->skope_stack, for_label_name_stk);

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
    
    /////////////////CONDITION/////////////////////
    Asm_expression(fp, root_of_if->left, ast);   
    ///////////////////////////////////////////////

// сверху произошла обработка кмп
// значит сейчас обрабатываем елсе 
// потом ставим метку которые из елса перепрыгиваем и пускаем иф после которого ставим метку на которыую должен прыгнуть елсе а она получается путем снпринтфа с припиской елсе


    if(leaf->right->value.oper == ELSE_C)
    {
        /////////////////ELSE/////////////////////////
        char end_of_if_label[120] = {};
        snprintf(end_of_if_label, sizeof(end_of_if_label), "else_end_%s", name_forr_if);

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
        Asm_expression(fp, leaf->right, ast);

    // копиурем область видимости создаем свою также как в функопенспей 
    // ставим флаги что мы находимся именно в ифе или вайле - нет не надо уже есть на это логика //
    // при встрече инита увеличивать смещение 
    // на старте положить в нулевую ячецку нового стека название, для названий нужна глобальное число чтобы не было повторений 
    
// посчитать сколько инитов для инициализации массива на стеке

    scope_table* stk_for_if_wh = stack_pop(ast->labels_names_for_if_while);
    ast->is_global_now = stk_for_if_wh->all_param_s[0].is_global;
}

void Asm_while_cmd(Arg_s)
{
    DE_BUG(leaf);    

    Le_af root_of_while = leaf;
    char* name_forr_while = preparation_if_while(arg_s);      

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
}
//________________________________________________________________________________________________________________________________________//

char* preparation_if_while(Arg_s)
{
    DE_BUG(leaf);

    ///////////////COPY_OLD_SCOPE///////////////////
    scope_table* new_space = (scope_table*)calloc((size_t)ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space == NULL, memory_aloca, );

    int i_i = 0;
    if(ast->skope_stack->size > 0)
        copy_old_scope(ast, new_space, &i_i);               // i_i == свободная ячейка для новых параметров для ифа и вайла не надо
    ///////////////////////////////////////////////


    /////////////////INITial_IF/////////////////
    scope_table* stk_for_if_wh = (scope_table*)calloc((size_t)ast->max_varia_num, sizeof(params_in_scope));
    AsserT(new_space == NULL, memory_aloca, );

    stk_for_if_wh->all_param_s[0].call_number = 1;          // чтобы не затереть при добавлении в rsp + 8 * call_number
    

    /////// всего инитов
    i_i = 0;   
    Le_af root_of_if = leaf;      
    how_many_init(root_of_if, &i_i);
    fprintf(fp, "sub rsp, %d        ; massiva for senyora if / while\n", (8 * i_i));
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

    return name_forr_if;
}

params_in_scope* search_info_for_IF_stk(ar_get, scope_table* stk_for_if_wh, char* name)
{              // тк в 0 тех инфо об ифе
    for(size_t i = 1; i < ast->max_func_user_num; i++)
        if(strcmp(name, stk_for_if_wh->all_param_s[i].name_of_var) == 0)
            return &(stk_for_if_wh->all_param_s[i]);

    fprintf(stderr, "varia <%s>not found in oblast if / while\n\n", name);
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
