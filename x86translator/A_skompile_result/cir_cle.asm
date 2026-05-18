bits 64
section .text


global _start
_start:

jmp skip_jmp_table
A_1_1_1:
   jmp M_Scanf
A_4_4_4:
   jmp draw_y
skip_jmp_table:



jmp draw_circle_skip_init

;===============draw_circle===============cdecle=======                
                
;   Entry:      (1) param                
;   Exit:       ret in stack                
;   Expected:   nothing                
;   Destr:      ax, may be cx                
                
;=====================================================

draw_circle:
;{
   push rbp
   mov rbp, rsp

sub rsp, 48

 ;_________INIT_x_max______________

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing

pop rax
mov [rbp - 8 - 0], rax    ; init go ta kadr
 ;_________________________________________


 ;_________INIT_y_max______________

      mov eax, [rbp - 8 - 0]        ; local param <x_max> eat from stack mem

      push rax

pop rax
mov [rbp - 8 - 8], rax    ; init go ta kadr
 ;_________________________________________


 ;_________INIT_x______________

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing
      pop rax
      mov rcx, 0
      sub rcx, rax
      push rcx


pop rax
mov [rbp - 8 - 16], rax    ; init go ta kadr
 ;_________________________________________


 ;_________INIT_y______________

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing
      pop rax
      mov rcx, 0
      sub rcx, rax
      push rcx


pop rax
mov [rbp - 8 - 24], rax    ; init go ta kadr
 ;_________________________________________


 ;_________INIT_radius2______________

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing
      pop rcx
      pop rax
      imul eax, ecx
      push rax

pop rax
mov [rbp - 8 - 32], rax    ; init go ta kadr
 ;_________________________________________

 start_of_while_label_0:

 ;________CMP_________
      mov eax, [rbp - 8 - 24]        ; local param <y> eat from stack mem

      push rax

      mov eax, [rbp - 8 - 8]        ; local param <y_max> eat from stack mem

      push rax

   pop rcx
   pop rax

   cmp eax, ecx

   jle    label_0
 ;____________________

jmp end_of_while_label_0
 label_0:
 start_of_while_label_1:

 ;________CMP_________
      mov eax, [rbp - 8 - 16]        ; local param <x> eat from stack mem

      push rax

      mov eax, [rbp - 8 - 0]        ; local param <x_max> eat from stack mem

      push rax

   pop rcx
   pop rax

   cmp eax, ecx

   jle    label_1
 ;____________________

jmp end_of_while_label_1
 label_1:

 ;_________INIT_dist______________

      mov eax, [rbp - 8 - 16]        ; local param <x> eat from stack mem

      push rax

      mov eax, [rbp - 8 - 16]        ; local param <x> eat from stack mem

      push rax
      pop rcx
      pop rax
      imul eax, ecx
      push rax

      mov eax, [rbp - 8 - 24]        ; local param <y> eat from stack mem

      push rax

      mov eax, [rbp - 8 - 24]        ; local param <y> eat from stack mem

      push rax
      pop rcx
      pop rax
      imul eax, ecx
      push rax
      pop rcx
      pop rax
      add eax, ecx
      push rax

pop rax
mov [rbp - 8 - 40], rax    ; init go ta kadr
 ;_________________________________________

jmp skip_init_draw_y_func
;===============POW_FUNC===============ABI============

;   Entry:      in rdi = need char 
;   Exit:       v_mem
;   Expected:   nothing
;   Destr:      rax, rsi, rdx

;=====================================================

    nop
    nop
    nop
    nop
    nop

draw_y:
;{
    push rdi

    mov     eax, 1
    mov     edi, 1
    mov     rsi, rsp
    mov     edx, 1
    syscall 

    pop rdi        

    ret 
;}
skip_init_draw_y_func:
      mov rax, 32      ; push number
      push rax


      pop rdi
call A_4_4_4   ; draw_y

 ;________CMP_________
      mov eax, [rbp - 8 - 40]        ; local param <dist> eat from stack mem

      push rax

      mov eax, [rbp - 8 - 32]        ; local param <radius2> eat from stack mem

      push rax

   pop rcx
   pop rax

   cmp eax, ecx

   jl    label_2
 ;____________________
 ; else:

      mov rax, 32      ; push number
      push rax


      pop rdi
call A_4_4_4   ; draw_y
 jmp else_end_label_2
 label_2:        ; if:


      mov rax, 35      ; push number
      push rax


      pop rdi
call A_4_4_4   ; draw_y
 else_end_label_2:
    ;______________


      mov rax, 32      ; push number
      push rax


      pop rdi
call A_4_4_4   ; draw_y

      mov eax, [rbp - 8 - 16]        ; local param <x> eat from stack mem

      push rax

      mov rax, 1      ; push number
      push rax

      pop rcx
      pop rax
      add eax, ecx
      push rax
      pop rax

      mov [rbp - 8 - 16], eax        ; local param <x> eat from stack mem

 jmp start_of_while_label_1
 end_of_while_label_1:
    ;______________


      mov rax, 10      ; push number
      push rax


      pop rdi
call A_4_4_4   ; draw_y

      mov eax, [rbp + 8 + 8]        ; take [1] param = <radius> for func from stack
      push rax                       ; var printing
      pop rax
      mov rcx, 0
      sub rcx, rax
      push rcx

      pop rax

      mov [rbp - 8 - 16], eax        ; local param <x> eat from stack mem

      mov eax, [rbp - 8 - 24]        ; local param <y> eat from stack mem

      push rax

      mov rax, 1      ; push number
      push rax

      pop rcx
      pop rax
      add eax, ecx
      push rax
      pop rax

      mov [rbp - 8 - 24], eax        ; local param <y> eat from stack mem

 jmp start_of_while_label_0
 end_of_while_label_0:
    ;______________


      mov rax, 1      ; push number
      push rax


   pop rax

   mov rsp, rbp
   pop rbp

    ret

;}
draw_circle_skip_init:

;_____________________________________________________


;_________INIT_rad______________
jmp skip_init_scan_ff
;===============SCAN_F==============nothing==========

;   Entry:      nothing
;   Exit:       aligned stack + scanf num in ax
;   Expected:   nothing
;   Destr:      ax, di, si, dx, cx, bx

;=====================================================

nop
nop
nop
nop
nop

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

call A_1_1_1   ; M_Scanf

      push rax
      pop rax

   mov [rel rad], eax  ; global param <rad> takes from label;_________________________________________


 ;_______FUNC_USE_____
      mov eax, [rel rad]  ; global param <rad> takes from label
      push rax
   call draw_circle
   add rsp, 8        ; skip push params 
   push rax            ; вернули ret in stack
 ;____________________


; _______________________
;return 0;
mov rax, 60
mov rdi, 0
syscall

section .data

rad:
	dd   0
