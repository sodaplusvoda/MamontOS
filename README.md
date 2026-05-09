# MamontOS x86_64

![MamontOS Banner](assets/mamontlogo.png)

Это моя учебная 64-битная Операционная Система написанная на Ассемблере NASM и C.

**Что уже реализовано:**
* **Загрузка:** Загрузчик с поддержкой Multiboot2.
* **Графика:** Работа с Framebuffer (32-битный цвет). Шрифт для текста брал с https://github.com/novariaos/novariaos-src.
* **System:** Настроен GDT.
* **Прерывания:** Обработка исключений процессора и аппаратных прерываний через APIC. Есть LAPIC, IOAPIC, ACPI и простая маршрутизация.

### Сборка
Для сборки требуется `gcc` (x86_64-elf), `nasm`, `binutils`, `grub`, `libisoburn`, `mtools` и `qemu` для запуска.