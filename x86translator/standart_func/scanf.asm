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
