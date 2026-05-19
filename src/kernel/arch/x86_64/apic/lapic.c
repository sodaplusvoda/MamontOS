#include <arch/x86_64/apic/lapic.h>
#include <arch/x86_64/io-hardware/mmio.h>

static uintptr_t g_lapic_base = 0;

uintptr_t get_lapic_base() {
	uint32_t low, high;
	//Инструкция rdmsr - Read MSR
	//0x1B - это адрес IA32_APIC_BASE
	__asm__ volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));
	//Извлекаем адрес биты 12-51
	return (uintptr_t)((high & 0xFFF) << 32) | (low & 0xFFFFF000);
}

void lapic_init() {
	g_lapic_base = get_lapic_base();
	//Читаем текущее состояние MSR 0x1B
	uint32_t low, high;
	__asm__ volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));
    
	//Устанавливаем 11-й бит (APIC Global Enable), если он не был установлен
	if (!(low & (1 << 11))) {
		low |= (1 << 11);
		__asm__ volatile("wrmsr" : : "a"(low), "d"(high), "c"(0x1B));
	}

	//0x1FF = 0x100 (Enable) | 0xFF (Vector)
	mmio_write32(g_lapic_base + LAPIC_REG_SIV, 0x1FF);

	print_color("[LAPIC] LAPIC init. LAPIC Base: ", 0x00FF00);
	print_hex64(g_lapic_base);
	print_color("\n", 0x00FF00); 
}

void lapic_eoi() {
	mmio_write32(g_lapic_base + 0xB0, 0);
}