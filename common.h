#ifndef COMMON_H
#define COMMON_H

constexpr size_t kPhysMemSize = 4096;
constexpr size_t kInsnSize = 4;

typedef uint32_t PhysAddr;
typedef uint32_t VirtAddr;

typedef uint32_t RegValue;
typedef uint32_t EncodedInsn;


enum RegId
{
    kX0,
	kX1,
	kx2,
    kx3,
    kx4,
    kx5,
    kx6,
    kx7,
    kx8,
    kx9,
    kx10,
    kx11,
    kx12,
    kx13,
    kx14,
    kx15,
    kx16,
    kx17,
    kx18,
    kx19,
    kx20,
    kx21,
    kx22,
    kx23,
    kx24,
    kx25,
    kx26,
    kx27,
    kx28,
    kx29,
    kx30,
	kX31,
	kRegIdZero = kX0,
    kRegFileGprSize = 32,
};



#endif