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

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while (n--) {
        if (*p1 != *p2) {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}