#include "instruction.h"
#include "../Hardware/hardware.h"

const char* Instruction::fromTypeToStr(InsnClass type)
{
	return kInsnTypeNames[type];
}

void Instruction::printInsnType(InsnClass type)
{
	std::cout << fromTypeToStr(type) << std::endl;
}

Instruction::Instruction(EncodedInsn insn, RegValue pc)
{
	/* TODO: exception-handler */

	insnType_ = insnERROR;
	switch (insn & 0x7F) /* 1st seven bits*/
	{
		// R-format insns
		case 0b0010111:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			imm_ = getBits<31, 12>(insn) << 12;
			
			insnType_ = kInsnAuipc;
			executor_ = &Instruction::executeAuipc;

			printInfo[PI_rd] = true;
			printInfo[PI_imm] = true;

			break;
		}
		case 0b0110111:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			imm_ = getBits<31, 12>(insn) << 12;

			insnType_ = kInsnLui;
			executor_ = &Instruction::executeLui;

			printInfo[PI_rd] = true;
			printInfo[PI_imm] = true;

			break;
		}
		case 0b0110011: /* DONE [only function realization needed] */
        {
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			auto funct3 = getBits<14, 12>(insn);
			auto funct7 = getBits<31, 25>(insn);
			
			printInfo[PI_rd] = true;
			printInfo[PI_rs1] = true;
			printInfo[PI_rs2] = true;

			/* TODO: switch-case ; executor = .. */

			if (funct3 == 0 && funct7 == 0)
			{
			    insnType_ = kInsnAdd;
				executor_ = &Instruction::executeAdd;
			}
			else if (funct3 == 0 && funct7 == 0b0100000)
			{
				insnType_ = kInsnSub;
				executor_ = &Instruction::executeSub;
			}
			else if (funct3 == 0b001 && funct7 == 0)
			{
			    insnType_ =  kInsnSll;
				executor_ = &Instruction::executeSll;
			}
			else if (funct3 == 0b010 && funct7 == 0)
			{
				insnType_ =  kInsnSlt;
				executor_ = &Instruction::executeNOEXIST;
			}		
			else if (funct3 == 0b011 && funct7 == 0)
			{
			    insnType_ =  kInsnSltu;
				executor_ = &Instruction::executeNOEXIST;
			}
			else if (funct3 == 0b100 && funct7 == 0)
			{
			   	insnType_ =  kInsnXor;
				executor_ = &Instruction::executeXor;
			}	
			else if (funct3 == 0b101 && funct7 == 0)
			{
			    insnType_ =  kInsnSrl;
				executor_ = &Instruction::executeSrl;
			}
			else if (funct3 == 0b101 && funct7 == 0b0100000)
			{
			    insnType_ =  kInsnSra;
				executor_ = &Instruction::executeSra;
			}
			else if (funct3 == 0b110 && funct7 == 0)
			{
			    insnType_ =  kInsnOr;
				executor_ = &Instruction::executeOr;
			}
			else if (funct3 == 0b111 && funct7 == 0)
			{
				insnType_ =  kInsnOr;
				executor_ = &Instruction::executeOr;
			}
			else
				assert (0);
		    break;
        }
        case 0b1100011: /* BEQ BNE BLT BGE BLTU BGEU */
		{
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			/* only possible jump available */
			imm_ = (getBits<11, 8>(insn) << 1) + (getBits<7, 7>(insn) << 11) + 
					(getBits<30, 25>(insn) << 5) + (getBits<0, 0>(static_cast<SRegValue>(insn)) >> 31) + pc;		

			auto funct3 = getBits<14, 12>(insn);

			printInfo[PI_rs1] = true;
			printInfo[PI_rs2] = true;
			printInfo[PI_imm] = true;
			
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnBeq;
				executor_ = &Instruction::executeBeq;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnBne;
				executor_ = &Instruction::executeBne;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnBlt;
				executor_ = &Instruction::executeBlt;
			}
			else if (funct3 == 0b101)
			{
				insnType_ = kInsnBge;
				executor_ = &Instruction::executeBge;
			}
			else if (funct3 == 0b110)
			{
				insnType_ = kInsnBltu;
				executor_ = &Instruction::executeBltu;
			}
			else if (funct3 == 0b111)
			{
				insnType_ = kInsnBgeu;
				executor_ = &Instruction::executeBgeu;
			}
			else
				assert (0 && "Undefinied funct3 for PC-instruction");
		    break;
        }
		case 0b0000011: /* LB LH LW LBU LHU */ 
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId>
			
			auto funct3 = getBits<14, 12>(insn);

			printInfo[PI_rd] = true;
			printInfo[PI_rs1] = true;
			printInfo[PI_imm] = true;
						
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnLb;
				executor_ = &Instruction::executeLb;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnLh;
				executor_ = &Instruction::executeLh;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnLw;
				executor_ = &Instruction::executeLw;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnLbu;
				executor_ = &Instruction::executeLbu;
			}
			else if (funct3 == 0b101)
			{
				insnType_ = kInsnLhu;
				executor_ = &Instruction::executeLhu;
			}
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

			printInfo[PI_rd] = true;
			printInfo[PI_rs1] = true;
			printInfo[PI_imm] = true;
			
			if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0)
			{
			    insnType_ = kInsnEcall;
				executor_ = &Instruction::executeEcall;
			}
			else if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0b000000000001)
			{
				insnType_ = kInsnEbreak;
				executor_ = &Instruction::executeEbreak;
			}
			else
				assert (0 && "Undefinied instruction");
		    break;
		}
		case 0b0100011: /* SB SH SW */ 
		{
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			imm_ = getBits<11, 7>(insn) + getBits<6, 0>(static_cast<SRegValue>(insn) >> 25) << 5;

			auto funct3 = getBits<14, 12>(insn);

			printInfo[PI_rs1] = true;
			printInfo[PI_rs2] = true;
			printInfo[PI_imm] = true;

			if (funct3 == 0b000)
			{
			    insnType_ = kInsnSb;
				executor_ = &Instruction::executeSb;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnSh;
				executor_ = &Instruction::executeSh;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnSw;
				executor_ = &Instruction::executeSw;
			}
			else
				assert (0 && "Undefinied type");
		    break;
        }
		case 0b0010011: /* ADDI SLTI SLTIU XORI ORI ANDI SLLI SRLI SRAI */
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId> 

			auto funct3 = getBits<14, 12>(insn);

			printInfo[PI_rd] = true;
			printInfo[PI_rs1] = true;
			printInfo[PI_imm] = true;

			if (funct3 == 0b000)
			{
				insnType_ = kInsnAddi;
				executor_ = &Instruction::executeAddi;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnSlti;
				executor_ = &Instruction::executeNOEXIST;
			}
			else if (funct3 == 0b011)
			{
				insnType_ = kInsnSltiu;
				executor_ = &Instruction::executeNOEXIST;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnXori;
				executor_ = &Instruction::executeXori;
			}
			else if (funct3 == 0b110)
			{
				insnType_ = kInsnOri;
				executor_ = &Instruction::executeOri;
			}
			else if (funct3 == 0b111)
			{
				insnType_ = kInsnAndi;
				executor_ = &Instruction::executeAndi;
			}
			else /* logical/arithmetic shifts */
			{
				auto funct7 = getBits<5, 11>(imm_);
				imm_ = getBits<0, 4>(imm_);

				if (funct7 == 0 && funct3 == 0b001)
				{
					insnType_ = kInsnSlli;
					executor_ = &Instruction::executeSlli;
				}
				else if (funct7 == 0 && funct3 == 0b101)
				{
					insnType_ = kInsnSrli;
					executor_ = &Instruction::executeSrli;
				}
				else if (funct7 == 0b100000 && funct3 == 0b101)
				{
					insnType_ = kInsnSrai;
					executor_ = &Instruction::executeSrai;
				}
				else
					assert (0 && "Undefinied type");
			}
			break;
		}
		case 0b1100111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId> 

			auto funct3 = getBits<14, 12>(insn);

			printInfo[PI_rd] = true;
			printInfo[PI_rs1] = true;
			printInfo[PI_imm] = true;
			
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnJalr;
				executor_ = &Instruction::executeJalr;
			}
			else 
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1101111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			
			uint32_t retImm = getBits<19, 0>(static_cast<SRegValue>(insn) >> 12);//getBits<31, 12>(insn);

			imm_ = (getBits<20, 20>(retImm) << 20) + (getBits<19, 9>(retImm) << 1) 
				 + (getBits<8 ,  8>(retImm) << 11) + (getBits<7 , 0>(retImm) << 12) + pc;

			printInfo[PI_rd] = true;
			printInfo[PI_imm] = true;

			insnType_ = kInsnJal;
			executor_ = &Instruction::executeJal;
		    break;
		}
		default:
		{
			auto opcode = (insn & 0x7F);
			std::cout << "Undefinied opcode = " << opcode << std::endl;
			P_BIT_NUM(opcode, 7);
			break;
		}
	}	
}
//////////////////////////////////////////////////////////////////////////

bool Instruction::executeNOEXIST(Hardware *hardw)
{
	std::cout << "CALL INSTRUCTION WITH NO executor EXISTS\n" << std::endl;
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool Instruction::executeAuipc(Hardware *harw)
{
	harw->setReg(rd_, imm_);
	return true;
}

bool Instruction::executeLui(Hardware *harw)
{
	harw->setReg(rd_, imm_ + harw->pc());
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool Instruction::executeAdd(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) + harw->getReg(rs2_));
	return true;
}

bool Instruction::executeSub(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) - harw->getReg(rs1_));
	return true;
}

bool Instruction::executeXor(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) ^ harw->getReg(rs1_));
	return true;
}

bool Instruction::executeOr (Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) | harw->getReg(rs1_));
	return true;
}

bool Instruction::executeAnd(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs2_) & harw->getReg(rs1_));
	return true;
}


bool Instruction::executeSll(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) << harw->getReg(rs2_));
	return true;
}	

bool Instruction::executeSrl(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) >> harw->getReg(rs2_));
	return true;	
}

bool Instruction::executeSra(Hardware *harw)
{
	if (getBits<31, 31>(harw->getReg(rs1_)) == 0b0)
		harw->setReg(rd_, harw->getReg(rs1_) >> harw->getReg(rs2_));
	else
		harw->setReg(rd_, static_cast<RegValue>(static_cast<int32_t>(harw->getReg(rs1_)) >> harw->getReg(rs2_)));
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool Instruction::executeBeq(Hardware *harw)
{
	if (harw->getReg(rs1_) == harw->getReg(rs2_))
	    harw->branch(imm_); 
	return true;
}

bool Instruction::executeBne(Hardware *harw)
{
	if (harw->getReg(rs1_) != harw->getReg(rs2_))
	    harw->branch(imm_); 
	return true;
}

bool Instruction::executeBlt(Hardware *harw)
{
	if (harw->getReg(rs1_) < harw->getReg(rs2_)) /* or rs2_ < rs1_*/
	    harw->branch(imm_); 
	return true;
}

bool Instruction::executeBge(Hardware *harw)
{
	if (harw->getReg(rs1_) >= harw->getReg(rs2_)) /* or rs2_ >= rs1_*/
	    harw->branch(imm_); 
	return true;
}

bool Instruction::executeBltu(Hardware *harw)
{
	if (harw->getReg(rs1_) < harw->getReg(rs2_)) /* or rs2_ < rs1_*/
	    harw->branch(getBits<11, 0>(imm_)); 
	return true;
}

bool Instruction::executeBgeu(Hardware *harw)
{
	if (harw->getReg(rs1_) >= harw->getReg(rs2_)) /* or rs2_ >= rs1_*/
	    harw->branch(getBits<11, 0>(imm_)); 
	return true;
}

//////////////////////////////////////////////////////////////////////////

#define BYTE 1
#define HWORD 2
#define WORD 4

bool Instruction::executeLb(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, BYTE);
	harw->setReg(rd_, readVal + getBits<7,7>(readVal) == 0b1? 0xFFFFFF00 : 0);
	return true;
}

bool Instruction::executeLh(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, HWORD);
	harw->setReg(rd_, readVal + getBits<15,15>(readVal) == 0b1? 0xFFFF0000 : 0);
	return true;
}

bool Instruction::executeLw(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, WORD);
	harw->setReg(rd_, readVal);
	return true;
}

/* unsigned === if read the byte 0xFF lbu will 0-extend this value to 0x000000FF*/

bool Instruction::executeLbu(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, BYTE);
	harw->setReg(rd_, readVal);
	return true;
}

bool Instruction::executeLhu(Hardware *harw)
{
	RegValue readVal;
	harw->read(harw->getReg(rs1_) + imm_, &readVal, HWORD);
	harw->setReg(rd_, readVal);
	return true;
}

/* 			LOAD	section	/\		*/
/* 	 		STORE	section \/		*/

bool Instruction::executeSb(Hardware *harw)
{
	harw->write(harw->getReg(rs1_) + imm_, harw->getReg(rs2_), BYTE);
	return true;
}

bool Instruction::executeSh(Hardware *harw)
{
	harw->write(harw->getReg(rs1_) + imm_, harw->getReg(rs2_), HWORD);
	return true;
}

bool Instruction::executeSw(Hardware *harw)
{
	harw->write(harw->getReg(rs1_) + imm_, harw->getReg(rs2_), WORD);
	return true;
}

#undef BYTE
#undef HWORD
#undef WORD

//////////////////////////////////////////////////////////////////////////

bool Instruction::executeEcall(Hardware *harw)
{
	harw->branch(0x80000180);
	return true;
}

bool Instruction::executeEbreak(Hardware *harw)
{
	harw->Ebreak();
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool Instruction::executeAddi(Hardware *harw)
{	
	harw->setReg(rd_, harw->getReg(rs1_) + imm_);
	return true;	
}

bool Instruction::executeXori(Hardware *harw)
{
	harw->setReg(rd_, imm_ ^ harw->getReg(rs1_));
	return true;
}

bool Instruction::executeOri (Hardware *harw)
{
	harw->setReg(rd_, imm_ | harw->getReg(rs1_));
	return true;
}

bool Instruction::executeAndi(Hardware *harw)
{
	harw->setReg(rd_, imm_ & harw->getReg(rs1_));
	return true;
}

bool Instruction::executeSlli(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) << imm_);
	return true;
}

bool Instruction::executeSrli(Hardware *harw)
{
	harw->setReg(rd_, harw->getReg(rs1_) >> imm_);
	return true;
}

bool Instruction::executeSrai(Hardware *harw)
{
	if (getBits<31, 31>(harw->getReg(rs1_)) == 0b0)
		harw->setReg(rd_, harw->getReg(rs1_) >> imm_);
	else
		harw->setReg(rd_, static_cast<RegValue>( static_cast<int32_t>(harw->getReg(rs1_)) >> imm_ ));
	return true;
}

//////////////////////////////////////////////////////////////////////////


bool Instruction::executeJal(Hardware *harw)
{
	harw->setReg(rd_, harw->pc() + 4);
	harw->branch(imm_);
	return true;
}

bool Instruction::executeJalr(Hardware *harw)
{
	harw->setReg(rd_, harw->pc() + 4);
	harw->branch(imm_ + harw->getReg(rd_));
	return true;
}


//////////////////////////////////////////////////////////////////////////

bool Instruction::executor(Hardware *hardw) 
{
	if (needDebug_)
	{
		std::cout << "\t\t Before execute [" << fromTypeToStr(insnType_) << "] : " << std::endl;

		if (printInfo[PI_rd])
			P_REG_VAL("rd", rd_)
		if (printInfo[PI_rs1])
			P_REG_VAL("rs1", rs1_)
		if (printInfo[PI_rs2])
			P_REG_VAL("rs2", rs2_)
		if (printInfo[PI_imm])
			P_NUM(imm_, 1)
	}

	if ( !(*this.*executor_)(hardw) )
	{
		std::cout << "FAILED" << std::endl;
		return false;
	}

	if (needDebug_)
	{
		std::cout << "\t\t After execute: " << std::endl;

		if (printInfo[PI_rd])
			P_REG_VAL("rd", rd_)
		if (printInfo[PI_rs1])
			P_REG_VAL("rs1", rs1_)
		if (printInfo[PI_rs2])
			P_REG_VAL("rs2", rs2_)
		if (printInfo[PI_imm])
			P_NUM(imm_, 1)
	}
	return true;

	/*switch (insnType_)
	{
	case insnERROR:
		std::cout << "insnError" << std::endl;
		assert(0);
		break;
/////////////////////////////////////////////
	case kInsnAuipc:
		executeAuipc(hardw);
		break;
	case kInsnLui:
		executeLui(hardw);
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
	case kInsnBlt:
		executeBlt(hardw);
		break;
	case kInsnBge:
		executeBge(hardw);
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
	case kInsnLbu:
		executeLbu(hardw);
		break;
	case kInsnLhu:
		executeLhu(hardw);
		break;
/////////////////////////////////////////////
	case kInsnEcall:
		executeEcall(hardw);
		break;
	case kInsnEbreak:
		executeEbreak(hardw);
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
	case kInsnJalr:
		executeJalr(hardw);
		break;
/////////////////////////////////////////////
	default:
		std::cout << "undefinied [instruction type = " << fromTypeToStr(insnType_) << "] LINE :" << __LINE__ << std::endl;
		return false;
		break;
	}*/

} 