#ifndef PHYSMEM_H
#define PHYSMEM_H


#include "../includes/libs.h"
#include "../includes/debug.h"
#include "../includes/common.h"




class PhysMem
{
private:

    char mem_[kPhysMemSize] = {};

public:

    bool write(PhysAddr addr, RegValue value, size_t size = kInsnSize);
	bool read(PhysAddr addr, RegValue* value, size_t size = kInsnSize);
};

#endif