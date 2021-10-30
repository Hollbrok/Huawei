#include "physMem.h"

/*
    constexpr size_t kPhysMemSize = 4096;
    constexpr size_t kInsnSize = 4;

    typedef uint32_t PhysAddr;
    typedef uint32_t VirtAddr;

    typedef uint32_t RegValue;
    typedef uint32_t EncodedInsn;

*/

/* returns 1 on error (0 in success) */
bool PhysMem::write(PhysAddr addr, size_t size, RegValue value)
{
    if (addr + size > kPhysMemSize)
    {
        DEBPRINT("address + size of value > kPhysMemSize\n"
                 "%zu + %zu  > %zu\n", addr, size, kPhysMemSize)
        return 1;
    }

    void *retval = memcpy(&mem_[addr], &value, size);

    std::cout << "retval of memcpy[write] = " << retval << std::endl;

    return 0;
}

/* return 1 on error (0 in success) */
bool PhysMem::read(PhysAddr addr, size_t size, RegValue* value)
{
    if (addr + size > kPhysMemSize)
    {
        DEBPRINT("address + size of value > kPhysMemSize\n"
                 "%zu + %zu  > %zu ", addr, size, kPhysMemSize)
        return 1;
    }

    void *retval = memcpy(value, &mem_[addr], size);
    std::cout << "retval of memcpy[read] = " << retval << std::endl;


    return 0;
}