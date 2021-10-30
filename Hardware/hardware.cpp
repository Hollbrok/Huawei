#include "hardware.h"

Hardware::Hardware(PhysMem *physMem) :
	regs_    {},
	pc_      {0},
	nextPc_  {0},
	physMem_ {physMem}
{
}

bool Hardware::execute()
{
    while(true)
    {
		EncodedInsn insnCode{};

        if (physMem_->read(pc_, &insnCode)) // 3rd arg = kInsnSize
		{    // TODO: exception
		     return false;
		}

		if (insnCode == 0) /* no more commands */
			return true;
		
		Instruction insn(insnCode, pc_);

		nextPc_ = pc_ + kInsnSize;
		insn.executor(this);
		pc_ = nextPc_; 
		std::cout << "after execute inst pc = " << pc_ << std::endl;
    }

    return true;
}