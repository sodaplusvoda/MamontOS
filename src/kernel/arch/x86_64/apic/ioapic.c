#include <arch/x86_64/apic/ioapic.h>

uintptr_t g_ioapic_base = 0;

uint32_t ioapic_read(uintptr_t base, uint8_t index) {
	mmio_write32(base + IOAPIC_REG_INDEX, index);
	return mmio_read32(base + IOAPIC_REG_DATA);
}
void ioapic_write(uintptr_t base, uint8_t index, uint32_t data) {
	mmio_write32(base + IOAPIC_REG_INDEX, index);
	mmio_write32(base + IOAPIC_REG_DATA, data);
}

//Настройка записи в Redirection Table, каждый IRQ занимает 2 индекса
void ioapic_set_entry(uintptr_t base, uint8_t irq, ioapic_redirection_entry_t entry) {
	ioapic_write(base, IOAPIC_REDTBL_BASE + (irq * 2), entry.lower);
	ioapic_write(base, IOAPIC_REDTBL_BASE + (irq * 2) + 1, entry.upper);
}

void ioapic_enable_irq(uintptr_t base, uint8_t irq, uint8_t vector, uint8_t apic_id) {
	ioapic_redirection_entry_t entry;
    
	//Вектор (0-255)
	entry.lower = vector; 
	//Назначаем конкретный процессор upper 8 bits это APIC ID
	entry.upper = (uint32_t)apic_id << 24;

	ioapic_set_entry(base, irq, entry);
}
void ioapic_init() {
	uintptr_t ioapic_addr = IOAPIC_BASE_DEFAULT;
	ioapic_enable_irq(ioapic_addr, 1, 0x21, 0);
	//print_color("IOAPIC: IRQ 1 Keyboard redirected to vector 0x21\n", 0x00FF00);
	print_color("IOAPIC init. IOAPIC Base: ", 0x00FF00);
	print_hex64(g_ioapic_base);
	print_color("\n", 0x00FF00); 
}