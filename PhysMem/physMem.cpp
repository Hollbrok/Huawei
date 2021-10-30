#include "physMem.h"


bool PhysMem::write(PhysAddr addr, RegValue value, size_t size)
{
    if (addr + size > kPhysMemSize)
    {
        DEBPRINT("address + size of value > kPhysMemSize\n"
                 "%zu + %zu  > %zu\n", addr, size, kPhysMemSize)
        return 1;
    }

    void *retval = memcpy(&mem_[addr], &value, size);

    if (DEBUG_REGIME)
        std::cout << "retval of memcpy[write] = " << retval << std::endl;

    return 0;
}

/* return 1 on error (0 in success) */
bool PhysMem::read(PhysAddr addr, RegValue* value, size_t size)
{
    if (addr + size > kPhysMemSize)
    {
        DEBPRINT("address + size of value > kPhysMemSize\n"
                 "%zu + %zu  > %zu ", addr, size, kPhysMemSize)
        return 1;
    }

    void *retval = memcpy(value, &mem_[addr], size);
    
    if (DEBUG_REGIME)
        std::cout << "retval of memcpy[read] = " << retval << std::endl;


    return 0;
}