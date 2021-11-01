#include "includes/libs.h"
#include "includes/debug.h"
#include "includes/test.h"

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

    EncodedInsn begInsn1 = 0b00000000100101000000011001100011; /* (if x8 == x9) jump + 12 */
    //EncodedInsn begInsn2 = 0b00000000100101001000010001100011; /* (if x8 == x8) jump + 8 */
    EncodedInsn jal      = 0b00000000100000000000011001101111; /* jump + [8 or 12; I can change], last pc saved in x12 */
    EncodedInsn add1     = 0b00000000000100001000000010110011;
    EncodedInsn add2     = 0b00000000000100010000000110110011;
    EncodedInsn srl      = 0b00000000000100011101000110110011;
    EncodedInsn sll      = 0b00000000000100011001000110110011;
    EncodedInsn testROMA = 0b11111111111100010000000100010011;

    physMem.write(0, begInsn1);
    physMem.write(4, jal);
    physMem.write(8, add1);
    physMem.write(12, add2);
    physMem.write(16, sll);
    physMem.write(20, testROMA);

    Hardware hardWare{&physMem, true};
    hardWare.setReg(kX1, 3);
    hardWare.setReg(kX2, 10000);
    hardWare.setReg(kX8, 13);
    hardWare.setReg(kX9, 12);


    auto result = hardWare.execute();

    std::cout << "result of execute() = " << result << std::endl; 

    std::cout << "kX3 = " << hardWare.getReg(kX3) << std::endl;
    std::cout << "kX2 = " << hardWare.getReg(kX2) << std::endl;

    /* TODO: register dump */


    exit(EXIT_SUCCESS);
}