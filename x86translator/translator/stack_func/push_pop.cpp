#include "push_pop.h"

#define L_F __LINE__, __FILE__


void stack_push(struct stk* data, tip_lac value)
{
    double_kanar(data);

    defence_of_capacity(data, L_F);
    def_size_capacity(data, L_F);
    

    data->stack[data->size] = value;
    data->size++;
    pr_in_tf(data, L_F);


    double_kanar(data);
}

tip_lac stack_pop(struct stk* data)
{
    AsserT(data == NULL, give_null_ptr, NULL);

    double_kanar(data);

    AsserT(data->size <= 0, syntax_err, NULL);
    
    data->size--;
    tip_lac paramchik = data->stack[data->size];

    pr_in_tf(data, L_F);


    double_kanar(data);

    return paramchik;
}
