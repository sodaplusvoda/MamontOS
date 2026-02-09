#include <stdint.h>
#include <stddef.h>
#include "mem/alloc/early_alloc.h"

extern uint8_t _kernel_start;
extern uint8_t _kernel_end;

static uintptr_t next_free_addr = (uintptr_t)&_kernel_end + 0x100000;

static uintptr_t align_up(uintptr_t addr, uint32_t align) {
	return (addr + (align - 1)) & ~(uintptr_t)(align - 1);
}

void* early_alloc(size_t size) {
	//Выравниваем текущий указатель по 16 байт
	next_free_addr = align_up(next_free_addr, 16);

	//Запоминаем текущее значение, чтобы вернуть его
	uintptr_t addr_to_return = next_free_addr;
	//Сдвигаем глобальный указатель вперед на размер запрошенной памяти
	next_free_addr += size;
	//Возвращаем запомненный адрес
	return (void*)addr_to_return;
}

void early_init(uint64_t base_addr, uint64_t size) {
	next_free_addr = base_addr;
}
