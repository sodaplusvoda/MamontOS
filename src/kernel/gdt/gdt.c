#include "gdt/gdt.h"
#include "debug/print.h"

struct gdt_struct gdt;
struct tss_entry tss;

extern void gdt_load(struct gdt_ptr* ptr);

void gdt_init() {
	print_color("GDT initialization...\n", 0x02);

	//for(int i = 0; i < sizeof(struct gdt_struct); i++) ((uint8_t*)&gdt)[i] = 0;
	//for(int i = 0; i < sizeof(struct tss_entry); i++) ((uint8_t*)&tss)[i] = 0;

	//Null
	gdt.target[0] = (struct gdt_entry){0, 0, 0, 0, 0, 0};

	//Kernel Code: Access 0x9A, Flags 0x2 (L=1 для 64-бит)
	gdt.target[1] = (struct gdt_entry){0, 0, 0, 0x9A, 0x20, 0};

	//Kernel Data: Access 0x92, Flags 0x0
	gdt.target[2] = (struct gdt_entry){0, 0, 0, 0x92, 0x00, 0};

	//User Data: Access 0xF2 (DPL=3), Flags 0x0
	gdt.target[3] = (struct gdt_entry){0, 0, 0, 0xF2, 0x00, 0};

	//User Code: Access 0xFA (DPL=3), Flags 0x2
	gdt.target[4] = (struct gdt_entry){0, 0, 0, 0xFA, 0x20, 0};

	//TSS
	uint64_t tss_base = (uint64_t)&tss;
	gdt.tss.limit_low = sizeof(struct tss_entry) - 1;
	gdt.tss.base_low = tss_base & 0xFFFF;
	gdt.tss.base_mid = (tss_base >> 16) & 0xFF;
	gdt.tss.access = 0x89;
	gdt.tss.flags = 0x00;
	gdt.tss.base_high = (tss_base >> 24) & 0xFF;
	gdt.tss.base_upper32 = (tss_base >> 32) & 0xFFFFFFFF;

	gdt.tss.reserved = 0;

	uint64_t gdt_base = (uint64_t)&gdt;

	print_color("GDT Base Address: ", 0x07);
	print_hex64(gdt_base);
	print_color("\n", 0x07);

	struct gdt_ptr ptr;
	ptr.limit = sizeof(struct gdt_struct) - 1;
	ptr.base = (uint64_t)&gdt;

	gdt_load(&ptr);
}