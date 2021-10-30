#ifndef INSTR_H
#define INSTR_H

#include "../libs.h"
#include "../debug.h"
#include "../common.h"


class Hardware;
class Instruction;

typedef void (*Executor)(Hardware* hart, const Instruction& insn);

enum InsnClass : uint8_t
{
    kInsnAdd,
    kInsnBeq,
};

template<int hi,int lo, class T = RegValue>
T getBits(T value)
{
    return (((value >> lo) << lo) << (sizeof(T) * 8 - hi - 1)) >> (sizeof(T) * 8 - hi - 1); 
}

class Instruction
{
private:
    Executor executor_;
    InsnClass insnType_;
	RegId rd_, rs1_, rs2_;
	RegValue imm_;

public:

    Instruction(EncodedInsn insn, RegValue pc)
	{
	    // TODO: decode format before individual instructions
	    // decoder is is the only place where raw constants are acceptable(!)
	    switch (insn & 0x7F)
		{
		case 0:
		    break;
		// R-format insns
		case 0b0110011:
        {
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			auto funct3 = getBits<14, 12>(insn);
			auto funct7 = getBits<31, 25>(insn);
			
			if (funct3 == 0 && funct7 == 0) // ADD
			{
				std::cout << "ADD FINDED\n";
			    insnType_ = kInsnAdd;
				//executor_ = &executeAdd;
			}
			//else if (...)
			//{
			//}
			else
				assert (0);
		    break;
        }
        case 0b1100011: // BEQ/BNE
		{
            rd_  = static_cast<RegId> (kRegIdZero);
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			//imm_ = ...;
			
			
			auto funct3 = getBits<14, 12>(insn);
			
			imm_ = imm_ + pc;
			
			if (funct3 == 0)
			{
			    insnType_ = kInsnBeq;
				//executor_ = &executeBeq;
			}
			/*else if (...)
			{
			}*/
			else
				assert (0);
		    break;
        }
		}
	}

// GETTERS

    RegId rd()  const {return rd_;} 
    RegId rs1() const {return rs1_;}
    RegId rs2() const {return rs2_;}

    RegValue imm() const {return imm_;}

	InsnClass insn() const { return insnType_;}

// OTHER

	void executeAdd(Hardware* harw);
	void executeBeq(Hardware* harw);

	void executor(Hardware* hardw) 
	{
		switch (insnType_)
		{
		case kInsnAdd:
			executeAdd(hardw);
			break;
		default:
			std::cout << "default insn Type\n";
			break;
		}
	} 

};

#endif 