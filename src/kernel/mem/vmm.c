#include "mem/vmm.h"
#include "mem/pmm.h"
#include "mem/string.h"
#include "debug/print.h"

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

static uint64_t* get_next_level(uint64_t* current_table, uint64_t index) {
}

bool vmm_map_page(uintptr_t virt, uintptr_t phys, uint64_t flags) {
}

void vmm_init() {
	print_color("VMM initialization... (in development)\n", 0x0A);
}