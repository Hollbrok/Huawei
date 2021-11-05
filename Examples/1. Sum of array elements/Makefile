file = main

all: $(file).c
	riscv32-unknown-elf-gcc -S $(file).c -o $(file).s
	riscv32-unknown-elf-gcc $(file).s -o $(file)
	riscv32-unknown-elf-objdump -S $(file) > $(file).dump
