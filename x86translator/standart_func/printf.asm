;__________INIT_BIG_PRINTF__________
;{    
    jmp skip_init_of_M_printf_s
    ;===============M_printf_s===============cdecle=======

    ;   Entry:      1param = adr of str && p234... for "%"
    ;   Exit:       what u want
    ;   Expected:   what u want
    ;   Destr:      si, ax, dx, r10, r9, di

    ;=====================================================

    nop
    nop
    nop
    nop
    nop

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

    ; prepare_s
        xor r8, r8                      ; flag for not need use float in dec in start before find float
        lea rdx, [rel what_prntf]       ; DX = start of buff with result // relative(относительная) addressing
        lea rdi, [rel format]           ; для бинарника надо
        mov rcx, rax                    ; al = count for float numbers in regs--> cl    // what_prntf
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

        sub al, 'b'

        lea rsi, [rel Springboard]
        movsxd rax, dword [rsi + rax*8]
        add rsi, rax

        jmp rsi

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
;}

section .data
format db "%d", 10, 0
what_prntf times 12 db 0
                align 8
            Springboard:
                dq (not_def_spec - Springboard)  ; 98
                dq (not_def_spec - Springboard)  ; 99
                dq (d_decimal - Springboard)     ; 100

section .text
