#include "debug/print.h"
#include "gdt/gdt.h"
#include "pic/pic.h"
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
	cls();
	print_color("MamontOS initialization...\n", 0x0f);
	print_color("Multiboot Information Address: ", 0x07);
	print_hex64(multiboot_addr);
	print_color("\n", 0x07);
	gdt_init();
	idt_init();
	pic_remap(0x20, 0x28);
	pit_init(100);
	outb(PIC1_DATA, 0xfe); 
	outb(PIC2_DATA, 0xff);
	uint64_t total_ram = 0;

	/*struct multiboot_info *mbi = (struct multiboot_info *)multiboot_addr;
	struct multiboot_tag *tag;

	for (tag = (struct multiboot_tag *)(multiboot_addr + 8);
	tag->type != 0; //Тип 0 означает конец списка
	tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {
		//Ищем тег карты памяти тип 6
		if (tag->type == 6) {
			struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
            
			//Проходим по всем записям в карте памяти
			for (uint8_t *p = (uint8_t *)mmap->entries;
			p < (uint8_t *)tag + tag->size;
			p += mmap->entry_size) {
				struct multiboot_mmap_entry *entry = (struct multiboot_mmap_entry *)p;
                
				//Если тип == 1, значит эта память RAM свободна
				if (entry->type == 1) {
					total_ram += entry->len;
					print_color("RAM Region: ", 0x0f);
					print_hex64(entry->addr);
					print_color(" Length: ", 0x0f);
					print_hex64(entry->len);
					print_char('\n', 0x0F);
				}
			}
		}
	}*/

	__asm__ volatile("sti");
	print_color("Interrupts enable\n", 0x0a);

	pmm_init(multiboot_addr);
	vmm_init();
	
	while(1) { __asm__ ("hlt"); }
}