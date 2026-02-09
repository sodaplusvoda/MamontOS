#pragma once

#include <stdint.h>
#include <stddef.h>

void cls();
void scroll();
void print_color(char* str, uint8_t color);
void print_hex8(uint8_t key);
void print_hex64(uint64_t key);
void print_char(char c, uint8_t color);
