all: bin/os-image SO.iso
	make clean

test:bin/os-image
	qemu-system-x86_64 bin/os-image -m 128

bin/os-image: bin/boot_sect.bin bin/kernel.bin
	cat bin/boot_sect.bin bin/kernel.bin > bin/os-image

SO.iso: bin/os-image
	mkdir -p iso
	cp -v bin/os-image iso/
	mkisofs -no-emul-boot -b os-image -o ExoKernel.iso iso/
	make clean

bin/kernel.bin: sorcecode/c_kernel/kernel.c sorcecode/c_kernel/compiled.c sorcecode/asm_kernel/kernel_entry.asm
	nasm sorcecode/asm_kernel/kernel_entry.asm -f elf64 -o bin/entry.o
	gcc -O2 -c sorcecode/c_kernel/kernel.c -o bin/kernel.o -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function
	gcc -O2 -c sorcecode/c_kernel/compiled.c -o bin/compiled.o -std=gnu99 -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function
	make -C compiler
	gcc -O2 -c public/global_main.c -o bin/user_main.o -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function
	ld -e 0x7E00 -T sorcecode/link.ld -o bin/kernel.bin bin/entry.o bin/kernel.o bin/compiled.o bin/user_main.o

bin/boot_sect.bin: sorcecode/asm_kernel/bootloader/boot.asm bin
	nasm sorcecode/asm_kernel/bootloader/boot.asm -o bin/boot_sect.bin

bin:
	mkdir bin

clean:
	rm -rf iso
	rm bin/*
	rm bin/os-image
	rm public/global_main.c
