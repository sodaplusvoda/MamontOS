#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint64_t size; //Размер битмапа в байтах
    uint8_t* buffer; //Указатель на начало данных битмапа
} bitmap;

void bitmap_set(bitmap* bitmap, uint64_t index, bool value);
bool bitmap_get(bitmap* bitmap, uint64_t index);