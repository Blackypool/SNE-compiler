// #include <elf.h>
// #include "emit_y.h"


// void elf_generator(ar_get, FILE* bin_f, int add_include_size)
// {
//     //////////////////////////////////////////////////

//     Elf64_Ehdr endr = {0};
//     Elf64_Phdr ph_dr = {0};
    
//     size_t size_of_ENDR = sizeof(Elf64_Ehdr);
//     size_t size_of_PHDR = sizeof(Elf64_Phdr);
    
//     /////////////////////E_IDENT//////////////////////
//     endr.e_ident[EI_MAG0] = ELFMAG0;                // 0x7f
//     endr.e_ident[EI_MAG1] = ELFMAG1;                // 'E'
//     endr.e_ident[EI_MAG2] = ELFMAG2;                // 'L'
//     endr.e_ident[EI_MAG3] = ELFMAG3;                // 'F'

//     endr.e_ident[EI_CLASS]      = ELFCLASS64;       // num of architecture 32/64
//     endr.e_ident[EI_DATA]       = ELFDATA2LSB;      // littlr-endian
//     endr.e_ident[EI_VERSION]    = EV_CURRENT;       // curent version
//     endr.e_ident[EI_OSABI]      = ELFOSABI_LINUX;   // linux ABI
//     endr.e_ident[EI_ABIVERSION] = 0;

//     for(int i = EI_PAD; i < EI_NIDENT; i++)
//         endr.e_ident[i] = 0;
//     //////////////////////////////////////////////////


//     //////////////////ANOTHER_ENDR////////////////////
//     endr.e_type = ET_EXEC;                          // type of obj file == исполняемый
//     endr.e_machine = EM_X86_64;                     // need architecture
//     endr.e_version = EV_CURRENT;
//     endr.e_entry = STANDART_ADR + size_of_ENDR + size_of_PHDR + SKIP_JMP_FOR_OFFSET_START;
//     endr.e_phoff = size_of_ENDR;                    // start of program headers

//     endr.e_shoff = 0;                               // section header dont needed
//     endr.e_flags = 0;                               // skip for x86
//     endr.e_ehsize = size_of_ENDR;                   // elf size

//     endr.e_phentsize = size_of_PHDR; 
//     endr.e_phnum = 1;                               // only ph

//     endr.e_shentsize = 0;
//     endr.e_shnum = 0;
//     endr.e_shstrndx = 0;
//     //////////////////////////////////////////////////


//     //////////////////////PH_DR///////////////////////
//     ph_dr.p_type = PT_LOAD;
//     ph_dr.p_offset = 0;
//     ph_dr.p_vaddr = STANDART_ADR;
//     ph_dr.p_paddr = ph_dr.p_vaddr;
//     ph_dr.p_filesz = size_of_ENDR + size_of_PHDR + ast->cur_ip + add_include_size;
//     ph_dr.p_memsz = ph_dr.p_filesz;                 // zero segments
//     ph_dr.p_flags = PF_X | PF_W | PF_R;
//     ph_dr.p_align = PAGE_SIZE;                      // размер страницы
//     //////////////////////////////////////////////////

//     fwrite(&endr, 1, size_of_ENDR, bin_f);
//     fwrite(&ph_dr, 1, size_of_PHDR, bin_f);
// }
