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

    EncodedInsn addInsn = 0b0110011;

    physMem.write(0, addInsn);

    RegValue retval;
    physMem.read(0, &retval);

    //std::cout << "T write:" << addInsn;
    //std::cout << "\nT read : " << retval << std::endl;

    Hardware hardWare{&physMem};
    hardWare.setReg(kX0, 10);

    auto result = hardWare.execute();

    std::cout << "result of execute() = " << result << std::endl; 

    std::cout << "kX0 = " << hardWare.getReg(kX0) << std::endl;


    exit(EXIT_SUCCESS);
}