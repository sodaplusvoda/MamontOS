#pragma once

#include <stdint.h>
#include <stddef.h>
#include "multiboot2/multiboot2.h"
#include "mem/alloc/early_alloc.h"
#include "mem/alloc/buddy_alloc.h"
#include "debug/print.h"

#define PAGE_SIZE 4096

void pmm_init(struct multiboot_info* mb_info);
void* pmm_alloc_page();
void pmm_free_page(void* addr);