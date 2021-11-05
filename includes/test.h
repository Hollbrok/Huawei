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

/////////////////////////////////////////////////

#define MULT_INSNS          \
    EncodedInsn mult[] = {  \
        0xfe010113,         \
        0x00112e23,         \
        0x00812c23,         \
        0x02010413,         \
        0x01300793,         \
        0xfef42623,         \
        0x00800793,         \
        0xfef42423,         \
        0xfe842583,         \
        0xfec42503,         \
        0x020000ef,         \
        0xfea42223,         \
        0x00000793,         \
        0x00078513,         \
        0x01c12083,         \
        0x01812403,         \
        0x02010113,         \
        0x00008067,         \
        0xfd010113,         \
        0x02812623,         \
        0x03010413,         \
        0xfca42e23,         \
        0xfcb42c23,         \
        0xfe042623,         \
        0x0380006f,         \
        0xfd842783,         \
        0x0017f793,         \
        0x00078a63,         \
        0xfec42703,         \
        0xfdc42783,         \
        0x00f707b3,         \
        0xfef42623,         \
        0xfd842783,         \
        0x4017d793,         \
        0xfcf42c23,         \
        0xfdc42783,         \
        0x00179793,         \
        0xfcf42e23,         \
        0xfd842783,         \
        0xfc0794e3,         \
        0xfec42783,         \
        0x00078513,         \
        0x02c12403,         \
        0x03010113,         \
        0x00008067          \
    };
   
#define SUM_INSNS             \
    EncodedInsn sum[] = {     \
        0xe5010113,             \
        0x1a812623,             \
        0x1b010413,             \
        0xfe042623,             \
        0x0280006f,             \
        0xfec42783,             \
        0x00279793,             \
        0xff078793,             \
        0x008787b3,             \
        0xfec42703,             \
        0xe6e7a223,             \
        0xfec42783,             \
        0x00178793,             \
        0xfef42623,             \
        0xfec42703,             \
        0x06300793,             \
        0xfce7dae3,             \
        0xfe042423,             \
        0xfe042223,             \
        0x0300006f,             \
        0xfe442783,             \
        0x00279793,             \
        0xff078793,             \
        0x008787b3,             \
        0xe647a783,             \
        0xfe842703,             \
        0x00f707b3,             \
        0xfef42423,             \
        0xfe442783,             \
        0x00178793,             \
        0xfef42223,             \
        0xfe442703,             \
        0x06300793,             \
        0xfce7d6e3,             \
        0x00000793,             \
        0x00078513,             \
        0x1ac12403,             \
        0x1b010113,             \
        0x00008067,             \
    };

#define MEMSET_INSNS            \
    EncodedInsn Imemset[] = {    \
        0x00f00313,                 \
        0x00050713,                 \
        0x02c37e63,                 \
        0x00f77793,                 \
        0x0a079063,                 \
        0x08059263,                 \
        0xff067693,                 \
        0x00f67613,                 \
        0x00e686b3,                 \
        0x00b72023,                 \
        0x00b72223,                 \
        0x00b72423,                 \
        0x00b72623,                 \
        0x01070713,                 \
        0xfed766e3,                 \
        0x00061463,                 \
        0x00008067,                 \
        0x40c306b3,                 \
        0x00269693,                 \
        0x00000297,                 \
        0x005686b3,                 \
        0x00c68067,                 \
        0x00b70723,                 \
        0x00b706a3,                 \
        0x00b70623,                 \
        0x00b705a3,                 \
        0x00b70523,                 \
        0x00b704a3,                 \
        0x00b70423,                 \
        0x00b703a3,                 \
        0x00b70323,                 \
        0x00b702a3,                 \
        0x00b70223,                 \
        0x00b701a3,                 \
        0x00b70123,                 \
        0x00b700a3,                 \
        0x00b70023,                 \
        0x00008067,                 \
        0x0ff5f593,                 \
        0x00859693,                 \
        0x00d5e5b3,                 \
        0x01059693,                 \
        0x00d5e5b3,                 \
        0xf6dff06f,                 \
        0x00279693,                 \
        0x00000297,                 \
        0x005686b3,                 \
        0x00008293,                 \
        0xfa0680e7,                 \
        0x00028093,                 \
        0xff078793,                 \
        0x40f70733,                 \
        0x00f60633,                 \
        0xf6c378e3,                 \
        0xf3dff06f,                 \
    };

/////////////////////////////////////////////////

#define MULT_EXAMPLE                                                \
    MULT_INSNS                                                      \
    RegValue startPc = 65620;                                   \
    for (int i = 0; i < sizeof(mult) / sizeof(EncodedInsn); i++)    \
        physMem.write(65620 + i * 4, mult[i], 4);

#define SUM_EXAMPLE                                                 \
    SUM_INSNS                                                       \
    RegValue startPc = 65852;                                       \
    for (int i = 0; i < sizeof(sum) / sizeof(EncodedInsn); i++)     \
        physMem.write(startPc + i * 4, sum[i], 4);                  
    
    //for (int i = 0; i < sizeof(Imemset) / sizeof(EncodedInsn); i++) 
    //    physMem.write(66244 + i * 4, Imemset[i], 4); 
            
#endif