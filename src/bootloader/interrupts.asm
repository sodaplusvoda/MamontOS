[bits 64]

extern exception_handler
extern isr_handler

%macro no_error_code_stub 1
global exception_stub_%1
exception_stub_%1:
	push 0
	push %1 ;Номер прерывания
	jmp interrupt_common
%endmacro

%macro error_code_stub 1
global exception_stub_%1
exception_stub_%1:
	push %1 ;Номер прерывания
	jmp interrupt_common
%endmacro

no_error_code_stub 0
no_error_code_stub 3
error_code_stub 13 ;General Protection Fault
error_code_stub 14 ;Page Fault

;Заглушка для таймера (IRQ0 -> 32)
global irq0_handler
irq0_handler:
	push 0 ;Фиктивный код ошибки
	push 32 ;Номер прерывания
	jmp interrupt_common

interrupt_common:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rdi, rsp 
	mov rbp, rsp 
	and rsp, -16 

	;Проверяем номер прерывания (int_no находится по смещению 120 байт от начала структы)
	;15 регистров * 8 байт = 120
	cmp qword [rbp + 120], 32
	jae .is_irq

	call exception_handler
	mov rsp, rbp
	jmp .done

.is_irq:
	call isr_handler

.done:
	mov rsp, rbp 
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	add rsp, 16 
	iretq