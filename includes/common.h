#ifndef COMMON_H
#define COMMON_H

constexpr size_t kPhysMemSize = 8192 * 32;
constexpr size_t kInsnSize = 4;
constexpr size_t regBitLength = 32;

typedef uint32_t PhysAddr;
typedef uint32_t VirtAddr;

typedef uint32_t RegValue;
typedef int32_t SRegValue;
typedef uint32_t EncodedInsn;


enum RegId
{
    kX0,
	kX1,
	kX2,
    kX3,
    kX4,
    kX5,
    kX6,
    kX7,
    kX8,
    kX9,
    kX10,
    kX11,
    kX12,
    kX13,
    kX14,
    kX15,
    kX16,
    kX17,
    kX18,
    kX19,
    kX20,
    kX21,
    kX22,
    kX23,
    kX24,
    kX25,
    kX26,
    kX27,
    kX28,
    kX29,
    kX30,
	kX31,
	kRegIdZero = kX0,
    kRegStackP = kX2,
    kRegFileGprSize = 32,
    kErrVal = -1,
};



#endif