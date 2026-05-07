
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

              ;[  ]
       sub rsp, 8        ; massiva for senyora if / while

     mov eax, [rbp + 16 + 0]       ; take [1] param = <num> for func from stack
     push rax                       ; var printing

     mov eax, dword ptr [rel one]     ; global param <one> takes from label

     push rax
       pop rcx
       pop rax

       cmp rax, rcx
jg label_0
jmp else_end_label_0

; if:
label_0:


     mov eax, [rbp + 16 + 0]       ; take [1] param = <num> for func from stack
     push rax                       ; var printing

     mov eax, dword ptr [rel one]     ; global param <one> takes from label

     push rax
       pop rcx
       pop rax
         sub eax, ecx
       push rax

       pop rax                ; init ->
       mov [rbp - 0], eax    ; local func param <delta> eat from stack mem

     mov eax, dword ptr [rel delta]     ; global param <delta> takes from label

     push rax
call fact_func

pop rax         ; забрали ret
add rsp, 8        ; skip push params 
push rax            ; вернули ret in stack

     mov eax, [rbp + 16 + 0]       ; take [1] param = <num> for func from stack
     push rax                       ; var printing
       pop rcx
       pop rax
         imul eax, ecx
       push rax
       pop rax
       mov [rbp + 16 + 0], eax       ; place to [1] param = <num> for func from stack

     mov eax, [rbp + 16 + 0]       ; take [1] param = <num> for func from stack
     push rax                       ; var printing
ret
else_end_label_0:

              ;[  ]
       sub rsp, 0        ; massiva for senyora if / while

     mov eax, [rbp + 16 + 0]       ; take [1] param = <num> for func from stack
     push rax                       ; var printing

     mov eax, dword ptr [rel zero]     ; global param <zero> takes from label

     push rax
       pop rcx
       pop rax

       cmp rax, rcx
jl label_1
jmp else_end_label_1

; if:
label_1:


     mov eax, dword ptr [rel one]     ; global param <one> takes from label

     push rax

     mov eax, dword ptr [rel two]     ; global param <two> takes from label

     push rax
       pop rcx
       pop rax
         sub eax, ecx
       push rax
ret
else_end_label_1:

     mov eax, dword ptr [rel one]     ; global param <one> takes from label

     push rax
ret

;}
fact_func_skip_init:

;_____________________________________________________


   jmp skip_global_label_need
need:            ; init ->
	.long   0
   skip_global_label_need:



call scan

       pop rax
       mov dword ptr [rel need], eax

   jmp skip_global_label_samFactor
samFactor:            ; init ->
	.long   0
   skip_global_label_samFactor:


     mov eax, dword ptr [rel need]     ; global param <need> takes from label

     push rax
call fact_func

pop rax           ; забрали ret
add rsp, 8        ; skip push params 
push rax          ; вернули ret in stack
       pop rax
       mov dword ptr [rel samFactor], eax


call printf what about params?+ align

HLT