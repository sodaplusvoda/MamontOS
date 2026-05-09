#include <arch/x86_64/io-hardware/mmio.h>
#include <arch/x86_64/apic/acpi.h>

uint8_t mmio_read8(uintptr_t addr) {
	return *(volatile uint8_t*)(addr);
}

uint16_t mmio_read16(uintptr_t addr) {
	return *(volatile uint16_t*)(addr);
}

uint32_t mmio_read32(uintptr_t addr) {
	return *(volatile uint32_t*)(addr);
}

uint64_t mmio_read64(uintptr_t addr) {
	return *(volatile uint64_t*)(addr);
}

void mmio_write8(uintptr_t addr, uint8_t data) {
	*(volatile uint8_t*)(addr) = data;
}

void mmio_write16(uintptr_t addr, uint16_t data) {
	*(volatile uint16_t*)(addr) = data;
}

void mmio_write32(uintptr_t addr, uint32_t data) {
	*(volatile uint32_t*)(addr) = data;
}

void mmio_write64(uintptr_t addr, uint64_t data) {
	*(volatile uint64_t*)(addr) = data;
}