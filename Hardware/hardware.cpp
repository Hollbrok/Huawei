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

	setReg(kRegStackP, 8192 * 16 - 21);
	pc_ = 65620;
    while(true)
    {
		EncodedInsn insnCode{};

        if (physMem_->read(pc_, &insnCode)) // 3rd arg = kInsnSize
		{    // TODO: exception
			std::cout << "NO MORE INSNS!!!!!!" << std::endl;
			std::cout << "/////////////////////////////////////////////// \n" << std::endl;
		    regDump();
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
				std::cout << "CRASH EXECUTOR!!!!!!" << std::endl;
				return false;
		}
		pc_ = nextPc_; 
		setReg(kRegIdZero, 0);
		
		if (needDebug_)
			std::cout << std::endl;
    }

	std::cout << "\n///////////////////////////////////////////////\n" << std::endl;

	regDump();

    return true;
}


