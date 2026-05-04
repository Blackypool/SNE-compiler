#include "constructor.h"

void lego_stack(struct stk* data, ssize_t capacity)
{
    AsserT(data == NULL, give_null_ptr, );
    AsserT(capacity <= 0, syntax_err,   );
    
    data->capacity = capacity;
    defence_of_capacity(data, __LINE__, __FILE__);

    data->stack = (tip_lac*) calloc((size_t)data->capacity + 2, sizeof(tip_lac));
    yadro_li_you(data, __LINE__, __FILE__);


    data->Kana_ = 914;
    data->_reikA = 419;

    data->size = 1;

    double_kanar(data);
}


void clear_stk(struct stk* data)
{
    AsserT(data == NULL, give_null_ptr, );

    for(ssize_t i = 0; i < data->size; i++)
        free(data->stack[i]);

    data->size = 0;
    data->capacity = 0;

    data->Kana_ = 0;
    data->_reikA = 0;

    free(data->stack);
}
