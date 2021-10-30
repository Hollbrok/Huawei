#ifndef HARDWARE_H
#define HARDWARE_H

#include "../libs.h"
#include "../debug.h"
#include "../common.h"

class PhysMemory;

class Hardware
{
private:
    RegValue regs_[kRegFileGprSize];
	RegValue pc_;
	RegValue nextPc_;
	PhysMemory& physMemory_; // & => *

public:

// GETTERS

    RegValue getReg(RegId id) const {return regs_[id];}
	
	RegValue getPc() const {return pc_;}
	
// SETTERS
    
	void setReg(RegId id, RegValue value) {regs_[id] = value;}

// Others

	void branch(RegValue target) {nextPc_ = target;}
	
    bool execute();
};



#endif 