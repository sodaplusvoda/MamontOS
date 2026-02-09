ENTRY(start)
SECTIONS
{
    . = 1M;
    _kernel_start = .;

    .text : { 
        _text_start = .;
        KEEP(*(.multiboot_header))
        *(.text) 
        _text_end = .;
    }

    .rodata : { 
        _rodata_start = .;
        *(.rodata) 
        _rodata_end = .;
    }

    .data : { 
        *(.data) 
    }

    .bss : { 
        *(.bss) 
        *(COMMON) 
    }

    . = ALIGN(4K);
    _kernel_end = .;
}