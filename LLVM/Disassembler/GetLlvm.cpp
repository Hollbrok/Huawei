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

    while (input >> name)
    {
        command = std::stof(name);

        if (0) /* sort */
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
        
        if (0) /*  BNE */
        {
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
        
        if (0) /* br */
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
    }

    std::cout << "\n#[LLVM IR]:\n";
    std::string s;
    llvm::raw_string_ostream os(s);
    module->print(os, nullptr);
    os.flush();
    std::cout << s;

    llvm::verifyFunction(*mainFunc);
    std::cout << "\n#[Running code]\n";
    llvm::ExecutionEngine *ee = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
    ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
        //if (fnName == "INSTR_dumpValue") { return reinterpret_cast<void *>(INSTR_dumpValue); }
        //if (fnName == "INSTR_sort") { return reinterpret_cast<void *>(INSTR_sort); }
        return nullptr;
    });

    ee->addGlobalMapping(regFile, (void*)REG_FILE);
    ee->finalizeObject();
    std::vector<llvm::GenericValue> noargs;
    ee->runFunction(mainFunc, noargs);
    std::cout << "#[Code was run]\n";

    for (int i = 0; i < REG_FILE_SIZE; i++)
    {
        std::cout << "[" << i << "] " << REG_FILE[i] << "\n";
    }

    return;
}