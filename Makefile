KERNEL_OUTPUT_FILE = ./build/kernel.asm.o

all: ./build/boot.bin ./build/kernel.bin
	rm -rf ./build/os.bin
	dd if=./build/boot.bin >> ./build/os.bin
	dd if=./build/kernel.bin >> ./build/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./build/os.bin
./build/kernel.bin : $(KERNEL_OUTPUT_FILE)
	i686-elf-ld -g -relocatable $(KERNEL_OUTPUT_FILE) -o ./build/kernel_full.o
	i686-elf-gcc -T ./src/linker_script.ld -o ./build/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernel_full.o

./build/boot.bin : ./src/boot.asm
	nasm -f bin ./src/boot.asm -o ./build/boot.bin

clean:
	rm -rf ./build/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin

./build/kernel.asm.o : ./src/kernel.asm
	nasm -f elf ./src/kernel.asm -o ./build/kernel.asm.o -g
