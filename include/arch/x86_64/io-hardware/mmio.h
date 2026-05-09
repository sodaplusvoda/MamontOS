#pragma once

#include <stdint.h>
#include <debug/print.h>

uint8_t  mmio_read8(uintptr_t addr);
uint16_t mmio_read16(uintptr_t addr);
uint32_t mmio_read32(uintptr_t addr);
uint64_t mmio_read64(uintptr_t addr);

void mmio_write8(uintptr_t addr, uint8_t data);
void mmio_write16(uintptr_t addr, uint16_t data);
void mmio_write32(uintptr_t addr, uint32_t data);
void mmio_write64(uintptr_t addr, uint64_t data);