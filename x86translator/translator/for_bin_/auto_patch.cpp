#include "auto_patch.h"

struct includes_func_bytes
{
    int e_num;

    unsigned char start_2_bytes[MAX_CMP_SYMB];

    int num_in_JT;

    uint32_t last_offset;
};

struct includes_func_bytes for_cmp[] = {

    {PRINT_F,   {0x57, 0x41},   0,  0x00},      // если ноль оастается то не заполняем
    {SCAN_C ,   {0x48, 0x83},   1,  0x00},
    {SQRT_C ,   {0xF2, 0x48},   2,  0x00},
    {POW_C  ,   {0xB8, 0x01},   3,  0x00},
    {DRAW_C ,   {0x57, 0xB8},   4,  0x00},

};
size_t size_of_include_func = sizeof(for_cmp) / sizeof(for_cmp[0]);


void auto_patcher_bin(const char* name_of_file)
{
    char finish_name[128] = {0};
    snprintf(finish_name, sizeof(finish_name), "%s.bin", name_of_file);
    //////////////////////////////////////////////

    size_t number_char = number__file(finish_name);

    FILE* fp = fopen(finish_name, "r+b");
    AsserT(fp == NULL, file_errorr, );

    /////////////////////////////////////////////////////////////////////
    unsigned char mid_buf[NUM_NOP] = {0};
    unsigned char nop_cmp[NUM_NOP] = {0x90, 0x90, 0x90, 0x90, 0x90}; // тоже массив чтобы в векторы

    for(size_t i = 0; i < number_char; i++)
    {
        fseek(fp, 1, SEEK_CUR);
        fread(&mid_buf, 5, 1, fp);      // через каждый 1 байт читаем 5 и ищем 5 nop

        check_nop_nop(fp, mid_buf, nop_cmp);

        fseek(fp, -5, SEEK_CUR);        // откатываем сдвиг fread только если не нопы, тк если нашли нопы значит надо как раз после них и прочитать
    }
    fclose(fp);
    /////////////////////////////////////////////////////////////////////
    // теперь ищем 52+i и заменяем на нужное смещение до вызова инклуд функции // можно целиком всю табличку заполнять

    fp = fopen(finish_name, "r+b");
    AsserT(fp == NULL, file_errorr, );

    for(size_t ik = 0; ik < size_of_include_func; ik++)
    {
        //////////////////////////////////////////////
        long index = for_cmp[ik].num_in_JT;
        long need_found = CONST_FOR_JT + index;
        long use_rel_ofst = START_OF_JT + 5 + 5 * index;
        long need_add = for_cmp[ik].last_offset - use_rel_ofst;
        //////////////////////////////////////////////

        uint32_t middle = 0;
        for(long i = 0; i < 0xA0; i++)      // < 0xA0 тк джамп таблица всегда выше
        {
            fseek(fp, 1, SEEK_CUR);         // next byte

            fread(&middle, 4, 1, fp);       // через каждый 1 байт читаем 4 и ищем 52+i
            fseek(fp, -4, SEEK_CUR);        // откатываем сдвиг fread

            if(middle == need_found)
            {
                fwrite(&need_add, 1, 4, fp);
                break;
            }
        }
    }

    fclose(fp);
}


void check_nop_nop(FILE* fp, unsigned char *mid_buf, unsigned char *nop_cmp)
{
    for(int k = 0; k < NUM_NOP; k++)
    {
        if(k == (NUM_NOP - 1) && mid_buf[k] == nop_cmp[k])
        {
            fprintf(stderr, "last nop found = %d\n\n\n", k);
            search_func(fp);
            break;
        }

        if(mid_buf[k] == nop_cmp[k])
        {
            fprintf(stderr, "nop found = %d\n\n", k);
            continue;
        }
        else break; // если не 5 нопов улетаем сразу 
    }
}


void search_func(FILE* fp)  // заполняем таблицу нужными данными
{
    uint32_t posi_tion = (uint32_t)ftell(fp);

    unsigned char cmp_incl[MAX_CMP_SYMB] = {0};
    fread(cmp_incl, 1, MAX_CMP_SYMB, fp);
    for(size_t i = 0; i < size_of_include_func; i++)
    {
        if(cmp_incl[0] == for_cmp[i].start_2_bytes[0] && cmp_incl[1] == for_cmp[i].start_2_bytes[1])
        {
            fprintf(stderr, "нашли функцию = %d\n\n", for_cmp[i].e_num);

            for_cmp[i].last_offset = posi_tion;
            fprintf(stderr, "posi in file = %x\n\n", for_cmp[i].last_offset);
            return ;
        }
    }

    fprintf(stderr, "nop here^ but Func in struct is not found\n\n");
}


////////////////////CH_MOD////////////////////////////
void chmod_bin_file(const char* nameof_file)
{
    char apple_script[120] = {0};
    snprintf(apple_script, sizeof(apple_script), "chmod +x %s.bin", nameof_file);
    system(apple_script);
}
//////////////////////////////////////////////////////