#include "arch/x86_64/apic/acpi.h"
#include "mem/string.h"
#include "debug/print.h"

extern uintptr_t g_ioapic_base;

//Поиск RSDP (Root System Description Pointer)
//Он находится либо в первых 1 КБ EBDA (Extended BIOS Data Area), либо в области памяти BIOS между 0x000E0000 - 0x000FFFFF.
acpi_rsdp_t* acpi_find_rsdp() {
	for (uintptr_t addr = 0x000E0000; addr < 0x000FFFFF; addr += 16) {
		if (memcmp((void*)addr, "RSD PTR ", 8) == 0) {
			return (acpi_rsdp_t*)addr;
		}
	}
	return NULL;
}

void* acpi_find_table(acpi_rsdp_t* rsdp, char* signature) {
	if (rsdp->Revision >= 2 && rsdp->XsdtAddress) {
		acpi_xsdt_t* xsdt = (acpi_xsdt_t*)(uintptr_t)rsdp->XsdtAddress;
		int entries = (xsdt->Header.Length - sizeof(acpi_header_t)) / 8;
		for (int i = 0; i < entries; i++) {
			acpi_header_t* table = (acpi_header_t*)(uintptr_t)xsdt->Tables[i];
			if (memcmp(table->Signature, signature, 4) == 0) {
				return (void*)table;
			}
		}
	} else {
		//ACPI 1.0 (RSDT)
		uint32_t* rsdt = (uint32_t*)(uintptr_t)rsdp->RsdtAddress;
		acpi_header_t* header = (acpi_header_t*)rsdt;
		int entries = (header->Length - sizeof(acpi_header_t)) / 4;
		uint32_t* tables = (uint32_t*)(rsdt + (sizeof(acpi_header_t) / 4));
		for (int i = 0; i < entries; i++) {
			acpi_header_t* table = (acpi_header_t*)(uintptr_t)tables[i];
			if (memcmp(table->Signature, signature, 4) == 0) {
				return (void*)table;
			}
		}
	}
	return NULL;
}

void madt_parse(acpi_header_t* madt) {
	uint8_t* ptr = (uint8_t*)(madt + 1);
	uint8_t* end = (uint8_t*)madt + madt->Length;

	//Пропускаем первые 8 байт (Local APIC Address и Flags)
	ptr += 8;

	while (ptr < end) {
		uint8_t type = ptr[0];
		uint8_t length = ptr[1];

		if (type == 0) { //Processor Local APIC
			uint8_t proc_id = ptr[2];
			uint8_t apic_id = ptr[3];
			uint32_t flags = *(uint32_t*)(ptr + 4);
			if (flags & 1) { //CPU активен
				//Тут будет списко доступных ядер
			}
		} 
		else if (type == 1) { //IOAPIC
			uint32_t ioapic_addr = *(uint32_t*)(ptr + 4);
			g_ioapic_base = ioapic_addr; 
		} 
		else if (type == 2) { //Interrupt Source Override
			uint8_t bus = ptr[2];
			uint8_t irq = ptr[3];
			uint32_t gsi = *(uint32_t*)(ptr + 4); //Куда он воткнут в IOAPIC
            
			print_color("[MADT] IRQ ", 0x00FF00);
			print_hex64(irq);
			print_color(" redirected to GSI ", 0x00FF00);
			print_hex64(gsi);
			print_color("\n", 0x00FF00);

		}

		ptr += length;
		if (length == 0) { break; }
	}
}