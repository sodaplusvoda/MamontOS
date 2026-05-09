#pragma once

#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/io-hardware/mmio.h>
#include <debug/print.h>

#define LAPIC_BASE 0xFEE00000 //Уже не нужна. Просто для галочки
#define LAPIC_REG_SIV 0xF0

uintptr_t get_lapic_base();
void lapic_eoi();
void lapic_init();