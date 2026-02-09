#pragma once
#include <stdint.h>
#include <stdbool.h>

#define KERNEL_VIRTUAL_BASE 0xFFFF800000000000
#define PHYS_TO_VIRT(addr) ((void*)((uintptr_t)(addr) + KERNEL_VIRTUAL_BASE))
#define VIRT_TO_PHYS(addr) ((uintptr_t)(addr) - KERNEL_VIRTUAL_BASE)

#define PAGE_SIZE 4096

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE (1 << 1)
#define PAGE_USER (1 << 2)

#define P4_IDX(addr) (((addr) >> 39) & 0x1FF)
#define P3_IDX(addr) (((addr) >> 30) & 0x1FF)
#define P2_IDX(addr) (((addr) >> 21) & 0x1FF)
#define P1_IDX(addr) (((addr) >> 12) & 0x1FF)

void vmm_init();
bool vmm_map_page(uintptr_t virt, uintptr_t phys, uint64_t flags);