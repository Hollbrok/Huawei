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
    
    EncodedInsn mult[] = {
        0xfe010113,
        0x00112e23,
        0x00812c23,
        0x02010413,
        0x01300793,
        0xfef42623,
        0x00800793,
        0xfef42423,
        0xfe842583,
        0xfec42503,
        0x020000ef,
        0xfea42223,
        0x00000793,
        0x00078513,
        0x01c12083,
        0x01812403,
        0x02010113,
        0x00008067,
        /* /\ main
            |       */
        0xfd010113,
        0x02812623,
        0x03010413,
        0xfca42e23,
        0xfcb42c23,
        0xfe042623,
        0x0380006f,
        0xfd842783,
        0x0017f793,
        0x00078a63,
        0xfec42703,
        0xfdc42783,
        0x00f707b3,
        0xfef42623,
        0xfd842783,
        0x4017d793,
        0xfcf42c23,
        0xfdc42783,
        0x00179793,
        0xfcf42e23,
        0xfd842783,
        0xfc0794e3,
        0xfec42783,
        0x00078513,
        0x02c12403,
        0x03010113,
        0x00008067
    };
   
    for (int i = 0; i < 45; i++)
        physMem.write(65620 + i * 4, mult[i], 4);

    /*physMem.write(0, ,);
    physMem.write(4, jal);
    physMem.write(8, add1);
    physMem.write(12, add2);
    physMem.write(16, sll);
    physMem.write(20, testROMA);*/

    Hardware hardWare{&physMem, true};

    auto result = hardWare.execute();

    std::cout << "result of execute() = " << result << std::endl; 

    exit(EXIT_SUCCESS);
} 
