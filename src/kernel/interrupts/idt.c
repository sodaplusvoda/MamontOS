#include "interrupts/idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void exception_stub_0();
extern void exception_stub_3();
extern void exception_stub_13();
extern void exception_stub_14();
extern void irq0_handler();

void idt_set_gate(uint8_t vector, void* handler, uint8_t flags) {
	uintptr_t base = (uintptr_t)handler;

	idt[vector].offset_low = base & 0xFFFF;
	idt[vector].selector = 0x08; //Селектор кода ядра в GDT
	idt[vector].ist = 0;
	idt[vector].type_attr = flags;
	idt[vector].offset_mid = (base >> 16) & 0xFFFF;
	idt[vector].offset_high = (base >> 32) & 0xFFFFFFFF;
	idt[vector].reserved = 0;
}

void idt_init() {
	print_color("IDT initialization...\n", 0x0A);
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = (uintptr_t)&idt;

	for(int i = 0; i < 256; i++) {
		idt_set_gate(i, 0, 0);
	}

	idt_set_gate(0,  exception_stub_0, IDT_TA_InterruptGate);
	idt_set_gate(3, exception_stub_3,IDT_TA_InterruptGate);
	idt_set_gate(13, exception_stub_13, IDT_TA_InterruptGate);
	idt_set_gate(14, exception_stub_14, IDT_TA_InterruptGate);
	idt_set_gate(32, irq0_handler, IDT_TA_InterruptGate);

	__asm__ volatile ("lidt %0" : : "m"(idtp));
	//print_color("Testing IDT...\n", 0x0f);
	//__asm__ volatile ("int $0x3");
}