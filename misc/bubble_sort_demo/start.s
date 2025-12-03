    .section .reset,"ax"
    .globl _start
_start:
    la    $sp, _stack
    la    $t0, _sbss
    la    $t1, _ebss
clear_bss_loop:
    bge   $t0, $t1, clear_bss_done
    sw    $zero, 0($t0)
    addiu $t0, $t0, 4
    j     clear_bss_loop
clear_bss_done:

    la    $t0, _idata
    la    $t1, _sdata
    la    $t2, _edata
copy_data_loop:
    bge   $t1, $t2, copy_data_done
    nop
    lw    $t3, 0($t0)
    sw    $t3, 0($t1)
    addiu $t0, $t0, 4
    addiu $t1, $t1, 4
    j     copy_data_loop
    nop
copy_data_done:

    jal   main
    nop
hang:
    b hang
    nop