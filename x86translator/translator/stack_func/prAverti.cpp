#include "prAverti.h"

void pr_in_tf(struct stk* data, int /*num_line*/, const char* /*file_n*/)
{
    ssize_t schetchik = data->capacity;

    FILE *fp = fopen("dump_graph/OTLADCHIK.txt", "w");
    
      
    while(schetchik > 0)
    {
        schetchik--;

        tip_lac shtrih = data->stack[schetchik];
        fprintf(fp, "stack[%zd] = (%d)\n", schetchik, shtrih);
    }

    fclose(fp);
}


void yadro_li_you(struct stk* data, int num_line, const char* file_n)
{
    uintptr_t adr_ptr = (uintptr_t)(data->stack);

    if ((adr_ptr < 0x1000) || (adr_ptr > 0x00007FFFFFFFFFFF))
    {
        printf(ANSI_COLOR_RED "\nNO_NO_NO Mr. fish, you go out is pamyat yadrad%s:%d\n", file_n, num_line);

        if(data->stack != NULL)
            free(data->stack);

        data->stack = (tip_lac*) calloc((size_t)(data->capacity), sizeof(int));

        if(data->stack == NULL)
            perror("Unluck - x2\n");
    }
}



void defence_of_capacity(struct stk* data, int num_line, const char* file_n)
{
    ssize_t schetchik = data->capacity;

    if(llabs(data->capacity) >= pow(2, 62))
    {
        printf("To big capacity = (%zd) %s:%d\n", data -> capacity, file_n, num_line);
        schetchik = llabs(data->capacity) / 4;
        data -> capacity = schetchik;
    }


    if(data->capacity == 0)
    {
        printf(ANSI_COLOR_RED "Capacity cant be zero\nBut I can fix it%s:%d\n", file_n, num_line);
        (data->capacity) = 1;
    }


    if(schetchik < 0)
    {
        printf(ANSI_COLOR_RED "\nCAPACITY STRONGER THAT YOU THINK /(modul of capasity)%s:%d\n", file_n, num_line);

        schetchik = labs(data->capacity) +2;
        data->capacity = schetchik + 4;
    }
}


void def_size_capacity(struct stk* data, int /*num_line*/, const char* /*file_n*/)
{
    ssize_t def_capacity = (data->capacity);

    if((data->size) >= (data->capacity))
    {
        printf("size >= capacity((( realok use, all right\n");
        while((data->size) >= def_capacity)
            def_capacity = def_capacity * 2;

        tip_lac* def_stack = (tip_lac*) realloc(data->stack, ((unsigned long)def_capacity + 2) * sizeof(tip_lac));
        
        data->stack = def_stack;

        data->capacity = def_capacity;
    }
}


void double_kanar(struct stk* data)
{
    AsserT((data->Kana_ != 914) || (data->_reikA != 419), syntax_err, );
}
