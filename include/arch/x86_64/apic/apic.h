#pragma once

#include <stdint.h>
#include <stddef.h>
#include "arch/x86_64/pic/pic.h"
#include "arch/x86_64/apic/lapic.h"
#include "arch/x86_64/apic/ioapic.h"
#include "arch/x86_64/apic/acpi.h"

void apic_init();