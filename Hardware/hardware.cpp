#include "hardware.h"

Hardware::Hardware(PhysMem *physMem, bool needDebug) :
	regs_    {},
	pc_      {0},
	nextPc_  {0},
	physMem_ {physMem},
	needDebug_{needDebug}
{
}

bool Hardware::execute()
{
    while(true)
    {
		EncodedInsn insnCode{};

        if (physMem_->read(pc_, &insnCode)) // 3rd arg = kInsnSize
		{    // TODO: exception
			std::cout << "/////////////////////////////////////////////// \n" << std::endl;
		     return false;
		}

		if (insnCode == 0) /* no more commands */
			break;
		
		Instruction insn(insnCode, pc_);
		insn.setDebugRegime(needDebug_);

		nextPc_ = pc_ + kInsnSize;
		
		if (needDebug_)
			std::cout << "/////////////////////////////////////////////// \n\n\t\t insn PC = " << pc_ << std::endl << std::endl;
		
		if (!insn.executor(this))
		{
				std::cout << "/////////////////////////////////////////////// \n" << std::endl;
				return false;
		}
		pc_ = nextPc_; 
		
		if (needDebug_)
			std::cout << std::endl;
    }

	std::cout << "///////////////////////////////////////////////\n" << std::endl;

    return true;
}