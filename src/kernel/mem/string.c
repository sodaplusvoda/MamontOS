#include "mem/string.h"

void *memset(void *dest, int c, size_t n) {
	__asm__ volatile (
		"rep stosb"
		: "=D"(dest), "=c"(n)
		: "0"(dest), "a"((unsigned char)c), "1"(n)
		: "memory"
	);
	return dest;
}

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *d = dest;
	const uint8_t *s = src;
	while (n--) *d++ = *s++;
	return dest;
}