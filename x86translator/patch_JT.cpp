#include "M_compile.h"

// g++ patch_JT.cpp -o patcher
// ./patcher facto_rial.bin i adr


//////////////FACT//////////////
// scanf  / i = 1       ./patcher facto_rial.bin 1 0x177
// printf / i = 0       ./patcher facto_rial.bin 0 0x21D
////////////////////////////////


//////////////KVADR/////////////
// scanf  / i = 1       ./patcher prime_kvadr.bin 1 0x111
// printf / i = 0       ./patcher prime_kvadr.bin 0 0x51F
// sqrt   / i = 2       ./patcher prime_kvadr.bin 2 0x1EE
////////////////////////////////



//////////////FIBA//////////////
// scanf  / i = 1       ./patcher fibo_nachi.bin 1 0x110
// printf / i = 0       ./patcher fibo_nachi.bin 0 0x1B6
////////////////////////////////



//////////////KRUG//////////////
// scanf  / i = 1       ./patcher cir_cle.bin 1 0x215
// draw   / i = 4       ./patcher cir_cle.bin 4 0x145
////////////////////////////////


int main(int argc, char* argv[])
{
    AsserT(argc < 4, syntax_err, 0);

    FILE* fp = fopen(argv[1], "r+b");
    AsserT(fp == NULL, file_errorr, 0);

    ///////////////////////
    long index = strtol(argv[2], NULL, 10);

    long need_found = 0x52 + index;

    long use_rel_ofst = 0x7D + 5 + 5 * index;

    long need_add = strtol(argv[3], NULL, 16) - use_rel_ofst;
    ///////////////////////

    uint32_t mid_buf = 0;
    for(long i = 0; i < 0xA0; i++)      // < 0xA0 тк джамп таблица всегда выше
    {
        fseek(fp, 1, SEEK_CUR);         // next byte

        fread(&mid_buf, 4, 1, fp);      // через каждый 1 байт читаем 4 и ищем 52+i
        fseek(fp, -4, SEEK_CUR);        // откатываем сдвиг fread

        if(mid_buf == need_found)
        {
            fwrite(&need_add, 1, 4, fp);
            break;
        }
    }

    fclose(fp);
    return 0;
}
