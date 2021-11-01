#ifndef INSTR_H
#define INSTR_H

#include "../includes/libs.h"
#include "../includes/debug.h"
#include "../includes/common.h"


class Hardware;
class Instruction;

enum InsnClass : uint8_t //256 instruction are possible
{
/* + */ 	kInsnAdd,		/* 0b0110011 */
/* + */		kInsnSub,
/* + */		kInsnSll,
/*   */		kInsnSlt,
/*   */		kInsnSltu,
/* + */		kInsnXor,
/* + */		kInsnSrl,
/* + */		kInsnSra,
/* + */		kInsnOr,
/* + */		kInsnAnd,
/* + */		kInsnBeq, 		/*/ 0b1100011 */
/* + */		kInsnBne,
/* + */		kInsnBlt,
/* + */		kInsnBge,
/*   */		kInsnBltu,
/*   */		kInsnBgeu,
/* + */		kInsnLb,		/* 0b0000011 */
/* + */		kInsnLh,
/* + */		kInsnLw,
/* + */		kInsnLbu,
/* + */		kInsnLhu,
/* + */		kInsnEcall,		/* 0b1110011 */
/* + */		kInsnEbreak,
/* + */		kInsnSb,		/* 0b0100011 */
/* + */		kInsnSh,
/* + */		kInsnSw,
/* + */		kInsnAddi,		/* 0b0010011 */
/*   */		kInsnSlti,
/*   */		kInsnSltiu,
/* + */		kInsnXori,
/* + */		kInsnOri,
/* + */		kInsnAndi,
/* + */		kInsnSlli,
/* + */		kInsnSrli,
/* + */		kInsnSrai,
/* + */		kInsnJal,		/* 0b1101111 */
/* + */		kInsnJalr,		/* 0b1100111 */
/* + */		kInsnAuipc,		/* 0b0010111 */
/* + */		kInsnLui,		/* 0b0110111*/
			insnERROR,
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

enum PrintInfo { PI_rd, PI_rs1, PI_rs2, PI_imm, PI_size };
const char INFO_NAMES[][20] = { "rd", "rs1", "rs2", "imm" };

class Instruction final
{
private:
	bool (Instruction::*executor_)(Hardware *hardw);
	
    InsnClass insnType_;
	RegId rd_, rs1_, rs2_;
	RegValue imm_;

	

	bool needDebug_;
	bool printInfo[PI_size] = {0};

public:

    Instruction(EncodedInsn insn, RegValue pc);

// SETTERS

	void setDebugRegime(bool regime) {needDebug_ = regime;}

// GETTERS

    RegId rd()  const {return rd_;} 
    RegId rs1() const {return rs1_;}
    RegId rs2() const {return rs2_;}

    RegValue imm() const {return imm_;}

	InsnClass insn() const { return insnType_;}

// OTHER

	const char* fromTypeToStr(InsnClass type);
	void printInsnType(InsnClass type);

	bool executeNOEXIST(Hardware *hardw);

	bool executeAuipc(Hardware *hardw);
	bool executeLui(Hardware *hardw);

	bool executeAdd(Hardware *hardw); 		/* + */
	bool executeSub(Hardware *hardw);		/* + */
	bool executeSll(Hardware *hardw);		/* + */
	bool executeXor(Hardware *hardw);		/* + */
	bool executeSrl(Hardware *hardw);		/* + */
	bool executeSra(Hardware *hardw);		/* + */
	bool executeOr (Hardware *hardw);		/* + */
	bool executeAnd(Hardware *hardw);		/* + */



	bool executeBeq(Hardware *hardw); 		/* + */
	bool executeBne(Hardware *hardw); 		/* + */
	bool executeBlt(Hardware *hardw); 		/* + */
	bool executeBge(Hardware *hardw); 		/* + */
	bool executeBltu(Hardware *hardw); 		/* + */
	bool executeBgeu(Hardware *hardw); 		/* + */


	bool executeLb(Hardware *hardw);		/* + */
	bool executeLh(Hardware *hardw);		/* + */
	bool executeLw(Hardware *hardw);		/* + */
	bool executeLbu(Hardware *hardw);		/* + */
	bool executeLhu(Hardware *hardw);		/* + */

	bool executeEcall(Hardware *hardw);		/* + */
	bool executeEbreak(Hardware *hardw);	/* + */

	bool executeSb(Hardware *hardw);		/* + */
	bool executeSh(Hardware *hardw);		/* + */
	bool executeSw(Hardware *hardw);		/* + */


	bool executeAddi(Hardware *hardw); 		/* + */
	bool executeXori(Hardware *hardw);		/* + */
	bool executeOri (Hardware *hardw);		/* + */
	bool executeAndi(Hardware *hardw);		/* + */
	bool executeSlli(Hardware *hardw);		/* + */
	bool executeSrli(Hardware *hardw);		/* + */
	bool executeSrai(Hardware *hardw);		/* + */


	bool executeJal(Hardware *hardw);		/* + */
	bool executeJalr(Hardware *hardw);		/* + */


	bool executor(Hardware* hardw);

};

#endif 