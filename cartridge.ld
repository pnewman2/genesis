SECTIONS
{
    /* Cartridge ROM */
    . = 0;
    .vector : { vector.o(.text) }
    .rodata : { *(.rodata) }

    /* RAM area */
    . = 0xff0000;
    /* FIXME: .data needs to be copied */
    .data : { *(.data) }
    .bss : { *(.bss) }

}