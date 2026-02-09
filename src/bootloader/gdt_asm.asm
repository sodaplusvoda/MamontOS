section .rodata
global gdt64
global gdt64_pointer

gdt64:
	dq 0 ;Null Descriptor
.kernel_code: equ $ - gdt64
    	dq (1<<44)|(1<<47)|(1<<43)|(1<<53) ;0x08
.kernel_data: equ $ - gdt64
    	dq (1<<44)|(1<<47)|(1<<41)         ;0x10
.user_data: equ $ - gdt64
    	dq (1<<44)|(1<<47)|(1<<41)|(3<<45) ;0x18
.user_code: equ $ - gdt64
    	dq (1<<44)|(1<<47)|(1<<43)|(1<<53)|(3<<45) ;0x20
.tss: equ $ - gdt64
    	dq 0
    	dq 0
    	dq 0

gdt64_pointer:
    	dw $ - gdt64 - 1
    	dq gdt64

section .text
[bits 64]
global gdt_load

gdt_load:
    	lgdt [rdi]
    
	;Обновление КС (0x08 это селектор кода ядра)
    	push 0x08
	lea rax, [rel .reload_segments]
	push rax
	retfq

.reload_segments:
	mov ax, 0x10 ;Селектор данных ядра
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
    
	;Загрузка TSS
	mov ax, 0x28
	ltr ax
	ret