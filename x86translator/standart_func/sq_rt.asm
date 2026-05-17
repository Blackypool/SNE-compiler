jmp skip_init_sq_rt_func
;===============sq_rt===============ABI===============

;   Entry:      1param = where to take the sqrt from
;   Exit:       rax = sqrt(rdi)
;   Expected:   free rax + align stack
;   Destr:      xmm0, rdi, rax

;=====================================================

nop
nop
nop
nop
nop

sq_rt:
;{
    cvtsi2sd xmm0, rdi
    sqrtsd xmm0, xmm0
    cvtsd2si eax, xmm0

    ret
;}
skip_init_sq_rt_func:
;_____________________________________________________