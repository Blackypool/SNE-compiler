FLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations \
		-Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body \
		-Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
		-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls \
		-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default \
		-Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers \
		-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
		-fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

FILE_S = translator/do_and_crash.cpp translator/AST_to_asm.cpp translator/logical.cpp translator/user_insert.cpp translator/emit_y.cpp
READ_TREE = translator/read_tree/dump_gr.cpp translator/read_tree/read_tree.cpp
STACK_FUNC = translator/stack_func/constructor.cpp translator/stack_func/prAverti.cpp translator/stack_func/push_pop.cpp

all:
	g++ $(FLAGS) $(FILE_S) $(READ_TREE) $(STACK_FUNC) translator/M_X86.cpp

# readelf -a facto_rial.bin
# chmod +x facto_rial.bin
# ./facto_rial.bin


# ./a.out facto_rial
# ./a.out prime_kvadr


# nasm -f elf64 A_skompile_result/facto_rial.asm -o A_skompile_result/facto_rial.o
# x86_64-elf-ld -o facto_rial A_skompile_result/facto_rial.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./facto_rial

# nasm -f elf64 A_skompile_result/prime_kvadr.asm -o A_skompile_result/prime_kvadr.o
# x86_64-elf-ld -o prime_kvadr A_skompile_result/prime_kvadr.o
# docker run -it --platform linux/amd64 -v "$(pwd)":/workspace -w /workspace ubuntu:latest ./prime_kvadr


# nasm -f bin pow_func.asm -o pow_func.bin
# nasm -f bin printf.asm -o printf.bin
# nasm -f bin scanf.asm -o scanf.bin
# nasm -f bin sq_rt.asm -o sq_rt.bin

# wc -c < sq_rt.bin

# x/20i $rip-10