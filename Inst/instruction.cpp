#include "instruction.h"
#include "../Hardware/hardware.h"

Instruction::Instruction(EncodedInsn insn, RegValue pc)
{
	    // TODO: decode format before individual instructions
	    // decoder is is the only place where raw constants are acceptable(!)

		std::cout << "pc = " << pc << std::endl;
	    switch (insn & 0x7F) /* 1st seven bits*/
		{
		case 0:
		    break;
		// R-format insns
		case 0b0110011: /* DONE [only function realization needed]*/
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
        case 0b1100011: /* PC (Branch)*/
		{

			/*std::bitset<32> insn_b(insn);
			std::cout << "insn_b = " << insn_b << std::endl;
			*/

			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));

			/*std::bitset<5> rd_b(insn);
			std::cout << "rd_b = " << rd_b << std::endl;

			std::cout << "rs1, rs2, rd= " << rs1_ << " " << rs2_ << " " << rd_ << std::endl;
			*/

			auto testval1 = getBits<11, 8>(insn) << 1;
			//std::bitset<5> testval1_b(testval1);
			//std::cout << "testval1_b = " << testval1_b << std::endl;

			auto testval2 = getBits<7, 7>(insn) << 11;
			//std::bitset<5> testval2_b(testval2);
			//std::cout << "testval2_b = " << testval2_b << std::endl;

			auto testval3 = getBits<30, 25>(insn) << 5;
			//std::bitset<5> testval3_b(testval3);
			//std::cout << "testval3_b = " << testval3_b << std::endl;


			auto testval4 = getBits<31, 31>(insn) << 12;
			//std::bitset<5> testval4_b(testval4);
			//std::cout << "testval4_b = " << testval4_b << std::endl;
			

			imm_ = testval1 + testval2 + testval3 + testval4;
			
			std::bitset<13> imm_b(imm_);
			//std::cout << "imm_b = " << imm_b << std::endl; 

			auto funct3 = getBits<14, 12>(insn);
			
			if (funct3 == 0b000)
			{
				//std::cout<< "Beq finded\n";

			    insnType_ = kInsnBeq;
			}
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
		case 0b0000011:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<31, 20>(insn);
			
			auto funct3 = getBits<14, 12>(insn);
			
			//imm_ = imm_ + pc; //!!!!!!!!
			
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
		case 0b1110011: /* ECALL - EBREAK*/
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
		case 0b0100011:
		{
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
					
			imm_ = getBits<11, 7>(insn) + getBits<31, 25>(insn) << 5;

			auto funct3 = getBits<14, 12>(insn);

			// imm_ + pc?

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
		case 0b0010011:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = 	static_cast<RegId> (getBits<31, 20>(insn));

			auto funct3 = getBits<14, 12>(insn);

			// imm_ + pc?

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
			else
				assert (0 && "Undefinied type");
		}
		case 0b1101111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			uint32_t retImm = getBits<31, 20>(insn);

			imm_ = getBits<20, 20>(retImm) << 20 + getBits<19, 9>(retImm) << 1 
				 + getBits<8 ,  8>(retImm) << 11 + getBits<0 , 7>(retImm) << 12;
			
			auto funct3 = getBits<14, 12>(insn);
			
			//imm_ = imm_ + pc; //!!!!!!!!
			
			if (funct3 == 0b000)
			    insnType_ = kInsnJal;
			else 
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1100111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<15, 19>(insn));
			imm_ = getBits<31, 20>(insn) ;
			// +pc
			
			insnType_ = kInsnJalr;
		    break;
		}
		}
	}

void Instruction::executeAdd(Hardware* harw)
{
    auto rs1value = harw->getReg(rs1_);
	auto rs2value = harw->getReg(rs2_);
	
	auto result = rs1value + rs2value;
	
	std::cout << "result in ADD = " << result << std::endl;

	harw->setReg(rd_, result);
}

void Instruction::executeBeq(Hardware* harw)
{
	std::cout << "rs1, rs2 = " << rs1_ << " " << rs2_ << std::endl;
    auto rs1value = harw->getReg(rs1_);
	auto rs2value = harw->getReg(rs2_);
	
	std::cout << "imm = " << imm_ << std::endl;

	if (rs1value == rs2value)
	    harw->branch(imm_); // + pc_
	
	//harw->setReg(insn.rd(), result);
}