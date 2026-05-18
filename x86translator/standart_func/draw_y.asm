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