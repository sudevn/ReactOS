gcc -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c kernal.cpp -o kernal.o
gcc -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c gdt.cpp -o gdt.o
gcc -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c port.cpp -o port.o
gcc -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c interrupts.cpp -o interrupts.o
as --32 loader.s -o loader.o
as --32 interruptsstubs.s -o interruptsstubs.o
ld -melf_i386 -T linker.ld loader.o gdt.o port.o kernal.o interruptsstubs.o interrupts.o -o mykernal.bin
cp mykernal.bin iso/boot/mykernal.bin
grub-mkrescue --output=mykernal.iso iso 
cp mykernal.iso /mnt/c/Users/Sudev/Desktop/mykernal.iso
rm -f kernal.o port.o gdt.o interruptsstubs.o interrupts.o mykernal.iso mykernal.bin