#include "includes/libs.h"
#include "includes/debug.h"
#include "includes/test.h"
#include "PhysMem/physMem.h"
#include "Hardware/hardware.h"
#include "Inst/instruction.h"
#include "ElfParsing/elfParsing.h"

constexpr int MAX_FN_SIZE = 20;

int main(int argc, char *argv[])
{
    char fileName[MAX_FN_SIZE] = {};
    if (argc < 2 || strlen(argv[1]) > MAX_FN_SIZE)
    {
        fprintf(stderr, "no file provided\n");
        exit(EXIT_FAILURE);
    }
    strcpy(fileName, argv[1]);
    DEBPRINT("name of file: %s\n", argv[1]);
    if (NEED_TEST)
    {
        TEST_MEM();
        TEST_GETBITS();
    }

    elfPars(fileName);
    fprintf(stderr, "after parsing\n");

    PhysMem physMem{};

    //MULT_EXAMPLE
    SUM_EXAMPLE


    Hardware hardWare{&physMem, startPc, true};

    auto result = hardWare.execute();

    std::cout << "result of execute() = " << result << std::endl; 

    exit(EXIT_SUCCESS);
} 
