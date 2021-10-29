#include "libs.h"
#include "debug.h"


#include "PhysMem/physMem.h"

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

    PhysMem virMem{};

    uint32_t testNum = 6666;

    virMem.write(0, 4,testNum);
    virMem.write(4, 4,testNum + 1u);
    virMem.write(8, 4,testNum + 2u);

    uint32_t readVal = 0;

    virMem.read(0, sizeof(uint32_t), &readVal);
    std::cout << "readVal = " << readVal << std::endl;
    
    virMem.read(4, 4, &readVal);
    std::cout << "readVal = " << readVal << std::endl;

    virMem.read(8, 4, &readVal);
    std::cout << "readVal = " << readVal << std::endl;

    //printf("testNum = %zu\n", readVal);

    exit(EXIT_SUCCESS);
}