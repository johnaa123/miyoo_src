.text
.align 4

.global spend_cycles @ c

spend_cycles:
    mov     r0, r0, lsr #2  @ 4 cycles/iteration
    sub     r0, r0, #2      @ entry/exit/init
.sc_loop:
    subs    r0, r0, #1
    bpl     .sc_loop

    bx      lr
