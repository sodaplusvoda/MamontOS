#include <arch/x86_64/apic/acpi.h>
#include <arch/x86_64/apic/lapic.h>
#include <arch/x86_64/apic/ioapic.h>
#include <arch/x86_64/pic/pic.h>

void apic_init() {
	acpi_rsdp_t* rsdp = acpi_find_rsdp();
	if (!rsdp) {
		print_color("[APIC] RSDP not found\n", 0x00FF00);
		return;
	}
	void* madt = acpi_find_table(rsdp, "APIC");
	if (!madt) {
		print_color("[APIC] MADT table not found\n", 0x00FF00);
		return;
	}
	madt_parse((acpi_header_t*)madt);
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
	print_color("[PIC] Disabled\n", 0x00FF00);
	lapic_init();
	ioapic_init();
}