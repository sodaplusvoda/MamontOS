#pragma once

#include <stdint.h>
#include <stddef.h>

void early_init(uint64_t base_addr, uint64_t size);
void* early_alloc(size_t size);