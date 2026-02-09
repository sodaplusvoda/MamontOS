#pragma once

#include "mem/bitmap.h"
#include "mem/alloc/early_alloc.h"
#include "mem/alloc/buddy_alloc.h"
#include "debug/print.h"
#include "multiboot2/multiboot2.h"
#include "mem/string.h"

void pmm_init(uint64_t multiboot_addr);
void* pmm_alloc_pages(uint8_t order);
void pmm_free_pages(void* ptr, uint8_t order);