#ifndef PHYSMEM_H
#define PHYSMEM_H


#include "../libs.h"
#include "../debug.h"
#include "../common.h"




class PhysMem
{
private:

    char mem_[kPhysMemSize];

public:

    bool write(PhysAddr addr, size_t size, RegValue value);
	bool read(PhysAddr addr, size_t size, RegValue* value);
};

#endif