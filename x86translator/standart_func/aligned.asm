jmp skip_init_aligned_stack
;===============ALIGNE===============nothing==========

;   Entry:      rdi = ptr to format  //  rsi = where save scan
;   Exit:       aligned stack + scanf num in ax
;   Expected:   nothing
;   Destr:      rbx, rax, rsp

;=====================================================

aligned_stack: 
;{
    pop rax         ; rax = ret
    
    mov rbx, rsp   
    and rbx, 15     ; 0x0..01111   

    cmp rbx, 0      
    je align_is_true

    sub rsp, 8

 align_is_true: 

    push rax
    ret
;}
skip_init_aligned_stack:
;_____________________________________________________