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
/* + */ 	kInsnAdd,		/* 0b0110011 */
/*   */		kInsnSub,
/*   */		kInsnSll,
/*   */		kInsnSlt,
/*   */		kInsnSltu,
/*   */		kInsnXor,
/*   */		kInsnSrl,
/*   */		kInsnSra,
/*   */		kInsnOr,
/*   */		kInsnAnd,
/* + */		kInsnBeq, 		/*/ 0b1100011 */
/*   */		kInsnBne,
/*   */		kInsnBlt,
/*   */		kInsnBge,
/*   */		kInsnBltu,
/*   */		kInsnBgeu,
/*   */		kInsnLb,		/* 0b0000011 */
/*   */		kInsnLh,
/*   */		kInsnLw,
/*   */		kInsnLbu,
/*   */		kInsnLhu,
/*   */		kInsnEcall,		/* 0b1110011 */
/*   */		kInsnEbreak,
/*   */		kInsnSb,		/* 0b0100011 */
/*   */		kInsnSh,
/*   */		kInsnSw,
/* + */		kInsnAddi,		/* 0b0010011 */
/*   */		kInsnSlti,
/*   */		kInsnSltiu,
/*   */		kInsnXori,
/*   */		kInsnOri,
/*   */		kInsnAndi,
/*   */		kInsnSlli,
/*   */		kInsnSrli,
/*   */		kInsnSrai,
/*   */		kInsnJal,		/* 0b1101111 */
/*   */		kInsnJalr		/* 0b1100111 */
};

const char kInsnTypeNames[][20] =
{
	"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", 
	"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU",
	"LB", "LH", "LW", "LBU", "LHU",
	"ECALL", "EBREAK",
	"SB", "SH", "SW",
	"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
	"JAL",
	"JALR"
};



template<int hi,int lo, class T = RegValue>
T getBits(T value)
{
    return ( ( value << (sizeof(T) * 8 - hi - 1) ) >> (sizeof(T) * 8 - hi - 1) ) >> lo ; 
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

	const char* fromTypeToStr(InsnClass type);

	void executeAdd(Hardware* harw);
	void executeBeq(Hardware* harw);

	void executeAddi(Hardware* harw);


	void executor(Hardware* hardw);

};

#endif 