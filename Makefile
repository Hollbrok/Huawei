.PHONY: all

all: MAIN

#sem_cntl.o: sem_cntl.c
#			gcc -c $(FLAGS) -o sem_cntl.o sem_cntl.c

physMem.o:
			g++ -c -o physMem.o PhysMem/physMem.cpp

main.o:	main.cpp
			g++ -c -o main.o main.cpp
MAIN: physMem.o main.o
			g++ $(flags) -o out main.o physMem.o $(def)

#WRITER: sem_cntl.o get_stuff.o writer.o
#			gcc -o writer sem_cntl.o get_stuff.o writer.o
#			rm -rf writer.o

#CLEAN:
#			rm -rf sem_cntl.o
#			rm -rf get_stuff.o