# Сборка
    [ ] git clone https://github.com/Hollbrok/Huawei
    [ ] cd ./Huawei
    [ ] make
    [ ] ./out [ELF_FILE]

At this moment there is no parsing of __elf-file__ and the executed instructions are written to memory manually.

## realized opportunities

- [X] Debug regime: printing executable instructions with the values of registers used / modified by current instruction.

- [X] Almost all __RV32I Base Instruction Set__ are supported:

__ Add, Sub, Sll, Slt, Sltu, Xor, Srl, Sra, Or, And, Beq, Bne, Blt, Bge, Bltu, Bgeu, Lb, Lh, Lw, Lbu, Lhu, Ecall, Ebreak, Sb, Sh, Sw, Addi, Slti, Sltiu, Xori, Ori, Andi, Slli, Srli, Srai, Jal, Jalr, Auipc, Lui__.

# TODO

- [ ] Parsing of __elf-file__.

- [ ] Correct support of processing the __x0__ register.

- [ ] Опция симулятора для печати исполняемых инструкций со значениями регистров используемых/модифицируемых инструкцией. Поощряются обобщённые решения (сложный вариант).


