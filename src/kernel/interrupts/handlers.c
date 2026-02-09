#include "interrupts/handlers.h"\

static uint64_t timer_ticks = 0;

struct interrupt_frame {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
	uint64_t int_no, error_code;
	uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

void exception_handler(struct interrupt_frame* frame) {
	uint64_t cr2;
	__asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
	print_color("\nEXCEPTION ", 0x04);
    
	if (frame->int_no == 3) {
		print_color("Breakpoint (int 3) detected\n", 0x0E);
	} else {
		print_color("Vector: ", 0x04);
		print_hex64(frame->int_no);
		print_color("\n", 0x0f);
	}

	print_color("Instruction Pointer (RIP): ", 0x07);
	print_hex64(frame->rip);
	print_color("\n", 0x07);
	print_color("CR2: ", 0x07);
	print_hex64(cr2);
	print_color("\n", 0x07);

	for(;;);
}

void page_fault_handler(struct interrupt_frame* frame) {
	uint64_t cr2;
	__asm__ volatile("mov %%cr2, %0" : "=r"(cr2));

	print_color("\nPAGE FAULT\n", 0x04);
    
	print_color("CR2: ", 0x07);
	print_hex64(cr2);
	print_color("\n", 0x07);

	print_color("Error Code: ", 0x07);
	print_hex64(frame->error_code);

	if (!(frame->error_code & 0x1)) print_color(" Not-Present", 0x0E);
	if (frame->error_code & 0x2) print_color(" Write", 0x0E);
	if (frame->error_code & 0x4) print_color(" User", 0x0E);
	if (frame->error_code & 0x10) print_color(" Instruction-Fetch", 0x0E);
	print_color("\n", 0x07);

	print_color("RIP: ", 0x07);
	print_hex64(frame->rip);
	print_color("\n", 0x07);

	for(;;);
}

void isr_handler(struct interrupt_frame* frame) {
	if (frame->int_no == 14) {
		page_fault_handler(frame);
		return;
	}
	if (frame->int_no == 32) { //IRQ0
		timer_ticks++;
		if (timer_ticks % 100 == 0 && timer_ticks <= 300) {
			print_color("\n", 0x0f);
			print_color("Tick! ", 0x0E);
			print_hex64(timer_ticks);
			print_color("\n", 0x0E);
		}
		pic_sendeoi(0);
	}
}