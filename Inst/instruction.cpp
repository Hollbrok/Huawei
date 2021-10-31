#include "instruction.h"
#include "../Hardware/hardware.h"

const char* Instruction::fromTypeToStr(InsnClass type)
{
	return kInsnTypeNames[type];
}

Instruction::Instruction(EncodedInsn insn, RegValue pc)
{
	    // TODO: decode format before individual instructions
	    // decoder is is the only place where raw constants are acceptable(!)

		std::cout << "pc = " << pc << std::endl;
		insnType_ = insnERROR;
	    switch (insn & 0x7F) /* 1st seven bits*/
		{
		case 0:
		    break;
		// R-format insns
		case 0b0110011: /* DONE [only function realization needed] */
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
			else if (funct3 == 0 && funct7 == 0b0100000)
			{
				std::cout << "SUB FINDED\n";
			    insnType_ = kInsnSub;
			}
			else if (funct3 == 0b001 && funct7 == 0)
			{
				std::cout << "SLL  FINDED\n";
			    insnType_ =  kInsnSll;
			}
			else if (funct3 == 0b010 && funct7 == 0)
			{
				std::cout << "SLT  FINDED\n";
			    insnType_ =  kInsnSlt;
			}
			else if (funct3 == 0b011 && funct7 == 0)
			{
				std::cout << "SLTU  FINDED\n";
			    insnType_ =  kInsnSltu;
			}
			else if (funct3 == 0b100 && funct7 == 0)
			{
				std::cout << "XOR FINDED\n";
			    insnType_ =  kInsnXor;
			}
			else if (funct3 == 0b101 && funct7 == 0)
			{
				std::cout << "SRL  FINDED\n";
			    insnType_ =  kInsnSrl;
			}
			else if (funct3 == 0b101 && funct7 == 0b0100000)
			{
				std::cout << "SRA  FINDED\n";
			    insnType_ =  kInsnSra;
			}
			else if (funct3 == 0b110 && funct7 == 0)
			{
				std::cout << "OR  FINDED\n";
			    insnType_ =  kInsnOr;
			}
			else if (funct3 == 0b111 && funct7 == 0)
			{
				std::cout << "AND  FINDED\n";
			    insnType_ =  kInsnOr;
			}
			else
				assert (0);
		    break;
        }
        case 0b1100011: /* PC (Branch) */
		{

			/*std::bitset<32> insn_b(insn);
			std::cout << "insn_b = " << insn_b << std::endl;
			*/

			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));

			auto testval1 = getBits<11, 8>(insn) << 1;

			P_BIT_NUM(testval1, 4);

			auto testval2 = getBits<7, 7>(insn) << 11;

			P_BIT_NUM(testval2, 1);

			auto testval3 = getBits<30, 25>(insn) << 5;
			auto testval4 = getBits<31, 31>(insn) << 12;
			
			std::cout << "pc = " << pc << std::endl;
			imm_ = testval1 + testval2 + testval3 + testval4 + pc;

			auto funct3 = getBits<14, 12>(insn);
			
			if (funct3 == 0b000)
			    insnType_ = kInsnBeq;
			else if (funct3 == 0b001)
				insnType_ = kInsnBne;
			else if (funct3 == 0b100)
				insnType_ = kInsnBlt;
			else if (funct3 == 0b101)
				insnType_ = kInsnBge;
			else if (funct3 == 0b110)
				insnType_ = kInsnBltu;
			else if (funct3 == 0b111)
				insnType_ = kInsnBgeu;
			else
				assert (0 && "Undefinied funct3 for PC-instruction");
		    break;
        }
		case 0b0000011: /* LB lH LW LBU LHU */ 
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<31, 20>(insn);
			
			auto funct3 = getBits<14, 12>(insn);
						
			if (funct3 == 0b000)
			    insnType_ = kInsnLb;
			else if (funct3 == 0b001)
				insnType_ = kInsnLh;
			else if (funct3 == 0b010)
				insnType_ = kInsnLw;
			else if (funct3 == 0b100)
				insnType_ = kInsnLbu;
			else if (funct3 == 0b101)
				insnType_ = kInsnLhu;
			else
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1110011: /* ECALL - EBREAK */
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<31, 20>(insn);
			
			auto funct3 = getBits<14, 12>(insn);
			
			if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0)
			    insnType_ = kInsnEcall;
			else if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0b000000000001)
				insnType_ = kInsnEbreak;
			else
				assert (0 && "Undefinied instruction");
		    break;
		}
		case 0b0100011: /* SB SH SW */ 
		{
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
					
			imm_ = getBits<11, 7>(insn) + getBits<31, 25>(insn) << 5;

			auto funct3 = getBits<14, 12>(insn);

			if (funct3 == 0b000)
			    insnType_ = kInsnSb;
			else if (funct3 == 0b001)
				insnType_ = kInsnSh;
			else if (funct3 == 0b010)
				insnType_ = kInsnSw;
			else
				assert (0 && "Undefinied type");
		    break;
        }
		case 0b0010011: /* ADDI SLTI SLTIU XORI ORI ANDI SLLI SRLI SRAI */
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = 	static_cast<RegId> (getBits<31, 20>(insn));

			auto funct3 = getBits<14, 12>(insn);

			if (funct3 == 0b000)
			    insnType_ = kInsnAddi;
			else if (funct3 == 0b010)
				insnType_ = kInsnSlti;
			else if (funct3 == 0b011)
				insnType_ = kInsnSltiu;
			else if (funct3 == 0b100)
				insnType_ = kInsnXori;
			else if (funct3 == 0b110)
				insnType_ = kInsnOri;
			else if (funct3 == 0b111)
				insnType_ = kInsnAndi;
			else /* logical/arithmetic shifts */
			{
				auto funct7 = getBits<5, 11>(imm_);

				if (funct7 == 0 && funct3 == 0b001)
					insnType_ = kInsnSlli;
				else if (funct7 == 0 && funct3 == 0b101)
					insnType_ = kInsnSrli;
				else if (funct7 == 0b100000 && funct3 == 0b101)
					insnType_ = kInsnSrai;
				else
					assert (0 && "Undefinied type");
			}
		}
		case 0b1100111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<31, 20>(insn);

			auto funct3 = getBits<14, 12>(insn);
			
			if (funct3 == 0b000)
			    insnType_ = kInsnJalr;
			else 
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1101111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			
			uint32_t retImm = getBits<31, 12>(insn);

			imm_ = (getBits<20, 20>(retImm) << 20) + (getBits<19, 9>(retImm) << 1) 
				 + (getBits<8 ,  8>(retImm) << 11) + (getBits<0 , 7>(retImm) << 12) + pc;

			insnType_ = kInsnJal;
		    break;
		}
		default:
		{
			auto opcode = (insn & 0x7F);
			std::cout << "Undefinied opcode = " << opcode << std::endl;
			P_BIT_NUM(opcode, 7);
		}
		}
	}

//////////////////////////////////////////////////////////////////////////

void Instruction::executeAdd(Hardware *harw)
{		
	harw->setReg(rd_, harw->getReg(rs1_) + harw->getReg(rs2_));
}

void Instruction::executeSub(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) - harw->getReg(rs1_));
}

void Instruction::executeXor(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) ^ harw->getReg(rs1_));
}

void Instruction::executeOr (Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) | harw->getReg(rs1_));
}

void Instruction::executeAnd(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) & harw->getReg(rs1_));
}


void Instruction::executeSll(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) << harw->getReg(rs2_));
}	

void Instruction::executeSrl(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) >> harw->getReg(rs2_));	
}

void Instruction::executeSra(Hardware *harw)
{
	if (getBits<31, 31>(harw->getReg(rs1_)) == 0b0)
		harw->setReg(rd_, harw->getReg(rs1_) >> harw->getReg(rs2_));
	else
		harw->setReg(rd_, static_cast<RegValue>(static_cast<int32_t>(harw->getReg(rs1_)) >> harw->getReg(rs2_)));
}

//////////////////////////////////////////////////////////////////////////


void Instruction::executeBeq(Hardware *harw)
{
	std::cout << "rs_1, rs_2, imm_ " << harw->getReg(rs1_) << " " << harw->getReg(rs2_) << " " << imm_ << std::endl;
	if (harw->getReg(rs1_) == harw->getReg(rs2_))
	    harw->branch(imm_); 
}

void Instruction::executeBne(Hardware *harw)
{
	std::cout << "rs_1, rs_2, imm_ " << harw->getReg(rs1_) << " " << harw->getReg(rs2_) << " " << imm_ << std::endl;
	if (harw->getReg(rs1_) != harw->getReg(rs2_))
	    harw->branch(imm_); 
}
//////////////////////////////////////////////////////////////////////////

#define BYTE 1
#define HWORD 2
#define WORD 4

void Instruction::executeLb(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, BYTE);
	harw->setReg(rd_, readVal);
}

void Instruction::executeLh(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, HWORD);
	harw->setReg(rd_, readVal);
}

void Instruction::executeLw(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, WORD);
	harw->setReg(rd_, readVal);
}

/* 			LOAD	section	/\		*/
/* 	 		STORE	section \/		*/

void Instruction::executeSb(Hardware *harw)
{
	harw->write(harw->getReg(rs2_) + imm_, harw->getReg(rs2_), BYTE);
}

void Instruction::executeSh(Hardware *harw)
{
	harw->write(harw->getReg(rs2_) + imm_, harw->getReg(rs2_), HWORD);
}

void Instruction::executeSw(Hardware *harw)
{
	harw->write(harw->getReg(rs2_) + imm_, harw->getReg(rs2_), WORD);
}

#undef BYTE
#undef HWORD
#undef WORD

//////////////////////////////////////////////////////////////////////////

void Instruction::executeAddi(Hardware *harw)
{	
	harw->setReg(rd_, harw->getReg(rs1_) + imm_);	
}

void Instruction::executeSlli(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) << imm_);
}

void Instruction::executeSrli(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) >> imm_);
}

void Instruction::executeSrai(Hardware *harw)
{
	auto shift = getBits<4, 0> (imm_);
	
	if (getBits<31, 31>(harw->getReg(rs1_)) == 0b0)
		harw->setReg(rd_, harw->getReg(rs1_) >> imm_);
	else
		harw->setReg(rd_, static_cast<RegValue>( static_cast<int32_t>(harw->getReg(rs1_)) >> imm_ ));
}

//////////////////////////////////////////////////////////////////////////


void Instruction::executeJal(Hardware *harw)
{
	std::cout << "saved pc = " << harw->pc() + 4 << ";in rd_ = " << rd_ << std::endl;
	std::cout << "imm_ = " << imm_ << std::endl;

	harw->setReg(rd_, harw->pc() + 4);
	
	//harw->pc() = imm_; 
	harw->branch(imm_);
}

void Instruction::executeJalr(Hardware *harw)
{
	std::cout << "saved pc = " << harw->pc() + 4 << ";in rd_ = " << rd_ << std::endl;
	std::cout << "imm_ = " << imm_ << std::endl;

	harw->setReg(rd_, harw->pc() + 4);
	harw->branch(imm_ + harw->getReg(rd_));
}


//////////////////////////////////////////////////////////////////////////

void Instruction::executor(Hardware *hardw) 
{
	if (DEBUG_REGIME)
		std::cout << "type of insn = " << fromTypeToStr(insnType_) << std::endl;

	switch (insnType_)
	{
	case insnERROR:
		std::cout << "insnError" << std::endl;
		assert(0);
		break;
/////////////////////////////////////////////
	case kInsnAdd:
		executeAdd(hardw);
		break;
	case kInsnSub:
		executeSub(hardw);
		break;
	case kInsnSll:
		executeSll(hardw);
		break;
	case kInsnXor:
		executeXor(hardw);
		break;
	case kInsnSrl:
		executeSrl(hardw);
		break;
	case kInsnSra:
		executeSra(hardw);
		break;
	case kInsnOr:
		executeOr(hardw);
		break;
	case kInsnAnd:
		executeAnd(hardw);
		break;
/////////////////////////////////////////////
	case kInsnBeq:
		executeBeq(hardw);
		break;
	case kInsnBne:
		executeBne(hardw);
		break;
/////////////////////////////////////////////
	case kInsnLb:
		executeLb(hardw);
		break;
	case kInsnLh:
		executeLh(hardw);
		break;
	case kInsnLw:
		executeLw(hardw);
		break;
/////////////////////////////////////////////
	case kInsnSb:
		executeSb(hardw);
		break;
	case kInsnSh:
		executeSh(hardw);
		break;
	case kInsnSw:
		executeSw(hardw);
		break;
/////////////////////////////////////////////
	case kInsnAddi:
		executeAddi(hardw);
		break;
	case kInsnSlli:
		executeSlli(hardw);
		break;
	case kInsnSrli:
		executeSrli(hardw);
		break;
	case kInsnSrai:
		executeSrai(hardw);
		break;
/////////////////////////////////////////////
	case kInsnJal:
		executeJal(hardw);
		break;
/////////////////////////////////////////////
	default:
		std::cout << "undefinied [instruction type = " << fromTypeToStr(insnType_) << "] LINE :" << __LINE__ << std::endl;
		assert(0);
		break;
	}
} 