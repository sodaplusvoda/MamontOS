#include "mem/pmm.h"
#include "mem/alloc/early_alloc.h"
#include "mem/alloc/buddy_alloc.h"
#include "debug/print.h"
#include "multiboot2/multiboot2.h"

static bitmap pmm_bitmap;
uint64_t total_usable_ram = 0;
static uint64_t page_count = 0;
static uint64_t bitmap_size = 0;

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];
extern uint8_t gdt64[];
extern uint8_t stack_top[];

void pmm_init(uint64_t multiboot_addr) {
	print_color("PMM initialization...\n", 0x0A);
	struct multiboot_tag *tag;

	for (tag = (struct multiboot_tag *)(multiboot_addr + 8);
		tag->type != 0; 
		tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7))) 
	{
		if (tag->type == 6) { //MMAP tag
			struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
            
			struct multiboot_mmap_entry *entry;
			for (entry = mmap->entries;
				(uint8_t *)entry < (uint8_t *)tag + tag->size;
				entry = (struct multiboot_mmap_entry *)((uintptr_t)entry + mmap->entry_size)) 
			{
				if (entry->type == 1) { //1 доступная памяьть
					total_usable_ram += entry->len;
				}
			}
		}
	}

	page_count = total_usable_ram / 4096;
	bitmap_size = page_count / 8;

	pmm_bitmap.buffer = (uint8_t*)early_alloc(bitmap_size);
	pmm_bitmap.size = bitmap_size;

	__asm__ volatile (
		"movq %0, %%rdi\n"
		"movq %1, %%rcx\n"
		"mov  $0xFF, %%rax\n"
		"rep stosb\n"
		:
		: "r"(pmm_bitmap.buffer), "r"(bitmap_size)
		: "rdi", "rcx", "rax", "memory"
	);

	for (uint64_t i = 0; i < pmm_bitmap.size; i++) {
		if (pmm_bitmap.buffer[i] != 0xFF) {
			print_color("CRITICAL BITMAP DATA\n", 0x0C);
		}
	}

	uintptr_t buddy_start = (uintptr_t)pmm_bitmap.buffer + bitmap_size;
	buddy_start = (buddy_start + 4095) & ~4095; 

	uint64_t used_by_pmm = (buddy_start - (uintptr_t)pmm_bitmap.buffer) / 4096;
	buddy_init(buddy_start, page_count - used_by_pmm);

	void* page1 = pmm_alloc_pages(0);
	void* page2 = pmm_alloc_pages(0);

	print_color("Allocated Page 1: ", 0x0f); print_hex64((uintptr_t)page1); print_color("\n", 0x0f);
	print_color("Allocated Page 2: ", 0x0f); print_hex64((uintptr_t)page2); print_color("\n", 0x0f);

	if (page1 == page2) {
		print_color("ERROR ALLOCATOR\n", 0x0C);
	} else {
		*(uint64_t*)page1 = 0x12345678;
		if (*(uint64_t*)page1 == 0x12345678) {
			print_color("Memory Test Okay\n", 0x0A);
		}
	}

	print_color("Kernel End: ", 0x07);
	print_hex64((uint64_t)&_kernel_end);
	print_color("\nTotal RAM: ", 0x0f); print_hex64(total_usable_ram); print_color(" bytes\n", 0x0f);
	print_color("Page Count: ", 0x0f); print_hex64(page_count); print_color("\n", 0x0f);
	print_color("Bitmap: ", 0x07);
	print_hex64((uint64_t)pmm_bitmap.buffer);
	print_color(" Size: ", 0x07);
	print_hex64(bitmap_size);
	print_color("\n", 0x07);
	
}

void* pmm_alloc_pages(uint8_t order) {
	return buddy_alloc(order);
}

void pmm_free_pages(void* ptr, uint8_t order) {
	buddy_free(ptr, order);
}