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
	kInsnJal,		//
	kInsnJalr		//
};

template<int hi,int lo, class T = RegValue>
T getBits(T value)
{
	auto x1 = value << (sizeof(T) * 8 - hi - 1);
	auto x2 = x1 >> (sizeof(T) * 8 - hi - 1);
	auto x3 = x2 >> lo;
	/*auto x1 = ((value >> lo) << lo);
	auto x2 = (x1 << (sizeof(T) * 8 - hi - 1));
	auto x3 = x2 >> (sizeof(T) * 8 - hi - 1);
	
	std::bitset<32> x1_b(x1);
	std::bitset<32> x2_b(x2);
	std::bitset<32> x3_b(x3);
	std::cout << "x1_b =   " << x1_b;
	std::cout << "\nx2_b =   " << x2_b;
	std::cout << "\nx3_b =   " << x3_b << std::endl;
*/

    return x3; 
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
		case kInsnBeq:
			std::cout << "type of insn = BEQ\n";
			executeBeq(hardw);
			break;
		default:
			std::cout << "default insn Type\n";
			break;
		}
	} 

};

#endif 