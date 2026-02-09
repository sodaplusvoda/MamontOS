mkdir -p build

nasm -f elf64 src/bootloader/header.asm -o build/header.o
nasm -f elf64 src/bootloader/boot.asm -o build/boot.o
nasm -f elf64 src/bootloader/boot64.asm -o build/boot64.o
nasm -f elf64 src/bootloader/gdt_asm.asm -o build/gdt_asm.o
nasm -f elf64 src/bootloader/interrupts.asm -o build/interrupts.o

x86_64-elf-gcc -c src/kernel/debug/print.c -o build/print.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/interrupts/idt.c -o build/idt.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/interrupts/handlers.c -o build/handlers.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/gdt/gdt.c -o build/gdt.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/pic/pic.c -o build/pic.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/bitmap.c -o build/bitmap.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/string.c -o build/string.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/alloc/early_alloc.c -o build/early_alloc.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/alloc/buddy_alloc.c -o build/buddy_alloc.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/pmm.c -o build/pmm.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/mem/vmm.c -o build/vmm.o -ffreestanding -O2 -Wall -Iinclude
x86_64-elf-gcc -c src/kernel/kernel.c -o build/kernel.o -ffreestanding -O2 -Wall -Iinclude

ld -m elf_x86_64 -T src/linker.ld \
   -o build/kernel.bin build/gdt_asm.o build/interrupts.o build/header.o build/boot.o build/boot64.o build/print.o build/gdt.o build/idt.o build/handlers.o build/pic.o build/bitmap.o build/string.o build/early_alloc.o build/buddy_alloc.o build/pmm.o build/vmm.o build/kernel.o

mkdir -p iso/boot/grub
cp build/kernel.bin iso/boot/

cat > iso/boot/grub/grub.cfg << EOF
set timeout=0
set default=0

menuentry "MamontOS (x86_64)" {
    multiboot2 /boot/kernel.bin
    boot
}
EOF

grub-mkrescue -o MamontOS.iso iso
grub-file --is-x86-multiboot2 build/kernel.bin
qemu-system-x86_64 -cdrom MamontOS.iso -m 512M
