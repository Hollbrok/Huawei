#ifndef INSTR_H
#define INSTR_H

#include "../libs.h"
#include "../debug.h"
#include "../common.h"


class Hardware;
class Instruction;

typedef void (*Executor)(Hardware* hart, const Instruction& insn);

enum InsnClass : uint8_t //256 instruction are possible
{
    kInsnAdd,		//
	kInsnSub,
	kInsnSll,
	kInsnSlt,
	kInsnSltu,
	kInsnXor,
	kInsnSrl,
	kInsnSra,
	kInsnOr,
	kInsnAnd,
    kInsnBeq, 		//
	kInsnBne,
	kInsnBlt,
	kInsnBge,
	kInsnBltu,
	kInsnBgeu,
	kInsnLb,		//
	kInsnLh,
	kInsnLw,
	kInsnLbu,
	kInsnLhu,
	kInsnEcall,		//
	kInsnEbreak,
	kInsnSb,		//
	kInsnSh,
	kInsnSw,
	kInsnAddi,		//
	kInsnSlti,
	kInsnSltiu,
	kInsnXori,
	kInsnOri,
	kInsnAndi,
	kInsnJalr		//
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

    Instruction(EncodedInsn insn, RegValue pc);

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