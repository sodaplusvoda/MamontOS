#pragma once
#include <stdint.h>
#include "debug/print.h"

#define IDT_TA_InterruptGate 0x8E //Present, Ring 0, Interrupt Gate
#define IDT_TA_TrapGate 0x8F //Present, Ring 0, Trap Gate

struct idt_entry {
	uint16_t offset_low; //Смещение 0..15
	uint16_t selector; //Селектор сегмента кода в GDT
	uint8_t  ist; //Interrupt Stack Table
	uint8_t  type_attr; //Тип и атрибуты
	uint16_t offset_mid; //Смещение 16..31
	uint32_t offset_high; //Смещение 32..63
	uint32_t reserved; //Зарезервировэйшн
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit; //Размер таблицы - 1
	uint64_t base; //Линейный адрес таблицы
} __attribute__((packed));

void idt_init(void);

void idt_set_gate(uint8_t vector, void* handler, uint8_t flags);