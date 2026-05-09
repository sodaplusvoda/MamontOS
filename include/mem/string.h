#pragma once
#include "stdint.h"
#include <stddef.h>

int memcmp(const void* s1, const void* s2, size_t n);

void *memset(void *dest, int c, size_t n);
void *memcpy(void* dest, const void* src, size_t count);
int memcmp(const void *s1, const void *s2, size_t n);