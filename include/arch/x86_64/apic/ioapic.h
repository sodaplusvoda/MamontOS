#pragma once

#include <stdint.h>
#include <arch/x86_64/apic/lapic.h>
#include <arch/x86_64/io-hardware/mmio.h>
#include <debug/print.h>

#define IOAPIC_BASE_DEFAULT 0xFEC00000
#define IOAPIC_REG_INDEX 0x00 //Регистр выбора индекса (IOREGSEL)
#define IOAPIC_REG_DATA 0x10 //Регистр данных (IOWIN)
#define IOAPIC_ID 0x00 //ID контроллера
#define IOAPIC_VER 0x01 //Версия и количество записей в Redirection Table
#define IOAPIC_ARB 0x02 //Приоритет арбитража
#define IOAPIC_REDTBL_BASE 0x10 //Начало таблицы перенаправления (по 2 регистра на запись)
#define IOAPIC_MASK_DISABLED (1 << 16) //Замаскировать прерывание
#define IOAPIC_TRIGGER_LEVEL (1 << 15) //0 Edge (по фронту), 1 Level (по уровню)
#define IOAPIC_POLARITY_LOW (1 << 13) //0 High (активный высокий), 1 Low (активный низкий)
#define IOAPIC_DEST_LOGICAL (1 << 11) //0 Physical, 1 Logical destination
#define IOAPIC_DELIVERY_FIXED 0x000
#define IOAPIC_DELIVERY_LOWEST 0x100
#define IOAPIC_DELIVERY_SMI 0x200
#define IOAPIC_DELIVERY_NMI 0x400
#define IOAPIC_DELIVERY_INIT 0x500
#define IOAPIC_DELIVERY_ExtINT 0x700

typedef struct {
	uint32_t lower; //Вектор, режим доставки, флаги
	uint32_t upper; //Destination APIC ID получателя
} __attribute__((packed)) ioapic_redirection_entry_t;

uint32_t ioapic_read(uintptr_t base, uint8_t index);
void ioapic_write(uintptr_t base, uint8_t index, uint32_t data);
void ioapic_set_entry(uintptr_t base, uint8_t irq, ioapic_redirection_entry_t entry);
void ioapic_enable_irq(uintptr_t base, uint8_t irq, uint8_t vector, uint8_t apic_id);
void ioapic_init();