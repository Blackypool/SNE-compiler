bits 64
section .text


extern scanf

global _start
_start:


;_________INIT_two______________

      mov rax, 2      ; push number
      push rax

      pop rax

   lea rcx, [rel two]        ; global param <two> takes from label
   mov dword [rcx], eax
;_________________________________________


;_________INIT_zero______________

      mov rax, 0      ; push number
      push rax

      pop rax

   lea rcx, [rel zero]        ; global param <zero> takes from label
   mov dword [rcx], eax
;_________________________________________


;_________INIT_one______________

      lea rcx, [rel two]        ; global param <two> takes from label
      mov eax, dword [rcx]

      push rax

      lea rcx, [rel two]        ; global param <two> takes from label
      mov eax, dword [rcx]

      push rax
      pop rcx
      pop rax
      cdq
      idiv ecx
      push rax
      pop rax

   lea rcx, [rel one]        ; global param <one> takes from label
   mov dword [rcx], eax
;_________________________________________



jmp fact_func_skip_init

;===============fact_func===============cdecle=======                
                
;   Entry:      (1) param                
;   Exit:       ret in stack                
;   Expected:   nothing                
;   Destr:      ax, may be cx                
                
;=====================================================

fact_func:
;{
   push rbp
   mov rbp, rsp

 ;________CMP_________
      mov eax, [rbp + 8 + 0]        ; take [1] param = <num> for func from stack
      push rax                       ; var printing

      lea rcx, [rel one]        ; global param <one> takes from label
      mov eax, dword [rcx]

      push rax

   pop rcx
   pop rax

   cmp rax, rcx
   jg    label_0
 ;____________________
 jmp else_end_label_0
 label_0:        ; if:


 ;_________INIT_delta______________

      mov eax, [rbp + 8 + 0]        ; take [1] param = <num> for func from stack
      push rax                       ; var printing

      lea rcx, [rel one]        ; global param <one> takes from label
      mov eax, dword [rcx]

      push rax
      pop rcx
      pop rax
      sub eax, ecx
      push rax

   pop rax                ; init ->
   mov [rbp - 8 - 0], eax    ; go to array on stack <delta>
 ;_________________________________________


 ;_______FUNC_USE_____
      mov eax, [rbp - 8 - 0]        ; local param <delta> eat from stack mem

      push rax
   call fact_func
   add rsp, 8        ; skip push params 
   push rax            ; вернули ret in stack
 ;____________________

      mov eax, [rbp + 8 + 0]        ; take [1] param = <num> for func from stack
      push rax                       ; var printing
      pop rcx
      pop rax
      imul eax, ecx
      push rax
      pop rax

      mov [rbp + 8 + 0], eax        ; take [1] param = <num> for func from stack
      mov eax, [rbp + 8 + 0]        ; take [1] param = <num> for func from stack
      push rax                       ; var printing

   pop rax

   mov rsp, rbp
   pop rbp

    ret
 else_end_label_0:
    ;______________


 ;________CMP_________
      mov eax, [rbp + 8 + 0]        ; take [1] param = <num> for func from stack
      push rax                       ; var printing

      lea rcx, [rel zero]        ; global param <zero> takes from label
      mov eax, dword [rcx]

      push rax

   pop rcx
   pop rax

   cmp rax, rcx
   jl    label_1
 ;____________________
 jmp else_end_label_1
 label_1:        ; if:


      lea rcx, [rel one]        ; global param <one> takes from label
      mov eax, dword [rcx]

      push rax

      lea rcx, [rel two]        ; global param <two> takes from label
      mov eax, dword [rcx]

      push rax
      pop rcx
      pop rax
      sub eax, ecx
      push rax

   pop rax

   mov rsp, rbp
   pop rbp

    ret
 else_end_label_1:
    ;______________


      lea rcx, [rel one]        ; global param <one> takes from label
      mov eax, dword [rcx]

      push rax

   pop rax

   mov rsp, rbp
   pop rbp

    ret

;}
fact_func_skip_init:

;_____________________________________________________


;_________INIT_need______________
jmp skip_init_scan_ff
;===============ALIGNE===============nothing==========

;   Entry:      nothing
;   Exit:       aligned stack + scanf num in ax
;   Expected:   nothing
;   Destr:      ax, di, si, dx, cx, bx

;=====================================================

M_Scanf:
;{
    sub rsp, 32                 ; massiva on stack
    
    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 32
    syscall
    
    xor rdx, rdx
    xor rax, rax                ; res
    xor rcx, rcx                ; num in massiva
    mov rbx, 1                  ; sign
    
    cmp byte [rsp], '-'
    jne positive_sc
    mov rbx, -1
    inc rcx

 positive_sc:
 ;{
    mov dl, byte [rsp + rcx]
    cmp dl, '0'
    jl return_0_from_scanf

    cmp dl, '9'
    jg return_0_from_scanf

    sub dl, '0'
    imul eax, 10
    add eax, edx
    
    inc ecx
    jmp positive_sc
 ;}

 return_0_from_scanf:

    imul eax, ebx               ; up sign
    add rsp, 32                 ; free(stack)

    ret
;}
skip_init_scan_ff:
;_____________________________________________________

      lea rdi, [rel format]

      lea rsi, [rel num]

call M_Scanf

      mov eax, dword [rel num]
      push rax
      pop rax

   lea rcx, [rel need]        ; global param <need> takes from label
   mov dword [rcx], eax
;_________________________________________


;_________INIT_samFactor______________

 ;_______FUNC_USE_____
      lea rcx, [rel need]        ; global param <need> takes from label
      mov eax, dword [rcx]

      push rax
   call fact_func
   add rsp, 8        ; skip push params 
   push rax            ; вернули ret in stack
 ;____________________
      pop rax

   lea rcx, [rel samFactor]        ; global param <samFactor> takes from label
   mov dword [rcx], eax
;_________________________________________

;__________INIT_BIG_PRINTF__________
;{    
    ; +-inf // switch_jump_t // float*(-1) // call printf // one func b_o_x

    ;           b   c   d    f    o    s    x   
    ;           98  99  100  102  111  115  120

    ; lldb-20  a.out -> run
    ; nasm -f elf64 -l Float_supp.lst Float_supp.s  ;  ld -s -o a.out Float_supp.o

    ; RIP-relative addressing  //  rip = now command => pie: now + msg = [rel msg]  //  Position Independent Executable = PIE

    jmp skip_init_of_M_printf_s
    ;===============M_printf_s===============cdecle=======

    ;   Entry:      1param = adr of str && p234... for "%"
    ;   Exit:       what u want
    ;   Expected:   what u want
    ;   Destr:      si, ax, dx, r10, r9, di

    ;=====================================================

    M_printf_s:
    ;{
        ;ret                            ; [BP + 32]
        push rdi                        ; [BP + 24]
        push r12                        ; [BP + 16]
        push r13                        ; [BP + 8]
        push rbp                        ; [BP]
        mov rbp, rsp                    ; [BP + 40 + R11] = stack params
        xor r11, r11                    ; R11 = for stack  

    ; defolt params in regs, start from [BP - 8 * 5 + R12]       // r12 <= 8*5
        xor r12, r12    ; r12 for int regs

        push r9         ; be param                      [BP - 8]
        push r8         ; flag for float in dec         [BP - 16]
        push rcx        ; cl = al for next printf       [BP - 24]
        push rdx        ; to save in buff               [BP - 32]
        push rsi        ; use in tempure calc in jt     [BP - 40]
    ;
    cmp al, 0
    je skip_xmm_N    ; float is not exist
    ; float params in regs, start from [BP - 8 * 8 - 40(int params) + R13]        // r13 <= 8*8
    ;______________
        xor r13, r13    ; r13 = for float regs

        movq r8, xmm7
        push r8         ; [BP - 48]
    
        movq r8, xmm6
        push r8         ; [BP - 56]

        movq r8, xmm5
        push r8         ; [BP - 64]

        movq r8, xmm4
        push r8         ; [BP - 72]

        movq r8, xmm3
        push r8         ; [BP - 80]

        movq r8, xmm2
        push r8         ; [BP - 88]

        movq r8, xmm1
        push r8         ; [BP - 96]

        movq r8, xmm0
        push r8         ; [BP - 104]
    ;______________
        
        skip_xmm_N:
    ; prepare_s
        xor r8, r8                      ; flag for not need use float in dec in start before find float
        lea rdx, [rel what_prntf]       ; DX = start of buff with result // relative(относительная) addressing
        mov rcx, rax                    ; al = count for float numbers in regs--> cl
    ;
    while_sl_zero:

        mov al, [rdi]               ; AL = ASCII

        cmp al, '%'                 ; "%" ?
        jne not_percent
    ; check params
            inc rdi                 ; skip %

            mov al, [rdi]

            cmp al, '%'
            je skip_double_percent

            cmp al, 'f'
            je float_args_processing
            
            cmp al, 'l'
            je float_args_processing

            cmp r12, 40                    ; 40 = 5params * 8 sizeof_in_stack
            jl int_regs_processing         ; <=

            stack_params:
            ;{
                mov r9, [rbp + 40 + r11]
                add r11, 8
                
                jmp next_step_jt
            ;}

            int_regs_processing:
            ;{
                mov r9, [rbp - 40 + r12]
                add r12, 8

                jmp next_step_jt
            ;}

            float_args_processing:
            ;{
                cmp r13, 56                     ; 64 - 8 = 8params * 8 sizeof_in_stack
                jg stack_params                 ; >

                ; regs params need
                mov r9, [rbp - 104 + r13]
                add r13, 8

                jmp next_step_jt
            ;}

            skip_double_percent:
            ;{
                inc rdi
                mov [rdx], al
                inc rdx

                jmp while_sl_zero
            ;}

        next_step_jt:

            call switch_jump_t    ; r9 = param && rdx for save in buff
            inc rdi                     ; next symb
            
            jmp while_sl_zero
    ;
    not_percent:

        cmp al, 0                   ; "\0" ?
        je end_time

        mov byte [rdx], al
        inc rdx
        inc rdi                     ; DI = *ptr to come str

        jmp while_sl_zero


    end_time:
        
        ;print buffer:
        push rcx
        push r11
        mov rax, 0x01
        mov rdi, 1
        lea rsi, [rel what_prntf]    ; rsi = start of buff before
        sub rdx, rsi                 ; rdx = fin of buff from while
        syscall
        pop r11
        pop rcx
        ;

        ; renessans stack
        ; return_0:

            mov rsp, rbp
            pop rbp
            pop r13
            pop r12
            pop rdi

        ret
    ;}

    ;_____________________________________________________



    ;===============switch_jump_t=============my_style====

    ;   Entry:      AX = case for switch;       R9 = param      [DX] = where save  
    ;   Exit:       save what give r9+ax --> in [DX]
    ;   Expected:   Entry
    ;   Destr:      si, ax, r9, dx(True)

    ;=====================================================

    switch_jump_t:
    ;{
        movzx rax, byte [rdi]       ; AX = specif // Move with Zero-Extend

        cmp rax, 'b'                ; if al < b
        jl not_def_spec

        cmp rax, 'x'                ; if al > x
        jg not_def_spec

        lea rsi, [rel Springboard]  ; rsi = offset+ip = full adr of jt 

        jmp [rsi + 8 * (rax - 'b')] ; jt

        not_def_spec:
        ;{
            mov byte [rdx], al
            inc rdx

            jmp break_
        ;}

        break_:
            cmp byte [rdi], 'l'
            jne the_break_
            inc rdi

        the_break_:
            ret  
    ;}

    ;_____________________________________________________
        


    ;======             c   d    f    o             ======

    ;   Entry:      r9 = param && [DX] = where save  
    ;   Exit:       save what give r9+ax --> in [DX]
    ;   Expected:   Entry
    ;   Destr:      ax, dx, r9

    ;=====================================================

    d_decimal:
    ;{
        push r9
        push rdi
        push rsi
        push rbx
        push r11
        push r15

        push r9
        ;check -numbers
        ;{
            test r9, r9
            jns plus_number         ; jump if not sign <=> jmp if r9 > 0
            
            mov byte [rdx], '-'

            cmp r8, 1111
            je skip_draw_minus
            inc rdx

            skip_draw_minus:
            pop r9
            neg r9
            jmp next_step_of_dec
        ;}
        plus_number:
        pop r9

        next_step_of_dec:
        mov rax, r9                 ; rax = param because AX need in DIV

        cmp r8, 1111
        jne _default_dec

        mov r8, 2222        ; for ret + not skip zero_s
        mov r11, 16         ; not need skip first zeroes^ because 898989.000001
        mov rsi, 100000     ; only 6 after point

        jmp next_shag_of_dec


    _default_dec:
        mov r11, 1                  ; flag of start zeros
        mov rsi, 10000000000        ; 10znak max

    next_shag_of_dec:
        mov r15, rdx                ; rdx need for DIV ((

        mov rdi, rdx                ; for check only one zero

        whele_for_d:
        ;{
            xor rdx, rdx
            div rsi                 ; rax = rax / rsi; rdx = rax % rsi 
            
            cmp al, 0
            je starting_zero_d

            need_zero_d:
            add al, '0'
            mov [r15], al
            xor r11, r11
            inc r15

        skip_zero_d:
            mov rax, rdx            ; rdx = ostatok
        
            ; div rsi, 10
                push rax                ; save AX
                mov rax, rsi            ; AX = SI
                mov rsi, 10             ; SI = 10 (on what div)
                xor rdx, rdx            ; need DX = 0
                div rsi                 ; AX / SI
                mov rsi, rax            ; SI = AX
                pop rax                 ; renessans of AX
            ;

            cmp rsi, 0
            jg whele_for_d  ; >
            
            jmp endi_of_d
            
            starting_zero_d:
                cmp r11, 1
                je skip_zero_d
                jmp need_zero_d
        ;}

    endi_of_d:
        cmp rdi, r15
        jne tochno_end

        mov byte [r15], '0'
        inc r15

    tochno_end:
        mov rdx, r15

        pop r15
        pop r11
        pop rbx
        pop rsi
        pop rdi
        pop r9

        cmp r8, 2222
        jne break_

        ret
    ;}

    ;=====================================================
    skip_init_of_M_printf_s:

    ;_____________________________________________________
    section .data
    ;{
        ; buff with end of printfing
        what_prntf times 1024 db 0

        ; jt:
        align 8
    Springboard:
        dq not_def_spec      ; 98
        dq not_def_spec        ; 99
        dq d_decimal     ; 100
        dq not_def_spec  ; 101
        dq not_def_spec       ; 102
        times ('l' - 'f' -1) dq not_def_spec
        dq not_def_spec       ; doubble = float
        times ('o' - 'l' -1) dq not_def_spec
        dq not_def_spec     ; 111
        times ('s' - 'o' -1) dq not_def_spec
        dq not_def_spec      ; 115
        times ('x' - 's' -1) dq not_def_spec
        dq not_def_spec     ; 120
    ;}
    ;_____________________________________________________
    section .text
;}
      lea rcx, [rel samFactor]        ; global param <samFactor> takes from label
      mov eax, dword [rcx]

      push rax

      pop rsi
      lea rdi, [rel format]
      xor rax, rax
call M_printf_s



; _______________________
;return 0;
mov rax, 60
mov rdi, 0
syscall

section .data

two:
	dd   2

zero:
	dd   0

one:
	dd   0

need:
	dd   0

samFactor:
	dd   0

format db "%d", 10, 0


section .bss

    num resd 1
