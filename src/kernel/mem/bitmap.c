#include "mem/bitmap.h"

void bitmap_set(bitmap* bitmap, uint64_t index, bool value) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_mask = 1 << bit_index;

    if (byte_index >= bitmap->size) return;

    if (value) {
        bitmap->buffer[byte_index] |= bit_mask;
    } else {
        bitmap->buffer[byte_index] &= ~bit_mask;
    }
}

bool bitmap_get(bitmap* bitmap, uint64_t index) {
    uint64_t byte_index = index / 8;
    uint8_t bit_index = index % 8;
    uint8_t bit_mask = 1 << bit_index;

    if (byte_index >= bitmap->size) return false;

    return (bitmap->buffer[byte_index] & bit_mask) > 0;
}