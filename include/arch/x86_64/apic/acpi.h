#pragma once

#include <stdint.h>

//RSDP (Root System Description Pointer)
typedef struct {
	char Signature[8]; //RSD PTR
	uint8_t Checksum;
	char OEMID[6];
	uint8_t Revision; //0 - ACPI 1.0, 2 - ACPI 2.0
	uint32_t RsdtAddress; //Физический адрес RSDT для 32-битных систем)
    
	//Поля ниже доступны только если Revision больше или равен 2
	uint32_t Length;
	uint64_t XsdtAddress; //Физический адрес XSDT для 64-битных систем
	uint8_t ExtendedChecksum;
	uint8_t Reserved[3];
} __attribute__((packed)) acpi_rsdp_t;

//Общий заголовок всех таблиц ACPI (SDT - System Description Table)
typedef struct {
	char Signature[4]; //Например, "APIC" для MADT
	uint32_t Length; //Полная длина таблицы, включая заголовок
	uint8_t Revision;
	uint8_t Checksum;
	char OEMID[6];
	char OEMTableID[8];
	uint32_t OEMRevision;
	uint32_t CreatorID;
	uint32_t CreatorRevision;
} __attribute__((packed)) acpi_header_t;

//XSDT (Extended System Description Table)
typedef struct {
	acpi_header_t Header;
	uint64_t Tables[]; //Массив указателей на другие таблицы
} __attribute__((packed)) acpi_xsdt_t;

//Функция для поиска RSDP в физической памяти.
acpi_rsdp_t* acpi_find_rsdp();

void* acpi_find_table(acpi_rsdp_t* rsdp, char* signature);
void madt_parse(acpi_header_t* madt);