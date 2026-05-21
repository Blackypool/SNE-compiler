FLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
		-Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body \
		-Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
		-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls \
		-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default \
		-Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers \
		-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
		-fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

FILE_S = translator/do_and_crash.cpp translator/AST_to_asm.cpp translator/logical.cpp translator/user_insert.cpp
READ_TREE = translator/read_tree/dump_gr.cpp translator/read_tree/read_tree.cpp
STACK_FUNC = translator/stack_func/constructor.cpp translator/stack_func/prAverti.cpp translator/stack_func/push_pop.cpp
FOR_BIN = translator/for_bin_/emit_y.cpp translator/for_bin_/auto_patch.cpp # translator/for_bin_/ELF_generator.cpp

all:
	g++ $(FLAGS) $(FILE_S) $(READ_TREE) $(STACK_FUNC) $(FOR_BIN) translator/M_X86.cpp
# translator/for_bin_/ELF_generator.cpp
# readelf -a facto_rial.bin

# chmod +x facto_rial.bin
# chmod +x prime_kvadr.bin
# chmod +x cir_cle.bin
# chmod +x fibo_nachi.bin

# ./facto_rial.bin
# ./prime_kvadr.bin
# ./cir_cle.bin
# ./fibo_nachi.bin


# ./a.out facto_rial
# ./a.out prime_kvadr
# ./a.out cir_cle
# ./a.out fibo_nachi


# nasm -f elf64 A_skompile_result/facto_rial.asm -o A_skompile_result/facto_rial.o
# x86_64-elf-ld -o facto_rial A_skompile_result/facto_rial.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./facto_rial

# nasm -f elf64 A_skompile_result/prime_kvadr.asm -o A_skompile_result/prime_kvadr.o
# x86_64-elf-ld -o prime_kvadr A_skompile_result/prime_kvadr.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./prime_kvadr

# nasm -f elf64 A_skompile_result/cir_cle.asm -o A_skompile_result/cir_cle.o
# x86_64-elf-ld -o cir_cle A_skompile_result/cir_cle.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./cir_cle

# nasm -f elf64 A_skompile_result/fibo_nachi.asm -o A_skompile_result/fibo_nachi.o
# x86_64-elf-ld -o fibo_nachi A_skompile_result/fibo_nachi.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./fibo_nachi


# nasm -f bin pow_func.asm -o pow_func.bin
# nasm -f bin printf.asm -o printf.bin
# nasm -f bin scanf.asm -o scanf.bin
# nasm -f bin sq_rt.asm -o sq_rt.bin 
# nasm -f bin draw_y.asm -o draw_y.bin

# wc -c < sq_rt.bin

# x/20i $rip-10