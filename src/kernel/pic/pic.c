#include "pic/pic.h"

void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ( "outb %b0, %w1" 
		: : "a"(val), "Nd"(port) 
		: "memory");
}

uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ( "inb %w1, %b0"
		: "=a"(ret)
		: "Nd"(port)
		: "memory");
	return ret;
}

static inline void io_wait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

void pic_sendeoi(uint8_t irq) {
	if (irq >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint8_t irq_line) {
    uint16_t port;
    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    outb(port, inb(port) | (1 << irq_line));
}

void pic_clear_mask(uint8_t irq_line) {
    uint16_t port;
    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    outb(port, inb(port) & ~(1 << irq_line));
}

void pit_init(uint32_t frequency) {
	uint32_t divisor = 1193182 / frequency;

	//Отправляем команду: Канал 0 режим 3 Square Wave Generator, формат Low/High байт
	outb(0x43, 0x36);

	//Отправляем делитель по частям
	outb(0x40, (uint8_t)(divisor & 0xFF)); //Младший байь
	outb(0x40, (uint8_t)((divisor >> 8) & 0xFF)); //Старший буйт
}

void pic_remap(uint8_t offset1, uint8_t offset2) {
	uint8_t a1, a2;
	//Сохраняем текущие маски
	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	//Установка смещения векторов (Offsets)
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();

	//Настройка связки Master/Slave
	outb(PIC1_DATA, 4);  //Сообщаем Master, что Slave на IRQ2
	io_wait();
	outb(PIC2_DATA, 2);  //Сообщаем Slave его каскадный ID (2)
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}