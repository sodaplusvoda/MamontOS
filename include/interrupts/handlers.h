#pragma once

#include "debug/print.h"
#include "arch/x86_64/pic/pic.h"
#include "arch/x86_64/apic/apic.h"
#include <stdint.h>

struct interrupt_frame {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
	uint64_t int_no, error_code;
	uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

void exception_handler(struct interrupt_frame* frame);
void page_fault_handler(struct interrupt_frame* frame);
void isr_handler(struct interrupt_frame* frame);