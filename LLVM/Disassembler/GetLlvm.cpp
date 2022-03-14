#include "disassembler.h"

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/TargetSelect.h"

static const int REG_FILE_SIZE = 8;
static uint32_t REG_FILE[REG_FILE_SIZE] = {};

static inline bool cmpCmd(double dCmd, my_commands::Commands cmd)
{
    return static_cast<int>(dCmd) == cmd;
}

static void funcAdd(int destN, int src1N, int src2N)
{
    REG_FILE[destN] = REG_FILE[src1N] + REG_FILE[src2N];
}

static void *lazyFunctionCreator(const std::string &fnName) {
  /*if (fnName == "do_exit") {
    return reinterpret_cast<void *>(do_exit);
  }
  if (fnName == "do_push") {
    return reinterpret_cast<void *>(do_push);
  }
  if (fnName == "do_pop") {
    return reinterpret_cast<void *>(do_pop);
  }
  if (fnName == "do_add_s") {
    return reinterpret_cast<void *>(do_add_s);
  }
  if (fnName == "do_sub_s") {
    return reinterpret_cast<void *>(do_sub_s);
  }
  if (fnName == "do_mul_s") {
    return reinterpret_cast<void *>(do_mul_s);
  }
  if (fnName == "do_div_s") {
    return reinterpret_cast<void *>(do_div_s);
  }
  if (fnName == "do_neg") {
    return reinterpret_cast<void *>(do_neg);
  }*/
  if (fnName == "do_add") {
    return reinterpret_cast<void *>(funcAdd);
  }/*
  if (fnName == "do_sub") {
    return reinterpret_cast<void *>(do_sub);
  }
  if (fnName == "do_mul") {
    return reinterpret_cast<void *>(do_mul);
  }
  if (fnName == "do_div") {
    return reinterpret_cast<void *>(do_div);
  }
  if (fnName == "do_addi") {
    return reinterpret_cast<void *>(do_addi);
  }
  if (fnName == "do_subi") {
    return reinterpret_cast<void *>(do_subi);
  }
  if (fnName == "do_muli") {
    return reinterpret_cast<void *>(do_muli);
  }
  if (fnName == "do_divi") {
    return reinterpret_cast<void *>(do_divi);
  }
  if (fnName == "do_b") {
    return reinterpret_cast<void *>(do_b);
  }
  if (fnName == "do_ret") {
    return reinterpret_cast<void *>(do_ret);
  }
  if (fnName == "do_bl") {
    return reinterpret_cast<void *>(do_bl);
  }
  if (fnName == "do_beq") {
    return reinterpret_cast<void *>(do_beq);
  }
  if (fnName == "do_bne") {
    return reinterpret_cast<void *>(do_bne);
  }
  if (fnName == "do_bge") {
    return reinterpret_cast<void *>(do_bge);
  }
  if (fnName == "do_blt") {
    return reinterpret_cast<void *>(do_blt);
  }
  if (fnName == "do_read") {
    return reinterpret_cast<void *>(do_read);
  }
  if (fnName == "do_write") {
    return reinterpret_cast<void *>(do_write);
  }*/
  return nullptr;
}

void getLlvm(Bytecode *bytecode, FILE *llResult)
{
    std::ifstream input;
    input.open("xxx");
    
    if (!input.is_open())
    {
        fprintf(stderr, "Failed in open xxx");
        return;
    }

    for (int i = 0; i < REG_FILE_SIZE; i++)
    {
        REG_FILE[i] = 0;
    }

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::LLVMContext context;
    // ; ModuleID = 'top'
    // source_filename = "top"
    llvm::Module* module = new llvm::Module("top", context);
    llvm::IRBuilder<> builder(context); 

    //[REG_FILE_SIZE x i32] regFile = {0, 0, 0, 0}
    llvm::ArrayType *regFileType = llvm::ArrayType::get(builder.getInt32Ty(), REG_FILE_SIZE);
    module->getOrInsertGlobal("regFile", regFileType);
    llvm::GlobalVariable *regFile = module->getNamedGlobal("regFile");

    // declare void @main()
    llvm::FunctionType *funcType = 
        llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = 
        llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
     // entry:
    llvm::BasicBlock *entryBB =
      llvm::BasicBlock::Create(context, "entry", mainFunc);

  builder.SetInsertPoint(entryBB);


    std::string name;
    std::string arg;

    double command;
    double cmdArg;

    std::unordered_map<std::string, llvm::BasicBlock*> BBMap;

    std::cout << "\n#[FILE]:\n";

    input >> name;

    command = std::stof(name);

    if (static_cast<int>(command) != ASSEM_ID)
    {
        fprintf(stderr, "Error ASSEM_ID\n");
        return;
    }

    using namespace my_commands;

    while (input >> name)
    {
        command = std::stof(name);

        if (cmpCmd(command, CMD_ADD) || cmpCmd(command, CMD_ADDI) || cmpCmd(command, CMD_MUL) || cmpCmd(command, CMD_MULI) ||
            cmpCmd(command, CMD_DIV) || cmpCmd(command, CMD_DIVI) || cmpCmd(command, CMD_SUB) || cmpCmd(command, CMD_SUBI) )
        { /* 3 args */
            input >> cmdArg >> cmdArg >> cmdArg;
            continue;
        } /* 1 arg */
        if (0)
        {
            input >> cmdArg;
            continue;
        } /* 0 arg */
        if (0)
        {
            continue;
        }
        
        std::cout << "BB " << command << "\n";
        BBMap[name] = llvm::BasicBlock::Create(context, name, mainFunc);
    }
    
    input.close();
    input.open("xxx");

    input >> name; /* ASSEM_ID */

    fprintf(stderr, "1\n");

    while (input >> name)
    {
        command = std::stof(name);

        input >> arg;
        std::cout << arg;
        // res
        llvm::Value *destRegNum = llvm::ConstantInt::get(builder.getInt64Ty(),
                                              (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

        input >> arg;
        std::cout << " = " << arg;
        llvm::Value *src1RegNum = llvm::ConstantInt::get(builder.getInt64Ty(),
                                              (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

        input >> arg;
        std::cout << " + " << arg << "\n";
        llvm::Value *src2RegNum = llvm::ConstantInt::get(builder.getInt64Ty(),
                                              (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

        llvm::FunctionType *CalleType = llvm::FunctionType::get(
            builder.getVoidTy(),
            llvm::ArrayRef<llvm::Type *>( {builder.getInt32Ty(), builder.getInt32Ty()} ), 
            false);

        builder.CreateCall(module->getOrInsertFunction(
                       "funcAdd", CalleType),
                   llvm::ArrayRef<llvm::Value *>({destRegNum, src1RegNum, src2RegNum}));
        fprintf(stderr, "CREATE CALL\n");
        continue;

        

        /*
        if (0) // sort
        {
            input >> arg;
            std::cout << "sort " << arg << "\n";
            // arg1
            llvm::Value *arg_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            llvm::Value *arg = builder.CreateLoad(arg_p);

            llvm::Function *CalleeF = llvm::Function::Create(llvm::FunctionType::get(builder.getVoidTy(),
                                                                                    llvm::ArrayRef<llvm::Type*> (builder.getInt32Ty()), false),
                                                            llvm::Function::ExternalLinkage, "INSTR_sort", module);

            builder.CreateCall(CalleeF, llvm::ArrayRef<llvm::Value*>(arg));
            continue;
        }
        
        if (cmpCmd(command, CMD_ADD))
        {
            input >> arg;
            std::cout << arg;
            // res
            fprintf(stderr, "(%d)\n", std::stoi(arg) - static_cast<int>(CMD_RAX));
            llvm::Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg) - static_cast<int>(CMD_RAX) + 1);
            input >> arg;
            std::cout << " = " << arg;
            // arg1
            llvm::Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg) - static_cast<int>(CMD_RAX) + 1);
            input >> arg;
            std::cout << " + " << arg << "\n";
            // arg2
            
            fprintf(stderr, "1");
            llvm::Value *arg2_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg) - static_cast<int>(CMD_RAX) + 1);
            fprintf(stderr, "arg2 = %p\n", arg2_p);
            llvm::Value *add_arg1_arg2 = builder.CreateAdd(builder.CreateLoad(arg1_p), builder.CreateLoad(arg2_p));


            fprintf(stderr, "3");
            builder.CreateStore(add_arg1_arg2, res_p);
            
            fprintf(stderr, "DONE WITH ADDI");
            continue;
        }
        
        if (cmpCmd(command, CMD_ADDI))
        {
            input >> arg;
            std::cout << arg;
            // res
            llvm::Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " = " << arg;
            // arg1
            llvm::Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " + " << arg << "\n";
            // arg2
            llvm::Value *arg2 = llvm::ConstantInt::get(builder.getInt32Ty(), std::stoi(arg));
            llvm::Value *add_arg1_arg2 = builder.CreateAdd(builder.CreateLoad(arg1_p), arg2);
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }

        if (cmpCmd(command, CMD_MUL))
        {
            input >> arg;
            std::cout << arg;
            // res
            llvm::Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " = " << arg;
            // arg1
            llvm::Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " * " << arg << "\n";
            // arg2
            llvm::Value *arg2_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            llvm::Value *add_arg1_arg2 = builder.CreateMul(builder.CreateLoad(arg1_p), builder.CreateLoad(arg2_p));
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        
        if (cmpCmd(command, CMD_MULI))
        {
            input >> arg;
            std::cout << arg;
            // res
            llvm::Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " = " << arg;
            // arg1
            llvm::Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " * " << arg << "\n";
            // arg2
            llvm::Value *arg2 = llvm::ConstantInt::get(builder.getInt32Ty(), std::stoi(arg));
            llvm::Value *add_arg1_arg2 = builder.CreateMul(builder.CreateLoad(arg1_p), arg2);
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        
        if (cmpCmd(command, CMD_RET))
        {
            std::cout << "ret" << std::endl;
            llvm::Function *CalleeF = llvm::Function::Create(llvm::FunctionType::get(builder.getVoidTy(), false),
                                                   llvm::Function::ExternalLinkage, "INSTR_dumpValue", module);
            builder.CreateCall(CalleeF);
            builder.CreateRetVoid();
            if (input >> name)
            {
                std::cout << "BB " << name << "\n";
                builder.SetInsertPoint(BBMap[name]);
            }
            continue;
        }
        
        if (0) // BNE /        {
            input >> arg;
            std::cout << "if (" << arg;
            // reg1
            llvm::Value *reg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " != " << arg;
            // reg2
            llvm::Value *reg2_p = builder.CreateConstGEP2_32(regFileType, regFile, 0, std::stoi(arg.substr(1)));
            input >> arg;
            std::cout << " ) then BB:" << arg;
            input >> name;
            std::cout << " else BB:" << name << std::endl;
            std::cout << "BB " << name << "\n";
            
            llvm::Value *cond = builder.CreateICmpNE(builder.CreateLoad(reg1_p), builder.CreateLoad(reg2_p));
            builder.CreateCondBr(cond, BBMap[arg], BBMap[name]);
            builder.SetInsertPoint(BBMap[name]);
            continue;
        }
        
        if (0) // br
        {
            // name
            input >> arg;
            std::cout << "br BB:" << arg << std::endl;
            builder.CreateBr(BBMap[arg]);
            if (input >> name)
            {
                std::cout << "BB " << name << "\n";
                builder.SetInsertPoint(BBMap[name]);
            }
            continue;
        }
        std::cout << "BB " << name << "\n";

        builder.CreateBr(BBMap[name]);
        builder.SetInsertPoint(BBMap[name]);

        */
    
       
    }

    builder.CreateRet(llvm::ConstantInt::get(builder.getInt32Ty(), 0));

    std::cout << "\n#[LLVM IR] DUMP:\n\n";
    std::string s;
    llvm::raw_string_ostream os(s);
    module->print(os, nullptr);
    os.flush();
    std::cout << s;

    std::cout << "\n#[LLVM IR] DUMP END\n\n\n";

    for (int i = 0; i < REG_FILE_SIZE; i++)
        REG_FILE[i] = 0;

    std::cout << "#[LLVM EE] RUN\n";

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::ExecutionEngine *ee =
      llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
    ee->InstallLazyFunctionCreator(lazyFunctionCreator);

    ee->addGlobalMapping(regFile, (void *)REG_FILE);
    ee->finalizeObject();
    std::vector<llvm::GenericValue> noargs;

    ee->runFunction(mainFunc, noargs);

    std::cout << "#[LLVM EE] END\n";

    for (int i = 0; i < REG_FILE_SIZE; i++)
        std::cout << "[" << i << "] " <<REG_FILE[i] << "\n";

    return;
}