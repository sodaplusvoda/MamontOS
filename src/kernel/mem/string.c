#include "mem/string.h"

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--) {
        if( *p1 != *p2 ) return *p1 - *p2;
        p1++; p2++;
    }
    return 0;
}

void* memset(void* dest, int ch, size_t count) {
    uint8_t* p = (uint8_t*)dest;
    for (size_t i = 0; i < count; i++) {
        p[i] = (uint8_t)ch;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < count; i++) {
        d[i] = s[i];
    }
    return dest;
}