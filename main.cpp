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
    {
        TEST_MEM();
        TEST_GETBITS();
    }

    PhysMem physMem{};

    EncodedInsn begInsn = 0b00000000100101000000010001100011;
    EncodedInsn add1    = 0b00000000000100001000000010110011;
    EncodedInsn add2    = 0b00000000000100010000000110110011;

    physMem.write(0, begInsn);
    physMem.write(4, add1);
    physMem.write(8, add2);

    RegValue retval;

    //std::cout << "T write:" << addInsn;
    //std::cout << "\nT read : " << retval << std::endl;

    Hardware hardWare{&physMem};
    hardWare.setReg(kX1, 2);
    hardWare.setReg(kX2, 5);
    hardWare.setReg(kX8, 11);
    hardWare.setReg(kX9, 11);


    auto result = hardWare.execute();

    std::cout << "result of execute() = " << result << std::endl; 

    std::cout << "kX3 = " << hardWare.getReg(kX3) << std::endl;


    exit(EXIT_SUCCESS);
}