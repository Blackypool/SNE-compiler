jmp skip_init_POW_FUNC
;===============POW_FUNC===============ABI============

;   Entry:      base = rdi  //  degree = rsi
;   Exit:       rax = rdi^rsi
;   Expected:   free rax + align stack
;   Destr:      rax, rsi, rdi

;=====================================================

nop
nop
nop
nop
nop

pow_func:
;{
    mov     eax, 1

 cycle_in_pow_func:
    cmp rsi, 0
    jle end_of_pow_func

    imul eax, edi
    dec esi
    jmp cycle_in_pow_func

 end_of_pow_func:
        ret
;}
skip_init_POW_FUNC:
;_____________________________________________________