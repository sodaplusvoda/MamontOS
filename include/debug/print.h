#pragma once

#include <stdint.h>
#include <stddef.h>
#include "multiboot2/multiboot2.h"
#include "debug/font.h"
#include "mem/string.h"

void fb_init(struct multiboot_info *mbi);
void cls(uint32_t color);
void putdot(uint32_t x, uint32_t y, uint32_t color);
void set_cursor(uint32_t x, uint32_t y);
void fb_scroll();
void print_char(uint32_t x, uint32_t y, char c, uint32_t color);
void print_color(const char *str, uint32_t color);
void print_hex8(uint8_t key);
void print_hex64(uint64_t key);