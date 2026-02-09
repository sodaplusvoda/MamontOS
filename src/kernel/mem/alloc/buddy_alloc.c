#include "mem/alloc/buddy_alloc.h"

static buddy_allocator_t main_buddy;

static uintptr_t get_buddy_addr(uintptr_t addr, uint8_t order) {
	return addr ^ (1UL << (12 + order)); //12 это log2(4096)
}

void buddy_init(uintptr_t base, uint64_t page_count) {
	main_buddy.start_addr = base;
	main_buddy.total_pages = page_count;

	//Обнуляем все списки
	for (int i = 0; i < MAX_ORDER; i++) {
		main_buddy.free_lists[i] = NULL;
	}

	uintptr_t current_base = base;
	uint64_t pages_left = page_count;

	//Идем от самого большого порядка к самому маленькому
	for (int i = MAX_ORDER - 1; i >= 0; i--) {
		uint64_t block_size = (1UL << i); //размер в страницах
        
		while (pages_left >= block_size) {
			buddy_block_t* block = (buddy_block_t*)current_base;
			block->next = main_buddy.free_lists[i];
			main_buddy.free_lists[i] = block;

			current_base += (block_size * 4096);
			pages_left -= block_size;
		}
	}
}

void* buddy_alloc(uint8_t order) {
	if (order >= MAX_ORDER) return NULL;

	//Ищем свободный блок нужного или большего порядка
	for (uint8_t i = order; i < MAX_ORDER; i++) {
		if (main_buddy.free_lists[i]) {
			buddy_block_t* block = main_buddy.free_lists[i];
			main_buddy.free_lists[i] = block->next;

			//Расщепляем большие блоки на части (buddies)
			while (i > order) {
				i--;
				uintptr_t buddy_addr = (uintptr_t)block + (1UL << (12 + i));
				buddy_block_t* buddy = (buddy_block_t*)buddy_addr;
		        
				//Добавляем близнеца в список меньшего порядка
				buddy->next = main_buddy.free_lists[i];
				main_buddy.free_lists[i] = buddy;
			}
			return (void*)block;
		}
	}
	return NULL; //Памяти НЕТУУУ
}

void buddy_free(void* ptr, uint8_t order) {
	if (!ptr || order >= MAX_ORDER) return;

	uintptr_t addr = (uintptr_t)ptr;
	uintptr_t relative_addr = addr - main_buddy.start_addr;

	while (order < MAX_ORDER - 1) {
		//Находим адрес близнеца (buddy)
		//Формула XOR с размером текущего блока
		uintptr_t buddy_relative_addr = relative_addr ^ (1UL << (12 + order));
		uintptr_t buddy_addr = buddy_relative_addr + main_buddy.start_addr;

		//Ищем близняшку в списке свободных блоков текущего порядка
		buddy_block_t** prev = &main_buddy.free_lists[order];
		buddy_block_t* current = main_buddy.free_lists[order];
		bool found = false;

		while (current) {
			if ((uintptr_t)current == buddy_addr) {
				//Близнец найден и он свободен. Извлекаем его из списка
				*prev = current->next;
				found = true;
				break;
			}
		prev = &current->next;
		current = current->next;
		}

		if (!found) {
			//Близнец занят
			break;
		}

		if (buddy_addr < addr) {
			addr = buddy_addr;
			relative_addr = buddy_relative_addr;
		}
        
		//Переходим к следующему порядку
		order++;
	}

	buddy_block_t* final_block = (buddy_block_t*)addr;
	final_block->next = main_buddy.free_lists[order];
	main_buddy.free_lists[order] = final_block;
}