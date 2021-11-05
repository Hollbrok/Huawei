#ifndef HARDWARE_H
#define HARDWARE_H

#include "../includes/libs.h"
#include "../includes/debug.h"
#include "../includes/common.h"

#include "../Inst/instruction.h"
#include "../PhysMem/physMem.h"

class Hardware final
{ 
private:
    RegValue regs_[kRegFileGprSize];
	RegValue pc_;
	RegValue nextPc_;
	PhysMem *physMem_; // & => *
	bool needDebug_;

public:

	Hardware(PhysMem *physMem, RegValue pc, bool needDebug = false);

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

	void regDump() 
	{ 
		std::cout << "///////////////////////////////////////////////\n" << std::endl;

		for (size_t i = 0; i < kRegFileGprSize; ++i)
			std::cout << "x" << i << " = " << regs_[i] << std::endl;		

		std::cout << "///////////////////////////////////////////////\n" << std::endl;	
	}
};



#endif 