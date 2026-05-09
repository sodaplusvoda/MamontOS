#include "debug/print.h"

static uint32_t *fb_addr = (uint32_t *)0;
static uint32_t fb_pitch = 0;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

void fb_init(struct multiboot_info *mbi) {
	if (!mbi) return;

	struct multiboot_tag *tag;
	uint8_t *mbi_ptr = (uint8_t *)mbi;
	uint32_t processed_size = 8;

	for (tag = (struct multiboot_tag *)(mbi_ptr + 8);
		tag->type != 0 && processed_size < mbi->total_size;
		tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7))) {
        
		processed_size += (tag->size + 7) & ~7;

		if (tag->type == 8) {
			struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *)tag;
			fb_addr = (uint32_t *)fb->framebuffer_addr;
			fb_pitch = fb->framebuffer_pitch;
			fb_width = fb->framebuffer_width;
			fb_height = fb->framebuffer_height;
			return;
		}
	}
}

void cls(uint32_t color) {
	if (!fb_addr) return;

	if (color == 0x000000) {
		memset(fb_addr, 0, fb_height * fb_pitch);
	} else {
		for (uint32_t i = 0; i < fb_height * (fb_pitch / 4); i++) {
			fb_addr[i] = color;
		}
	}
}

void putdot(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= fb_width || y >= fb_height) return;
	uint32_t *pixel = fb_addr + (y * (fb_pitch / 4)) + x;
	*pixel = color;
}

void set_cursor(uint32_t x, uint32_t y) {
	cursor_x = x;
	cursor_y = y;
}

void fb_scroll() {
	if (!fb_addr) return;

	uint32_t line_height = 16;
	uint32_t bytes_per_line = fb_pitch; 
	uint32_t scroll_region_height = fb_height - line_height;

	memcpy(fb_addr, (uint8_t*)fb_addr + (line_height * bytes_per_line), scroll_region_height * bytes_per_line);

	uint32_t *last_line_ptr = (uint32_t*)((uint8_t*)fb_addr + (scroll_region_height * bytes_per_line));

	memset(last_line_ptr, 0, line_height * bytes_per_line);

	cursor_y = fb_height - line_height;
	cursor_x = 0;
}

void print_char(uint32_t x, uint32_t y, char c, uint32_t color) {
	if ((uint8_t)c > 127) return;

	for (int i = 0; i < 8; i++) {
		uint8_t row = builtin_font[(int)c][i];
		for (int j = 0; j < 8; j++) {
			if (row & (0x80 >> j)) {
				putdot(x + j, y + i, color);
			}
		}
	}
}

void print_color(const char *str, uint32_t color) {
	while (*str) {
		if (cursor_y + 16 > fb_height) {
			fb_scroll();
		}

		if (*str == '\n') {
			cursor_x = 0;
			cursor_y += 16;
		} else if (*str == '\t') {
			cursor_x += 32; 
		} else {
			print_char(cursor_x, cursor_y, *str, color);
			cursor_x += 8;
		}

		if (cursor_x + 8 > fb_width) {
			cursor_x = 0;
			cursor_y += 16;
		}
        
		str++;
	}
}

void print_hex8(uint8_t key) {
	char* hex_digits = "0123456789ABCDEF";
	char buffer[3];
	buffer[0] = hex_digits[(key >> 4) & 0x0F];
	buffer[1] = hex_digits[key & 0x0F];
	buffer[2] = '\0';
	print_color(buffer, 0xFFFFFF);
}

void print_hex64(uint64_t key) {
	print_color("0x", 0xFFFFFF);
	for (int i = 7; i >= 0; i--) {
		uint8_t byte = (key >> (i * 8)) & 0xFF;
		print_hex8(byte);
	}
}