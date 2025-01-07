KERNEL_OUTPUT_FILE = ./build/kernel.asm.o ./build/kernel.o ./build/idt.o ./build/idt.asm.o ./build/memory.o ./build/io.asm.o ./build/heap.o ./build/kheap.o
INCLUDES = -I ./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./build/boot.bin ./build/kernel.bin
	rm -rf ./build/os.bin
	dd if=./build/boot.bin >> ./build/os.bin
	dd if=./build/kernel.bin >> ./build/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./build/os.bin
	
./build/kernel.bin : $(KERNEL_OUTPUT_FILE)
	i686-elf-ld -g -relocatable $(KERNEL_OUTPUT_FILE) -o ./build/kernel_full.o
	i686-elf-gcc $(FLAGS) -T ./src/linker_script.ld -o ./build/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernel_full.o

./build/boot.bin : ./src/boot.asm
	nasm -f bin ./src/boot.asm -o ./build/boot.bin

clean:
	rm -rf ./build/boot.bin
	rm -rf ./build/kernel.bin
	rm -rf ./build/kernel_full.o
	rm -rf ./build/kernel.asm.o
	rm -rf ./build/os.bin

./build/kernel.asm.o : ./src/kernel.asm
	nasm -f elf ./src/kernel.asm -o ./build/kernel.asm.o -g

./build/io.asm.o : ./src/io.asm
	nasm -f elf ./src/io.asm -o ./build/io.asm.o -g

./build/idt.asm.o: ./src/idt.asm
	nasm -f elf ./src/idt.asm -o ./build/idt.asm.o -g

./build/memory.o: ./src/memory.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/memory.c -o ./build/memory.o

./build/idt.o: ./src/idt.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/idt.c -o ./build/idt.o

./build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/heap.o: ./src/heap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/heap.c -o ./build/heap.o

./build/kheap.o: ./src/kheap.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kheap.c -o ./build/kheap.o