#include "instruction.h"
#include "../Hardware/hardware.h"

void Instruction::executeAdd(Hardware* harw)
{
    auto rs1value = harw->getReg(rs1_);
	auto rs2value = harw->getReg(rs2_);
	
	auto result = rs1value + rs2value;
	
	std::cout << "result in ADD = " << result << std::endl;

	harw->setReg(rd_, result);
}

void Instruction::executeBeq(Hardware* harw)
{
    auto rs1value = harw->getReg(rs1_);
	auto rs2value = harw->getReg(rs2_);
	
	if (rs1value == rs2value)
	    harw->branch(imm_);
	
	//harw->setReg(insn.rd(), result);
}