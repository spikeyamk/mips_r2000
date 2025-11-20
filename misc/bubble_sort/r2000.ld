MEMORY {
    ROM       (rx) : ORIGIN = 0x00000000, LENGTH = 2K
    RAM        (r) : ORIGIN = 0x80000000, LENGTH = 128
    RAM_CONST (rw) : ORIGIN = 0x80000400, LENGTH = 1K
}

_stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    .text : ALIGN(4) {
        KEEP(*(.reset))
        *(.text*)
        . = ALIGN(4);
        _etext = .;
    } > ROM

    _idata = LOADADDR(.data);

    .data : ALIGN(4) {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> RAM_CONST

    .bss : ALIGN(4) {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    /DISCARD/ : {
        *(.MIPS.abiflags)
        *(.reginfo)
        *(.pdr)
        *(.comment)
        *(.gnu.attributes)
    }
}

ENTRY(_start)