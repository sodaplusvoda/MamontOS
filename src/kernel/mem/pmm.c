#include "mem/pmm.h"

static uint64_t total_pages = 0;
static uint64_t last_free_page = 0;

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

void pmm_init(struct multiboot_info* mb_info) {
	if (!mb_info) return;

	print_color("[PMM] Parsing Multiboot2 memory map...\n", 0x00FF00);

	struct multiboot_tag* tag = (struct multiboot_tag*)(mb_info + 1);
	struct multiboot_tag_mmap* mmap_tag = NULL;

	while (tag->type != MULTIBOOT_TAG_TYPE_END) {
		if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
			mmap_tag = (struct multiboot_tag_mmap*)tag;
			break;
		}
		tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
	}

	if (!mmap_tag) {
		print_color("[PMM] ERROR: Memory map tag not found!\n", 0xFF0000);
		return;
	}

	uintptr_t kernel_start_phys = (uintptr_t)_kernel_start;
	uintptr_t kernel_end_phys = (uintptr_t)_kernel_end;

	uintptr_t early_alloc_end = early_get_next_addr();

	uintptr_t multiboot_start = (uintptr_t)mb_info;
	uintptr_t multiboot_end = multiboot_start + mb_info->total_size;

	uintptr_t highest_reserved_addr = kernel_end_phys;
	if (early_alloc_end > highest_reserved_addr) {
		highest_reserved_addr = early_alloc_end;
	}
	if (multiboot_end > highest_reserved_addr && multiboot_start < highest_reserved_addr + 0x1000000) {
		highest_reserved_addr = multiboot_end;
	}

	uintptr_t reserved_end = (highest_reserved_addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

	uintptr_t best_region_base = 0;
	uint64_t best_region_pages = 0;

	struct multiboot_mmap_entry* entry = mmap_tag->entries;
	uint32_t entries_count = (mmap_tag->size - sizeof(struct multiboot_tag_mmap)) / mmap_tag->entry_size;

	for (uint32_t i = 0; i < entries_count; i++) {
		//MULTIBOOT_MEMORY_AVAILABLE = 1 означает свободную RAM
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
			uint64_t base = entry->addr;
			uint64_t length = entry->len;
			uint64_t end = base + length;

			if (base < 0x100000) {
				if (end <= 0x100000) {
					entry = (struct multiboot_mmap_entry*)((uint8_t*)entry + mmap_tag->entry_size);
					continue;
				}
				base = 0x100000;
				length = end - base;
			}

			if (base < reserved_end) {
				if (end <= reserved_end) {
					entry = (struct multiboot_mmap_entry*)((uint8_t*)entry + mmap_tag->entry_size);
					continue;
				}
				base = reserved_end;
				length = end - base;
			}

			//Выравниваем базовый адрес страницы вверх, а размер вниз (до кратных 4 КБ)
			uintptr_t aligned_base = (base + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
            
			if (length > (aligned_base - base)) {
				uint64_t aligned_len = (length - (aligned_base - base)) & ~(PAGE_SIZE - 1);
				uint64_t pages = aligned_len / PAGE_SIZE;

				if (pages > 0) {
					total_pages += pages; //Подсчет общей доступной RAM в системе
                    
					//Выбираем наибольший непрерывный кусок памяти для Buddy Allocator
					if (pages > best_region_pages) {
						best_region_base = aligned_base;
						best_region_pages = pages;
					}
				}
			}
		}
		entry = (struct multiboot_mmap_entry*)((uint8_t*)entry + mmap_tag->entry_size);
	}

	if (best_region_pages > 0) {
		print_color("[PMM] Initializing Buddy Allocator at: ", 0x00FF00);
		print_hex64(best_region_base);
		print_color(" with pages: ", 0x00FF00);
		print_hex64(best_region_pages);
		print_color("\n", 0xFFFFFF);

		buddy_init(best_region_base, best_region_pages);
        
		print_color("[PMM] Total available pages managed by PMM: ", 0x00FF00);
		print_hex64(total_pages);
		print_color("\n", 0xFFFFFF);
	} else {
		print_color("[PMM] CRITICAL ERROR: No suitable memory region found for Buddy Allocator!\n", 0xFF0000);
		while(1) { __asm__("hlt"); } //Останавливаем систему, работать без памяти он не будет
	}
}

void* pmm_alloc_page() {
	return buddy_alloc(0); 
}

void pmm_free_page(void* addr) {
	buddy_free(addr, 0);
}