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
        virMem.read(0, &readVal);           \
        std::cout << "readVal = " << readVal << std::endl;  \
                                                            \
        virMem.read(4, &readVal);                           \
        std::cout << "readVal = " << readVal << std::endl;  \
                                                            \
        virMem.read(8, &readVal);                           \
        std::cout << "readVal = " << readVal << std::endl;  \
    } while(0)                                 

#define TEST_GETBITS()                                      \
    do {                                                    \
        auto retval = getBits<0, 0, RegValue>(15);          \
        std::cout << "retval = " << retval << std::endl;    \
                                                            \
        retval = getBits<1, 0, RegValue>(15);               \
        std::cout << "retval = " << retval << std::endl;    \
                                                            \
        retval = getBits<2, 0, RegValue>(15);               \
        std::cout << "retval = " << retval << std::endl;    \
                                                            \
        retval = getBits<3, 0, RegValue>(15);               \
        std::cout << "retval = " << retval << std::endl;    \
                                                            \
        retval = getBits<4, 0, RegValue>(15);               \
        std::cout << "retval = " << retval << std::endl;    \
                                                            \
        retval = getBits<16, 5, RegValue>(15);              \
        std::cout << "retval = " << retval << std::endl;    \
    } while(0)
    



#endif