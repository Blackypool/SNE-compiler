#include "../M_compile.h"


int main(int argc, char* argv[])
{
    AsserT(argc < 2, syntax_err, 0);

    struct A_S_T ast = {0};
    
    Le_af root = merger(argv[1], &ast);
////////////////////////////
    
    int err = asm_main(&ast);
    AsserT(err < 0, err, 0);

////////////////////////////
    fell_a_root(&root);

    return 0;
}
