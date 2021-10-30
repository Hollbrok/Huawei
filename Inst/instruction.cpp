#include "instruction.h"
#include "../Hardware/hardware.h"

void executeAdd(Hardware* harw, const Instruction& insn)
{
    auto rs1value = harw->getReg(insn.rs1());
	auto rs2value = harw->getReg(insn.rs2());
	
	auto result = rs1value + rs2value;
	
	harw->setReg(insn.rd(), result);
}

void executeBeq(Hardware* harw, const Instruction& insn)
{
    auto rs1value = harw->getReg(insn.rs1());
	auto rs2value = harw->getReg(insn.rs2());
	
	if (rs1value == rs2value)
	    harw->branch(insn.imm());
	
	//harw->setReg(insn.rd(), result);
}