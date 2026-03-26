section .multiboot_header
header_start:
    dd 0xe85250d6 ;Мэджик число
    dd 0 ;Архитектура 0 (i386)
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    align 8
    dw 5 ;Тип тэга 5 (Framebuffer)
    dw 0 ;Флаги (0 для заполнения)
    dd 20 ;Размер тега 20 байт
    dd 800 ;Ширина
    dd 600 ;Высота
    dd 32

    ;Энд тэг
    align 8
    dw 0
    dw 0
    dd 8
header_end: