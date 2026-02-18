#include "debug/print.h"

static uint16_t cursor_pos = 0;
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VIDEO_MEM_ADDR 0xB8000

void cls() {
    volatile uint16_t *VIDEO_MEMORY = (uint16_t *)0xB8000;
    uint16_t blank = (' ' & 0xFF) | (0x0F << 8);
    for (int i = 0; i < 80 * 25; i++) {
        VIDEO_MEMORY[i] = blank;
    }
    cursor_pos = 0;
}

void scroll() {
    volatile uint16_t *VIDEO_MEMORY = (uint16_t *)VIDEO_MEM_ADDR;

    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        VIDEO_MEMORY[i] = VIDEO_MEMORY[i + VGA_WIDTH];
    }

    uint16_t blank = (' ' & 0xFF) | (0x0F << 8);
    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        VIDEO_MEMORY[i] = blank;
    }

    cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
}

void print_char(char c, uint8_t color) {
    volatile uint16_t *VIDEO_MEMORY = (uint16_t *)VIDEO_MEM_ADDR;
    
    if (c == '\n') {
        cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
    } else {
        VIDEO_MEMORY[cursor_pos] = (c & 0xFF) | (color << 8);
        cursor_pos++;
    }

    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        scroll();
    }
}

void print_color_pos(char* str, uint8_t color, uint16_t x, uint16_t y) {
    volatile uint16_t *VIDEO_MEMORY = (uint16_t *)VIDEO_MEM_ADDR;
    uint16_t pos = y * VGA_WIDTH + x;

    while (*str) {
        if (pos >= VGA_WIDTH * VGA_HEIGHT) break;
        VIDEO_MEMORY[pos] = (*str & 0xFF) | (color << 8);
        str++;
        pos++;
    }
}

void print_color(char* str, uint8_t color) {
    while (*str) {
        print_char(*str, color);
        str++;
    }
}

void print_hex8(uint8_t key) {
    char* hex_digits = "0123456789ABCDEF";
    char buffer[3];
    buffer[0] = hex_digits[(key >> 4) & 0x0F];
    buffer[1] = hex_digits[key & 0x0F];
    buffer[2] = '\0';
    print_color(buffer, 0x0F);
}

void print_hex64(uint64_t key) {
    print_color("0x", 0x0F);
    for (int i = 7; i >= 0; i--) {
        uint8_t byte = (key >> (i * 8)) & 0xFF;
        print_hex8(byte);
    }
}
