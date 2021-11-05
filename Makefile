.PHONY: all

flags = -lelf

all: MAIN CLEAN

./PhysMem/physMem.o:
			g++ -c -o physMem.o PhysMem/physMem.cpp

./Inst/instruction.o:
			g++ -c -o instruction.o ./Inst/instruction.cpp

./Hardware/hardware.o:
			g++ -c -o hardware.o Hardware/hardware.cpp
		
./ElfParsing/elfParsing.o:
			g++ -c $(flags) -o elfParsing.o ElfParsing/elfParsing.cpp 

main.o:	main.cpp
			g++ -c -o main.o main.cpp

MAIN: ./PhysMem/physMem.o ./Hardware/hardware.o ./Inst/instruction.o ./ElfParsing/elfParsing.o main.o 
			g++ $(flags) -o out main.o physMem.o hardware.o instruction.o elfParsing.o $(def)

CLEAN:
			rm -rf physMem.o
			rm -rf hardware.o
			rm -rf main.o
			rm -rf instruction.o
			rm -rf elfParsing.o
			clear