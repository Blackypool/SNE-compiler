section .text


global _start
_start:


;_________INIT_two______________

      mov rax, 2      ; push number
      push rax

      pop rax

   lea rcx, [rip + two]        ; global param <two> takes from label
   mov dword ptr [rcx], eax
;_________________________________________


;_________INIT_zero______________

      mov rax, 0      ; push number
      push rax

      pop rax

   lea rcx, [rip + zero]        ; global param <zero> takes from label
   mov dword ptr [rcx], eax
;_________________________________________


;_________INIT_one______________

      lea rcx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rcx]

      push rax

      lea rcx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rcx]

      push rax
      pop rcx
      pop rax
      cdq
      idiv ecx
      push rax
      pop rax

   lea rcx, [rip + one]        ; global param <one> takes from label
   mov dword ptr [rcx], eax
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

      lea rcx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rcx]

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

      lea rcx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rcx]

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

      lea rcx, [rip + zero]        ; global param <zero> takes from label
      mov eax, dword ptr [rcx]

      push rax

   pop rcx
   pop rax

   cmp rax, rcx
   jl    label_1
 ;____________________
 jmp else_end_label_1
 label_1:        ; if:


      lea rcx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rcx]

      push rax

      lea rcx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rcx]

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


      lea rcx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rcx]

      push rax

   pop rax

   mov rsp, rbp
   pop rbp

    ret

;}
fact_func_skip_init:

;_____________________________________________________


;_________INIT_need______________


 call scan

      pop rax

   lea rcx, [rip + need]        ; global param <need> takes from label
   mov dword ptr [rcx], eax
;_________________________________________


;_________INIT_samFactor______________

 ;_______FUNC_USE_____
      lea rcx, [rip + need]        ; global param <need> takes from label
      mov eax, dword ptr [rcx]

      push rax
   call fact_func
   add rsp, 8        ; skip push params 
   push rax            ; вернули ret in stack
 ;____________________
      pop rax

   lea rcx, [rip + samFactor]        ; global param <samFactor> takes from label
   mov dword ptr [rcx], eax
;_________________________________________



 call printf what about params?+ align



; _______________________
;return 0;
mov rax, 60
mov rdi, 0
syscall

.section .data

two:
	.long   2

zero:
	.long   0

one:
	.long   0

need:
	.long   0

samFactor:
	.long   0
