all:
	# nasm -f elf32 ./protected_mode.asm -o ./protected.o
	# nasm -f elf32 ./boot.asm -o ./boot.o
	# ld -m elf_i386 protected.o boot.o -o boot
	# dd if=./protected.bin >> ./boot.bin
	nasm -f bin ./boot.asm -o ./boot.bin
	# dd if=./hello.txt >> ./boot.bin
	# dd if=/dev/zero bs=512 count=1 >> boot.bin
clean:
	rm -rf ./boot.bin