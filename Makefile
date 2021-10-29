.PHONY: all

all: MAIN

#sem_cntl.o: sem_cntl.c
#			gcc -c $(FLAGS) -o sem_cntl.o sem_cntl.c

#writer.o: writer.c
#			gcc -c $(FLAGS) -o writer.o writer.c

MAIN: #sem_cntl.o get_stuff.o reader.o
			g++ $(flags) main.cpp $(def) -o out

#WRITER: sem_cntl.o get_stuff.o writer.o
#			gcc -o writer sem_cntl.o get_stuff.o writer.o
#			rm -rf writer.o

#CLEAN:
#			rm -rf sem_cntl.o
#			rm -rf get_stuff.o