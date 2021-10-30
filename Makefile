.PHONY: all

all: MAIN CLEAN

#sem_cntl.o: sem_cntl.c
#			gcc -c $(FLAGS) -o sem_cntl.o sem_cntl.c

./PhysMem/physMem.o:
			g++ -c -o physMem.o PhysMem/physMem.cpp

./Inst/instruction.o:
			g++ -c -o instruction.o ./Inst/instruction.cpp

./Hardware/hardware.o:
			g++ -c -o hardware.o Hardware/hardware.cpp

main.o:	main.cpp
			g++ -c -o main.o main.cpp
MAIN: ./PhysMem/physMem.o ./Hardware/hardware.o ./Inst/instruction.o main.o 
			g++ $(flags) -o out main.o physMem.o hardware.o instruction.o $(def)

#WRITER: sem_cntl.o get_stuff.o writer.o
#			gcc -o writer sem_cntl.o get_stuff.o writer.o
#			rm -rf writer.o

CLEAN:
			rm -rf physMem.o
			rm -rf hardware.o
			rm -rf main.o
			rm -rf instruction.o