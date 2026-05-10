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

        call sec_printf

        ; renessans stack
        return_0:

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
        


    ;===============sec_printf================my_style====
    ;               steck par r11-8        regs par r12-8         xxm par r13-8
    ;   Entry:      [BP + 32 + r11]   //   [BP - 40 + r12]   //   [BP - 104 + r13]   //   rdi [BP + 24]
    ;   Exit:       true printf
    ;   Expected:   Entry
    ;   Destr:      uuu its quastion to not me br..

    ;=====================================================

    sec_printf:
    ;{
        mov rax, rcx    ; renessans al for xmm regs

        ; defolt regs
        ;{
            mov rdi, [rbp + 24]
        
            mov r9,  [rbp - 8 ]

            mov r8,  [rbp - 16]

            mov rcx, [rbp - 24]

            mov rdx, [rbp - 32]

            mov rsi, [rbp - 40]
        ;}

        cmp al, 0
        je steck_params
        ; xmmN renessans
        ;{
            movq  xmm7, [rbp - 48 ]

            movq  xmm6, [rbp - 56 ]

            movq  xmm5, [rbp - 64 ]

            movq  xmm4, [rbp - 72 ]

            movq  xmm3, [rbp - 80 ]

            movq  xmm2, [rbp - 88 ]

            movq  xmm1, [rbp - 96 ]

            movq  xmm0, [rbp - 104]
        ;}

        cmp r11, 8
        jl skip_stack_params

        ; heal offset of stack
        ;{
            mov rbx, r11
            and rbx, 0xF
            cmp rbx, 0
            je check_stack_16_r11good           ; r11 // 16

            ; r11 !// 16
            ;{
                mov rbx, rsp
                and rbx, 0xF
                cmp rbx, 0
                jne steck_params                ; r11 and rsp ! // 16 => good (-1)(-1) = +

                sub rsp, 8
                sub r11, 8
                jmp steck_params
            ;}

            ; r11 // 16
            check_stack_16_r11good:
            ;{
                mov rbx, rsp
                and rbx, 0xF
                cmp rbx, 0
                je steck_params                 ; r11 and rsp // 16 => all good

                sub rsp, 8
                sub r11, 8
                jmp steck_params
            ;}
        ;}

        steck_params:
        ;{
            cmp r11, 0
            jl skip_stack_params

            mov rbx, [rbp + 40 + r11]
            push rbx
            sub r11, 8

            jmp steck_params
        ;}
        skip_stack_params:

        ; second chance for stack renessans 16
        ;{
            mov rbx, rsp
            and rbx, 0xF
            cmp rbx, 0
            je kratno_all_good

            sub rsp, 8
        ;}
        kratno_all_good:

        call printf wrt ..plt

        jmp return_0
    ;}

    ;_____________________________________________________



    ;======             c   d    f    o             ======

    ;   Entry:      r9 = param && [DX] = where save  
    ;   Exit:       save what give r9+ax --> in [DX]
    ;   Expected:   Entry
    ;   Destr:      ax, dx, r9

    ;=====================================================

    c_char:
    ;{
        mov rax, r9
        mov byte [rdx], al
        inc rdx
            
        jmp break_
    ;}

    ;=====================================================

    s_string:
    ;{
        while_s_sl0:
            mov al, [r9]

            cmp al, 0
            je break_

            mov byte [rdx], al
            inc r9
            inc rdx

            jmp while_s_sl0

                
        jmp break_
    ;}

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

    f_float:
    ;{
        movq xmm0, r9               ; xmm0 = original just copy bites
        jmp check_NAN_Inf
        
        not_anomaly:
        pop r9

        ; convert усечение Scalar d 2 Signed Integer
        cvttsd2si rax, xmm0         ; AX = int
        mov r9, rax

        ; int part
        ;{
            mov r8, 2222                ; flag for ret here from dec
            push rax
            call d_decimal              ; printf int
            pop rax
            
            mov byte [rdx], '.'
            inc rdx
        ;}

        ; delete int
        ;{  cvttsd2si^-1 
            cvtsi2sd xmm1, rax          ; xmm1 = int

            subsd xmm0, xmm1            ; xmm0 = after point part
        ;}

        ; after point --> int 
        ;{
            mov rax, 1000000
            cvtsi2sd xmm1, rax

            mulsd xmm0, xmm1            
            cvttsd2si r9, xmm0

            mov r8, 1111            ; flag for first zero_s
            call d_decimal
        ;}

        mov r8, 12                  ; change r8 for not confuse defolt dec
        jmp break_
    ;}

    ;=====================================================

    check_NAN_Inf:
    ;{
        push r9
        
        mov rax, 0x7FF0000000000000         ;mask for chek anomaly
        and r9, rax
        cmp r9, rax
        jne not_anomaly

        pop r9
        push r9
        shl r9, 12      ; go out of exp
        cmp r9, 0
        je inf_find
        
        ;NAN_moment
        ;{
            inc rdi                         ; next after inf

            mov byte [rdx], 'N'
            inc rdx
            mov byte [rdx], 'A'
            inc rdx
            mov byte [rdx], 'N'
            inc rdx

            pop r9          ; renessans stack_chik
            pop rax         ; eat ret from jump_table for not disbalance stack

            jmp while_sl_zero
        ;}


        inf_find:
        ;{
            inc rdi                         ; next after inf

            pop r9                          ; renessans stack
            mov rax, 0xFFF0000000000000     ; check first 1 bit for -INF

            and r9, rax
            cmp r9, rax
            jne minus_not_need

            mov byte [rdx], '-'
            inc rdx

        minus_not_need:

            mov byte [rdx], 'I'
            inc rdx
            mov byte [rdx], 'N'
            inc rdx
            mov byte [rdx], 'F'
            inc rdx

            pop rax         ; eat ret from jump_table for not disbalance stack

            jmp while_sl_zero
        ;}
    ;}

    ;=====================================================

    b_binary:
    ;{
        push 1
        push 63
        push 1
        push 'b'

        call percent_pow_two

        jmp break_
    ;}

    ;=====================================================

    o_octagon:
    ;{
        push 7
        push 63
        push 3
        push 'o'

        call percent_pow_two

        jmp break_
    ;}

    ;=====================================================

    x_hexagon:
    ;{
        push 0xF
        push 60
        push 4
        push 'x'

        call percent_pow_two

        jmp break_
    ;}

    ;======               b   o   x                 ======

    ;   Entry:      mask, shr/l, degree of two, letter
    ;   Exit:       save what give r9 --> in [DX]
    ;   Expected:   r9 = param && [DX] = where save  
    ;   Destr:      ax, r9, rdx

    ;=====================================================

    percent_pow_two:
    ;{  
        ;{  go params
            push rdi        
            mov rdi, rsp

            push r14        
            push r8         
            push rcx       
            push rbx        

            push r15        ; flag for only one zero
            push rsi        ; save mask for same task)

            mov r14, [rdi + 16]     ; letter     -->     next flag for first zero_s
            mov r8,  [rdi + 24]     ; degree of two (16 = 4; 8 = 3; 2 = 1)
            mov rcx, [rdi + 32]     ; sh r/l
            mov rbx, [rdi + 40]     ; mask
            mov rsi, rbx
        ;}

        ;0|x_b_o|
            mov byte [rdx], '0'
            inc rdx

            mov byte [rdx], r14b
            inc rdx

            mov r15, rdx
            xor r14, r14    ; flag for first zero_s
        ;      

        while__force:
        ;{
            mov rbx, rsi
            mov rax, r9

            shr rax, cl
            cmp al, 10
            jb num_need

            sub al, 10
            add al, 'A'

            jmp next_pow_xbo

        num_need:
            cmp al, 0
            je zero_skipper_xbo

        no_skipa_xbo:
            add al, '0'

        next_pow_xbo:
            mov byte [rdx], al
            inc rdx
            inc r14             ; ruin flag for not skeep zero_s

        need_skip_xbo:
            ; burn hight bit
                mov rax, r9
                shl rbx, cl
                and rax, rbx
                sub r9, rax
            ;

            sub rcx, r8
            cmp rcx, 0
            jge while__force

            jmp mb_only_one_zero

                zero_skipper_xbo:
                    cmp r14, 0
                    je need_skip_xbo

                    jmp no_skipa_xbo
        ;}

    mb_only_one_zero:
        ;{
            cmp rdx, r15
            jne end_of_xbo

            mov byte [rdx], '0'
            inc rdx
        ;}

    end_of_xbo:
        ;{ cleaning
            pop rsi
            pop r15

            pop rbx
            pop rcx
            pop r8
            pop r14

            pop rdi
        ;}

        ret 32
    ;}

    skip_init_of_M_printf_s:

    ;_____________________________________________________
    section .data
    ;{
        ; buff with end of printfing
        what_prntf times 1024 db 0

        ; jt:
        align 8
    Springboard:
        dq b_binary      ; 98
        dq c_char        ; 99
        dq d_decimal     ; 100
        dq not_def_spec  ; 101
        dq f_float       ; 102
        times ('l' - 'f' -1) dq not_def_spec
        dq f_float       ; doubble = float
        times ('o' - 'l' -1) dq not_def_spec
        dq o_octagon     ; 111
        times ('s' - 'o' -1) dq not_def_spec
        dq s_string      ; 115
        times ('x' - 's' -1) dq not_def_spec
        dq x_hexagon     ; 120
    ;}
    ;_____________________________________________________
    section .text
;}