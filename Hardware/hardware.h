#ifndef HARDWARE_H
#define HARDWARE_H

#include "../libs.h"
#include "../debug.h"
#include "../common.h"

#include "../Inst/instruction.h"
#include "../PhysMem/physMem.h"

class Hardware final
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

// SETTERS
    
	void setReg(RegId id, RegValue value) {regs_[id] = value;}

// Others

	void branch(RegValue target) {nextPc_ = target;}
	
    bool execute();

	bool write(PhysAddr addr, RegValue value, size_t size = kInsnSize) {return physMem_->write(addr, value, size);}
	bool read(PhysAddr addr, RegValue* value, size_t size = kInsnSize) {return physMem_->read(addr, value, size);}

	void Ebreak() { assert(0 && "EBREAK"); };
};



#endif 