#include "../libs.h"
#include "../debug.h"


constexpr size_t kPhysMemSize = 4096;
constexpr size_t kInsnSize = 4;

typedef uint32_t PhysAddr;
typedef uint32_t VirtAddr;

typedef uint32_t RegValue;
typedef uint32_t EncodedInsn;


class PhysMem
{
private:

    char mem_[kPhysMemSize];

public:

    bool write(PhysAddr addr, size_t size, RegValue value);
	bool read(PhysAddr addr, size_t size, RegValue* value);
};