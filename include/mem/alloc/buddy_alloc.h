#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_ORDER 11 //Макс блок 4 КБ * 2^10 = 4 МБ

typedef struct buddy_block {
    struct buddy_block* next;
} buddy_block_t;

typedef struct {
    uintptr_t start_addr;
    uint64_t total_pages;
    buddy_block_t* free_lists[MAX_ORDER];
} buddy_allocator_t;

void buddy_init(uintptr_t base, uint64_t page_count);
void* buddy_alloc(uint8_t order);
void buddy_free(void* ptr, uint8_t order);