#ifndef HARDWARE_H
#define HARDWARE_H

#include "../libs.h"
#include "../debug.h"
#include "../common.h"

#include "../Inst/instruction.h"
#include "../PhysMem/physMem.h"

class Hardware
{
private:
    RegValue regs_[kRegFileGprSize];
	RegValue pc_;
	RegValue nextPc_;
	PhysMem *physMem_; // & => *

public:

	Hardware(PhysMem *physMem);

// GETTERS

    RegValue getReg(RegId id) const {return regs_[id];}
	
	const RegValue& pc() const {return pc_;}
	RegValue& pc() {return pc_;}
// SETTERS
    
	void setReg(RegId id, RegValue value) {regs_[id] = value;}

// Others

	void branch(RegValue target) {nextPc_ = target;}
	
    bool execute();
};



#endif 