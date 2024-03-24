section .text   
    bits 64
    default rel ; to handle address relocation
    global asm_domath

asm_domath:
    
    ; Save callee-saved registers if needed
    push rbp
    mov rbp, rsp

    MULSS xmm0, xmm1  
    ADDSS xmm0, xmm2   

    MOVSS [rsp-8], xmm0 
    ;MOVSS [rdi], xmm0

    ; Clean up and return
    ; mov rsp, rbp
    pop rbp
    ret
