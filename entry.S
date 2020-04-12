#include "config.h"

#define CPUID_BCM2835  0x410FB767

    .extern _cstart
    .extern _edata, _end
    .extern _cpuid

    .text
    .globl _entry
_entry:
    mrc p15, #0, r12, c0, c0, #0
    ldr r11, =CPUID_BCM2835
    cmp r11, r12
    beq start

    ;@shutdown other cores
    mrc p15, #0, r11, c0, c0, #5
    and r11, r11, #3
    cmp r11, #0
    bne .

start:
    mov sp, #LOADADDR
    push {r0-r2}

    mov r0, #LOADADDR
    mov r1, #BOOTADDR
    ldr r2, =_end
    sub r2, r2, #BOOTADDR
copy:
    ldmia r0!, {r3}
    stmia r1!, {r3}
    sub r2, r2, #4
    cmp r2, #0
    bne copy

    bl trampoline

    ;@zeroify bss
    ldr r0, =_edata
    ldr r1, =_end
    ldr r2, =0x0
zeroify:
    cmp r1, r0
    ble zeroify.1
    stmia r0!, {r2}
    b zeroify
zeroify.1:

	;@must be after zeroifying bss
    ldr r0, =_cpuid
    str r12, [r0]

    bl _cstart
    pop {r0-r2}
    ldr pc, =(LOADADDR)

trampoline:
    sub lr, lr, #(LOADADDR-BOOTADDR)
    bx lr

    .globl ___aeabi_idivmod
___aeabi_idivmod:
    stmfd sp!, {r0, r1, ip, lr}
    bl ___aeabi_idiv
    ldmfd sp!, {r1, r2, ip, lr}
    mul r3, r0, r2
    sub r1, r1, r3
    mov pc, lr
