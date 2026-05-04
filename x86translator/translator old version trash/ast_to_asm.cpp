#include "read_tree.h"

#define PR_ASM \
        Asm_expression(fp, leaf->left, ast); \
        Asm_expression(fp, leaf->right, ast);

#define Debug \
        fprintf(stderr, "\n====FUNC_%s====  type = -%zu-\n\n", __func__, leaf->type);


// AX   = ret of func
// BX   = ptr to prev memory location (for supp rek)
// CX   = can use anywhere
// DX   = ptr to memory
// EX   = need to достать из памяти како-нибудь параметр

// also can use anywherre:
// FX
// GX
// HX
// IX
// GX


void Asm_another(FILE* fp, Le_af leaf, ar_get)
{
    if(leaf == NULL)
        return ;

    if(leaf->value.oper == ZAPYTAYA)
        return ;

    Debug;

        if(leaf->type == OPERAT)
        {
            //fprintf(stderr, "\n\n OPERAT poteralsa with enum == %zu \n\n\n", leaf->value.oper);
            switch(leaf->value.oper)
            {
                case TOCHKA_ZAP:
                    PR_ASM;
                break;

                case USER_OPER:     
                    user_oper_init(fp, leaf, ast); //инициализация функции пользователя
                break;

                case IF_C:
                    Asm_if_cmd(fp, leaf, ast);
                break;

                case RET_C:
                    Asm_expression(fp, leaf->left, ast);        // чтобы можно было возвращать выражения
                    Asm_ret_for_rek_supp(fp);
                break;

                case WHILE_C:
                    Asm_while_cmd(fp, leaf, ast);
                break;

                case ELSE_C:
                    fprintf(stderr, "\nOKAK, else without if?!\n\n");
                break;

                case IN_IT:
                    Asm_init(arg_s);
                break;

                case RAVNO:
                    Asm_ass_ignment(arg_s);
                break;

                case LOG_AND:
                    Asm_logical_and(arg_s);
                break;

                case LOG_OR:
                    Asm_logical_or(arg_s);
                break;

                case PRINT_F:
                    Asm_expression(fp, leaf->left, ast);
                    fprintf(fp, "OUT\n");
                break;

                case SQRT_C:
                    Asm_expression(fp, leaf->left, ast);
                    fprintf(fp, "SQRT\n");
                break;

                case SCAN_C:
                    fprintf(fp, "INT\n");
                break;

                case EQUAL_C:
                case L_E_bigger:
                case L_bigger_R:
                case L_E_smaller:
                case L_smaller_R:
                case N_EQUAL_C:

                bear_gammy_jump_func(fp, leaf, ast);

                break;

                default:
                    fprintf(stderr, "\n%zu unluck Asm_another Asm_another Asm_another\n\n", leaf->value.oper);
                break;
            }
        }
}


void user_oper_init(Arg_s)
{
    Debug;
    (ast->func_num)++;

    int main_offset_addr = 0;
    main_offset_addr = ast->Adress_next;
    ast->Adress_next = 0;   //offset for params

    //leaf == func == operat
    //leaf->left == name of func
    //leaf->right == connect
    //connect->left->left->left == params
    //connect->right == body

    Le_af name_of_func = leaf->left;
    Le_af connect = leaf->right;
    Le_af body = connect->right;

    int func_id = ast->init_num;
    (ast->init_num)++;

    /////save func_name
    ast->userz_func[func_id].name = strdup(name_of_func->value.us_op);

    //ffor seeing params
    table_name* param_array = (table_name*)calloc((size_t)ast->max_num, sizeof(table_name));
    AsserT(param_array == NULL, );

    //func factorial (param1 param2) {body}

    //////////////////
    Le_af param = connect->left;
    int i = 0;
    while(param != NULL) //колво парам
    {
        param_array[i].name = strdup(param->value.x);
        param_array[i].num_param = i;
        param_array[i].adress = -1;

        param = param->left;
        ++i;
    }

    ast->userz_func[func_id].size_tabl = (size_t)i;  //num of params
    ast->userz_func[func_id].init_params = param_array;  //massiva wiyh info about params
    //////////////////
    
    ast->userz_func[func_id].adress = ast->func_num;  //адресс функции == номер метки на будущее
    
    int start_of_func = (ast->func_num);
    (ast->func_num)++;
    
    int end_of_func = (ast->func_num);
    (ast->func_num)++;

    // func
    fprintf(fp, "JMP :%d\n", end_of_func);  //init перепрыгиваем внутри основного кода 
    fprintf(fp, ":%d\n", start_of_func);    //lable    //сюда будет колл

    // (
    func_open_space(connect, ast, i, func_id);   //парамс чек+ инит адреса
    // )

//     // {    support rekursia
//     fprintf(fp, "PUSHREG BX\n");       // ; mov [dx], bx
//     fprintf(fp, "POPM [DX]\n");

//     fprintf(fp, "PUSHREG DX\n");       // ; mov bx, dx     
//     fprintf(fp, "POPREG BX\n");
//     // }
    
// //////////////////////////////////////save regs -->
    fprintf(fp, "PUSHREG HX\n");

    fprintf(fp, "PUSHREG CX\n");
// //////////////////////////////////////

    //[
    Asm_expression(fp, body, ast);  //body (ret in here)
    //]
    
    // {    skip metka
    fprintf(fp, "JMP :%d\n", end_of_func); //в конец
    // }

    fprintf(fp, ":%d\n", end_of_func);

    stack_pop(&ast->var_ble_tabl);              //after ret go out see zone          

// description to how work rekurs support -->
    // ; самый первый вызов (нужна глубина рекурсии флаг что мы и так в функции тогда другая конструкция)
    // mov bx, dx              ; сохраняем указатель на текущую область памяти
    // add dx, 100             ; выделяем новую память под call
    //
    // call func               ; при вызове все регистры сохраняются 
    // {
    // mov [dx], bx        ; в новой памяти в первой ячейке лежит адрес начала старой памяти
    // mov bx, dx          ; указывает на место где лежит указатель на предыдущюю область памяти в новой памяти 
    //
    // (mov dx, bx         ; сбрасываем накопленный dx до налчала вызова функции
    //  add dx, 100        ; выделяем новую область памяти
    //  call func
    //  {
    //  mov [dx], bx       ; сохраняем указатель на предыдущую область
    //  mov bx, dx         ; сохраняем указатель в новой области на указатель на старую область памяти
    //
    //  spam
    //
    //  mov dx, [bx]
    //  mov bx, dx
    //  } )
    //
    //
    // mov dx, [bx]        ; переносим в dx указатель на предыдущую область памяти; в ячейку 
    // mov bx, dx          ; bx указыает на указатеь на ячейку в которой хранится указатель на предпредыдущую область памяти в кооринатах предыдущейобдасти памяти
    // }

    ast->Adress_next = main_offset_addr;
}


void Asm_ret_for_rek_supp(FILE* fp)
{
    fprintf(fp, "POPREG AX\n");         // AX = ret

////////////////////////////////////// retern regs
    fprintf(fp, "POPREG CX\n");      

    fprintf(fp, "POPREG HX\n");
//////////////////////////////////////

    // // {    support rekursia
    // fprintf(fp, "PUSHM [BX]\n");     //    ; mov dx, [bx]
    // fprintf(fp, "POPREG DX\n");
    // // }

    fprintf(fp, "RET\n");
}


void func_open_space(Le_af leaf, ar_get, int how_much_func, int num_func)
{
    Debug;

    /////leaf == connect;      all params in ->left
    // need new steck

    table_name* new_space = (table_name*)calloc((size_t)ast->max_num, sizeof(table_name));
    AsserT(new_space == NULL, );

    int i_i = 0;
    if(ast->var_ble_tabl.size == 1) //не может быть больше in time init func
    {
        table_name* reserve = stack_pop(&ast->var_ble_tabl);
        for(; i_i < ast->max_num && reserve[i_i].name != NULL; ++i_i)
        {
            new_space[i_i].adress = reserve[i_i].adress;
            new_space[i_i].est_net = reserve[i_i].est_net;
            new_space[i_i].name = strdup(reserve[i_i].name);
            new_space[i_i].size_tabl = reserve[i_i].size_tabl;
            new_space[i_i].num_param = -1;
        }
        stack_push(&ast->var_ble_tabl, reserve);
    }

    //если надо было перекопировали всю предыдущую облась видимости
    //так как мы не можем инициализировать функцию в других места кроме глобальных значит перекопироваться должеон именно глобальный стек если он есть а если есть только 1 стек то он точно глобальный
    
    Le_af cur = leaf->left;
    for(int i = 0; i < how_much_func; ++i) //добавляем новые элементы инициализированные внутри вызова то есть в круглых скобочках
    {
        ast->userz_func[num_func].init_params[i].adress = ast->Adress_next;

        new_space[i_i].adress = ast->Adress_next;
        (ast->Adress_next)++;

        new_space[i_i].est_net = LOCAL;
        new_space[i_i].name = strdup(cur->value.x);

        cur = cur->left;
        ++i_i;
    }

    new_space[0].size_tabl = (size_t)i_i;

    stack_push(&ast->var_ble_tabl, new_space);
}


void Asm_init(Arg_s)
{
    Debug;

    Le_af ass_ignment = leaf->left;
    Le_af val = ass_ignment->left;

    char* name_var = strdup(val->value.x);


    int global_nado = 0; 
    int size_stk = (int)ast->var_ble_tabl.size;

    if(size_stk == 0)
        global_nado = GLOBAL;

    if(size_stk > 1)
        global_nado = LOCAL;

    if(size_stk == 1)
    {
        table_name *chek_gl_li = stack_pop(&ast->var_ble_tabl);

        global_nado = chek_gl_li[0].est_net;

        stack_push(&ast->var_ble_tabl, chek_gl_li);
    }
    //chek global is already

    
    if(size_stk > 0)//if >0 значит просто добавляем элемент с меткой глобала/local in nado
    {
        table_name *add_in_local = stack_pop(&ast->var_ble_tabl);
        int num = (int)add_in_local[0].size_tabl;

        add_in_local[num].name = strdup(name_var); //пополняем стек
        add_in_local[num].est_net = global_nado;
        add_in_local[num].adress = ast->Adress_next;

        (ast->Adress_next)++;

        add_in_local[0].size_tabl = (size_t)(num + 1);
        
        stack_push(&ast->var_ble_tabl, add_in_local); //comeback
    }

    if(size_stk == 0) //new steck
    {
        table_name *new_stk = (table_name*)calloc((size_t)ast->max_num, sizeof(table_name));
        AsserT(new_stk == NULL, );

        new_stk[0].est_net = GLOBAL;
        new_stk[0].size_tabl = 1;
        new_stk[0].name = strdup(name_var);
        new_stk[0].adress = (ast->Adress_next);
        
        (ast->Adress_next)++;

        stack_push(&ast->var_ble_tabl, new_stk);    // size++ here deeper
    }

    free(name_var);

    Asm_expression(fp, ass_ignment, ast);
}


void Asm_ass_ignment(Arg_s)
{
    Debug;

    int is_global = 0;
    char* name = strdup(leaf->left->value.x);
    int adr = where_you_from(ast, name, &is_global);
    if(adr < 0)
    {
        fprintf(stderr, "\nadr < 0) in Asm_ass_ignment(Arg_s) \n\n");
        return ;
    }

    Asm_expression(fp, leaf->right, ast); //in stack val of var

    fprintf(fp, "PUSH %d\n", adr);
    
    if(is_global == LOCAL)
    {
        fprintf(fp, "PUSHREG DX\n");
        fprintf(fp, "ADD\n");
    }

    fprintf(fp, "POPREG EX\n");
    fprintf(fp, "POPM [EX]\n");

    free(name);
}


void Asm_expression(Arg_s)          // types
{
    if(leaf == NULL)
        return ;

    if(leaf->type == Z_NAK && leaf->value.oper == ZAPYTAYA)   
        return ;

    Debug;

    switch (leaf->type)
    {
        case NUMBER:
            fprintf(fp, "PUSH %.0lf\n", leaf->value.num);
        break;

        case VARIA:
            var_printing(fp, leaf, ast);
        break;

        case OPERAT:
            operat_ptinting(fp, leaf, ast);
        break;

        case USER_OPER: //alresdy in use
            func_user_ptinting(fp, leaf, ast);
        break;

        case Z_NAK:
            PR_ASM;
        break;

        default:
            fprintf(stderr, "\n Asm_expression case ruined\n\n");
        break;
    }
}


void var_printing(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    int is_global = 0;
    char* name = strdup(leaf->value.x);
    int adr = where_you_from(ast, name, &is_global);
    if(adr < 0)
    {
        fprintf(stderr, "\nadr < 0) in Asm_ass_ignment(Arg_s) parama netu\n\n");
        return ;
    }

    fprintf(fp, "PUSH %d\n", adr);

    if(is_global == LOCAL)
    {
        fprintf(fp, "PUSHREG DX\n");
        fprintf(fp, "ADD\n");
    }
    
    fprintf(fp, "POPREG EX\n");
    fprintf(fp, "PUSHM [EX]\n");            // param is also in stack 

    free(name);
}


void func_user_ptinting(FILE* fp, Le_af leaf, ar_get)
{
    Debug;

    const char* func_name = leaf->value.us_op;
    int func_id = searching_func(func_name, ast);
    if(func_id == -1)
    {
        fprintf(stderr, "\nfunc_user_ptinting===( func not found)===\n\n");
        return ;
    }

    int num_in_f = (int)ast->userz_func[func_id].size_tabl;  //сколько функция принимает параметров смотрим
    table_name* params = ast->userz_func[func_id].init_params;  //маас сив с данными о нужных функциях

    int i = 0;
    Asm_get_args_(fp, leaf->left, ast, num_in_f, params, &i);


    // if(deep_of_func > 0) // =>> рекурсивный вызов
    // {
    //     fprintf(fp, "PUSHREG BX\n");   // mov dx, bx             ; сбрасываем накопленный dx до налчала вызова функции
    //     fprintf(fp, "POPREG DX\n");
    // }
    // else
    // {
    //     fprintf(fp, "PUSHREG DX\n");   //mov bx, dx              ; сохраняем указатель на текущую область памяти main
    //     fprintf(fp, "POPREG BX\n");
    // }
    fprintf(fp, "PUSHREG DX\n");    // save dx

    fprintf(fp, "PUSHREG DX\
               \nPUSH %d\
               \nADD\
               \nPOPREG DX\n", COUNT_OF_MEM_FOR_FUNC);          // DX = DX + const=25 (how much memory need for func higher)

    fprintf(fp, "CALL :%d\n", ast->userz_func[func_id].adress );

    fprintf(fp, "POPREG DX\n");     // renessans dx

    fprintf(fp, "PUSHREG AX\n");    // push param in stack
}


void Asm_get_args_(FILE* fp, Le_af leaf, ar_get, int num_in_f, table_name* params, int *i)
{
    Debug;

    if(leaf == NULL || *i == num_in_f)
        return ;

    if(leaf->type == OPERAT && leaf->value.oper == ZAPYTAYA)
    {
        // Asm_get_left_args_(fp, leaf->left, ast, params, *i);
        // (*i)++;

        Asm_get_args_(fp, leaf->left, ast, num_in_f, params, i);
        Asm_get_args_(fp, leaf->right, ast, num_in_f, params, i);
    }

    else
    {
        Asm_get_left_args_(fp, leaf, ast, params, *i);
        (*i)++;
    }
}


void Asm_get_left_args_(FILE* fp, Le_af leaf, ar_get, table_name* params, int i)
{
    Debug;

    Asm_expression(fp, leaf, ast);

    fprintf(fp, "PUSH %d\n", params[i].adress);
    fprintf(fp, "PUSH %d\n", COUNT_OF_MEM_FOR_FUNC);
    fprintf(fp, "ADD\n");

    fprintf(fp, "PUSHREG DX\n");
    fprintf(fp, "ADD\n");

    fprintf(fp, "POPREG EX\n");
    fprintf(fp, "POPM [EX]\n");
}


int where_you_from(ar_get, char* name_var, int* is_global)          // just check stack for param // return address where live param
{
    table_name* reserve_stk = stack_pop(&ast->var_ble_tabl);
    for(int i = 0; i < ast->max_num && reserve_stk[i].name != NULL; ++i)
    {
        if(strcmp(name_var, reserve_stk[i].name) == 0)
        {
            *is_global = reserve_stk[i].est_net;
            stack_push(&ast->var_ble_tabl, reserve_stk);
            fprintf(stderr, "\nparam <%s> is found\n", name_var);
            return reserve_stk[i].adress;
        }
    }
    fprintf(stderr, "\nparam <%s> NOT FOUND\n", name_var);
    stack_push(&ast->var_ble_tabl, reserve_stk);

    return -1;
}


int searching_func(const char* func_name, ar_get)
{
    for(int i = 0; i < ast->max_func_num && ast->userz_func[i].name != NULL; ++i)
        if(strcmp(func_name, ast->userz_func[i].name) == 0)
            return i;

    return -1;
}
