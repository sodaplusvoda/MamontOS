#include "debug/print.h"
#include "arch/x86_64/gdt/gdt.h"
#include "arch/x86_64/pic/pic.h"
#include "arch/x86_64/apic/apic.h"
#include "interrupts/idt.h"
#include "multiboot2/multiboot2.h"
#include "mem/alloc/early_alloc.h"
#include "mem/bitmap.h"
#include "mem/pmm.h"
#include "mem/vmm.h"

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];
extern uint8_t gdt64[];
extern uint8_t stack_top[];

void kernel_main(uint64_t multiboot_addr) {
	fb_init((struct multiboot_info *)multiboot_addr);
	cls(0x000000);
	print_color("MamontOS initialization...\n", 0xFFFFFF);
	print_color("Multiboot Information Address: ", 0xFFFF00);
	print_hex64(multiboot_addr);
	print_color("\n", 0xFFFFFF);
	gdt_init();
	idt_init();
	pic_remap(0x20, 0x28);
	pit_init(100);
	outb(PIC1_DATA, 0xfe);
	outb(PIC2_DATA, 0xff);
	apic_init();
	uint64_t total_ram = 0;

	__asm__ volatile("sti");

	print_color("Interrupts enable\n", 0xFFFFFF);
	
	while(1) { __asm__ ("hlt"); }
}