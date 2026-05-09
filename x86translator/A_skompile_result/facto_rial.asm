section .text


global _start
_start:


;_________INIT_two______________
   jmp skip_global_label_two
 two:            ; init ->
	.long   2

      mov rax, 2      ; push number
      push rax

   pop rax
   mov dword ptr [rel two], eax
   skip_global_label_two:
;_________________________________________


;_________INIT_zero______________
   jmp skip_global_label_zero
 zero:            ; init ->
	.long   0

      mov rax, 0      ; push number
      push rax

   pop rax
   mov dword ptr [rel zero], eax
   skip_global_label_zero:
;_________________________________________


;_________INIT_one______________
   jmp skip_global_label_one
 one:            ; init ->
	.long   0

      lea rbx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rbx]

      push rax

      lea rbx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rbx]

      push rax
      pop rcx
      pop rax
            idiv eax, ecx
      push rax
   pop rax
   mov dword ptr [rel one], eax
   skip_global_label_one:
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

      lea rbx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rbx]

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

      lea rbx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rbx]

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

      lea rbx, [rip + zero]        ; global param <zero> takes from label
      mov eax, dword ptr [rbx]

      push rax

   pop rcx
   pop rax

   cmp rax, rcx
   jl    label_1
 ;____________________
 jmp else_end_label_1
 label_1:        ; if:


      lea rbx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rbx]

      push rax

      lea rbx, [rip + two]        ; global param <two> takes from label
      mov eax, dword ptr [rbx]

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


      lea rbx, [rip + one]        ; global param <one> takes from label
      mov eax, dword ptr [rbx]

      push rax

   pop rax

   mov rsp, rbp
   pop rbp

    ret

;}
fact_func_skip_init:

;_____________________________________________________


;_________INIT_need______________
   jmp skip_global_label_need
 need:            ; init ->
	.long   0


 call scan

   pop rax
   mov dword ptr [rel need], eax
   skip_global_label_need:
;_________________________________________


;_________INIT_samFactor______________
   jmp skip_global_label_samFactor
 samFactor:            ; init ->
	.long   0

 ;_______FUNC_USE_____
      lea rbx, [rip + need]        ; global param <need> takes from label
      mov eax, dword ptr [rbx]

      push rax
   call fact_func
   add rsp, 8        ; skip push params 
   push rax            ; вернули ret in stack
 ;____________________
   pop rax
   mov dword ptr [rel samFactor], eax
   skip_global_label_samFactor:
;_________________________________________



 call printf what about params?+ align



; _______________________
;return 0;
mov rax, 60
mov rdi, 0
syscall