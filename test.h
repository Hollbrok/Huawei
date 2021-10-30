#ifndef TEST_H
#define TEST_H

#define NEED_TEST 0

#define TEST_MEM()          \
    do {                    \
        PhysMem virMem{};   \
                            \
        uint32_t testNum = 6666;    \
                                    \
        virMem.write(0, 4,testNum); \
        virMem.write(4, 4,testNum + 1u);    \
        virMem.write(8, 4,testNum + 2u);    \
                                            \
        uint32_t readVal = 0;               \
                                            \
        virMem.read(0, sizeof(uint32_t), &readVal);         \
        std::cout << "readVal = " << readVal << std::endl;  \
                                                            \
        virMem.read(4, 4, &readVal);                        \
        std::cout << "readVal = " << readVal << std::endl;  \
                                                            \
        virMem.read(8, 4, &readVal);                        \
        std::cout << "readVal = " << readVal << std::endl;  \
    } while(0)                                 

#endif