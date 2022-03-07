VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all

CFLAGS = -g -MD -c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

DISA_CODE_PATH = Disassembler
ASM_CODE_PATH = Assembler
STK_CODE_PATH = StackLib

TEST_PATH = tests


DISA_SOURCES = $(DISA_CODE_PATH)/dis_main.cpp $(DISA_CODE_PATH)/disassembler_func.cpp
ASM_SOURCES  = $(ASM_CODE_PATH)/main_assem.cpp $(ASM_CODE_PATH)/assem_func.cpp
STK_SOURCES  = $(STK_CODE_PATH)/stack_func.cpp

DISA_OBJ = $(DISA_SOURCES:.cpp=.o)
ASM_OBJ = $(ASM_SOURCES:.cpp=.o)
STK_OBJ = $(STK_SOURCES:.cpp=.o)

OBJECTS = $(SOURCES:.c=.o)

DISA_EXECUTABLE = disa
ASM_EXECUTABLE  = asm

# -------------------------------

all: asm disasm $(DISA_EXECUTABLE) $(ASM_EXECUTABLE)#cpu

# -------------------------------

disasm: $(DISA_SOURCES) $(DISA_OBJ) stk

$(DISA_EXECUTABLE): $(DISA_OBJ)
	$(CC) $(DISA_OBJ) $(STK_OBJ) -lstdc++ -o $@

# -------------------------------

asm: $(ASM_SOURCES) $(ASM_OBJ)

$(ASM_EXECUTABLE): $(ASM_OBJ)
	$(CC) $(ASM_OBJ) -lstdc++ -o $@

# -------------------------------

stk: $(STK_SOURCES) $(STK_OBJ)

# -------------------------------


%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# -------------------------------

run_test :
	sh tests/run_tests.sh && tests/

valgrind : $(SOURCES)
	valgrind $(VALGRIND_FLAGS) tests/test   

# valgrind --leak-check=full --show-leak-kinds=all ./xxx 

-include *.d

.PHONY: clean

clean: 
		rm -rf *.o *.d *.txt *.holl


