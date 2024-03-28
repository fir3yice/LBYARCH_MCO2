section .text   
    bits 64
    default rel ; to handle address relocation
    global asm_domath

asm_domath:
    push rbp
    mov rbp, rsp
    add rbp, 16 ; align stack to 16 bytes

    ;MULSS xmm0, xmm1  
    ;ADDSS xmm0, xmm2   
    ;MOVSS [rsp-8], xmm0 


    ;rcx = n                    0
    ;xmm1 (rdx) = A             8
    ;movss xmm1, [rbp + 8]  ; A
    ; r8 = x[]                  16
    ; r9 = y[]                  24
    mov r10, [rbp + 32]  ; z[]  32
    xor rax, rax; i
    
    
    

    Loop1:
        movss xmm0, [r8 + rax*4]   ; Load x[i]
        movss xmm2, [r9 + rax*4]    ; Load y[i]
        mulss xmm0, xmm1             ; Multiply a and x[i]
        addss xmm0, xmm2             ; Add the result to y[i]
        movss [r10 + rax*4], xmm0     ; Store the result in z[i]
        inc rax
        dec rcx
        cmp rcx, 0
        jnz Loop1

    End:
    pop rbp
    ret
