#include "libs.h"
#include "debug.h"
#include "test.h"

#include "PhysMem/physMem.h"
#include "Hardware/hardware.h"
#include "Inst/instruction.h"

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

    DEBPRINT("name of file: %s\n", fileName)

    if (NEED_TEST)
        TEST_MEM();

    auto retval = getBits<0, 0, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;

    retval = getBits<1, 0, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;

    retval = getBits<2, 0, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;

    retval = getBits<3, 0, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;

    retval = getBits<4, 0, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;

    retval = getBits<16, 5, RegValue>(15);
    std::cout << "retval = " << retval << std::endl;



    exit(EXIT_SUCCESS);
}