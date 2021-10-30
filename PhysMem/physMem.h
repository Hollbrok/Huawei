#ifndef PHYSMEM_H
#define PHYSMEM_H


#include "../libs.h"
#include "../debug.h"
#include "../common.h"




class PhysMem
{
private:

    char mem_[kPhysMemSize] = {};

public:

    bool write(PhysAddr addr, RegValue value, size_t size = kInsnSize);
	bool read(PhysAddr addr, RegValue* value, size_t size = kInsnSize);
};

#endif