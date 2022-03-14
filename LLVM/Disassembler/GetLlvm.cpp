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

static const int REG_FILE_SIZE = 4;
static uint32_t REG_FILE[REG_FILE_SIZE] = {};

/* (i)dcmd == cmd */
static inline bool cmpCmd(double dCmd, my_commands::Commands cmd)
{
    return static_cast<int>(dCmd) == cmd;
}

/* dest = src1 + src2*/
static void funcAdd(int destN, int src1N, int src2N)
{
    REG_FILE[destN] = REG_FILE[src1N] + REG_FILE[src2N];
}

/* dest = src1 + number*/
static void funcAddi(int destN, int src1N, double number)
{
    REG_FILE[destN] = REG_FILE[src1N] + number;
}

/* dest = src1 / src2*/
static void funcDiv(int destN, int src1N, int src2N)
{
    REG_FILE[destN] = REG_FILE[src1N] / REG_FILE[src2N];
}

/* dest = src1 / number*/
static void funcDivi(int destN, int src1N, double number)
{
    REG_FILE[destN] = REG_FILE[src1N] / number;
}

/* dest = src1 * src2*/
static void funcMul(int destN, int src1N, int src2N)
{
    REG_FILE[destN] = REG_FILE[src1N] * REG_FILE[src2N];
}

/* dest = src1 * number */
static void funcMuli(int destN, int src1N, double number)
{
    REG_FILE[destN] = REG_FILE[src1N] * number;
}

/* dest = src1 - src2*/
static void funcSub(int destN, int src1N, int src2N)
{
    REG_FILE[destN] = REG_FILE[src1N] - REG_FILE[src2N];
}

/* dest = src1 - number*/
static void funcSubi(int destN, int src1N, double number)
{
    REG_FILE[destN] = REG_FILE[src1N] - number;
}

static std::string defineStrCmdType(enum my_commands::Commands cmd)
{
    using namespace my_commands;
    switch (cmd)
    {
    case CMD_ADD:
        return std::string("Add");    
    case CMD_SUB:
        return std::string("Sub");
    case CMD_MUL:
        return std::string("Mul");
    case CMD_DIV:
        return std::string("Div");
    case CMD_ADDI:
        return std::string("Addi");
    case CMD_SUBI:
        return std::string("Subi");
    case CMD_MULI:
        return std::string("Muli");
    case CMD_DIVI:
        return std::string("Divi");
    default:
        return std::string("Error");
    }
}

static void funcError(int destN, int src1N, int src2N)
{
    fprintf(stderr, "Function [funcError] was called.\n");
}

static void *lazyFunctionCreator(const std::string &fnName) 
{
    if (fnName == "funcAdd")
        return reinterpret_cast<void *>(funcAdd);
    if (fnName == "funcSub")
        return reinterpret_cast<void *>(funcSub);
    if (fnName == "funcMul")
        return reinterpret_cast<void *>(funcMul);
    if (fnName == "funcDiv")
        return reinterpret_cast<void *>(funcDiv);
    if (fnName == "funcAddi")
        return reinterpret_cast<void *>(funcAddi);
    if (fnName == "funcSubi")
        return reinterpret_cast<void *>(funcSubi);
    if (fnName == "funcMuli")
        return reinterpret_cast<void *>(funcMuli);
    if (fnName == "funcDivi")
        return reinterpret_cast<void *>(funcDivi);

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
    
    /* ; ModuleID = 'top'
       source_filename = "top" */

    llvm::Module* module = new llvm::Module("top", context);
    llvm::IRBuilder<> builder(context); 

    llvm::ArrayType *regFileType = llvm::ArrayType::get(builder.getInt32Ty(), REG_FILE_SIZE);

    module->getOrInsertGlobal("regFile", regFileType);
    llvm::GlobalVariable *regFile = module->getNamedGlobal("regFile");

    /* declare void @main() */

    llvm::FunctionType *funcType = 
            llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = 
            llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
     
    /* entry: */
    
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

    input >> name; /* skip ASSEM_ID */

    while (input >> name)
    { 
        command = std::stof(name);

        if (0)//(cmpCmd(command, CMD_ADD)) /* test LLVM IR generation without lazyfunctions */
        {
            /* dest reg */
            input >> arg;
            llvm::Value *destP = builder.CreateConstGEP2_32(regFileType, regFile, 0, 
                                        std::stoi(arg) - static_cast<int>(CMD_RAX));

            /* src1 reg */
            input >> arg;
            llvm::Value *src1P = builder.CreateConstGEP2_32(regFileType, regFile, 0, 
                                        std::stoi(arg) - static_cast<int>(CMD_RAX));
            
            /* src2 reg */
            input >> arg;
            llvm::Value *src2P = builder.CreateConstGEP2_32(regFileType, regFile, 0, 
                                        std::stoi(arg) - static_cast<int>(CMD_RAX));
            
            llvm::Value *addResult = builder.CreateAdd(builder.CreateLoad(src1P), builder.CreateLoad(src2P));

            builder.CreateStore(addResult, destP);
            
            continue;
        }
        
        /* arguments are 3 registers */
        if (cmpCmd(command, CMD_ADD) || cmpCmd(command, CMD_SUB) || cmpCmd(command, CMD_MUL) || cmpCmd(command, CMD_DIV))
        {
            enum Commands curCmd = static_cast<enum Commands>(command);

            input >> arg;
            llvm::Value *destRegNum = llvm::ConstantInt::get(builder.getInt32Ty(),
                                                (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

            input >> arg;
            llvm::Value *src1RegNum = llvm::ConstantInt::get(builder.getInt32Ty(),
                                                (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

            input >> arg;
            llvm::Value *src2RegNum = llvm::ConstantInt::get(builder.getInt32Ty(),
                                                (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

            /* builder.getInt32Ty() === int (number of register) */
            llvm::FunctionType *CalleType = llvm::FunctionType::get(
                    builder.getVoidTy(),
                    llvm::ArrayRef<llvm::Type *>( {builder.getInt32Ty(), builder.getInt32Ty()} ), 
                    false);

            std::string strCmdType = defineStrCmdType(curCmd);

            builder.CreateCall(module->getOrInsertFunction(
                       "func" + strCmdType, CalleType),
                        llvm::ArrayRef<llvm::Value *>(
                        {destRegNum, src1RegNum, src2RegNum} ) );
            
            continue;
        }

        /* arguments are 2 registers and 1 imm*/
        if (cmpCmd(command, CMD_ADDI) || cmpCmd(command, CMD_SUBI) || cmpCmd(command, CMD_MULI) || cmpCmd(command, CMD_DIVI))
        {
            enum Commands curCmd = static_cast<enum Commands>(command);

            input >> arg;
            llvm::Value *destRegNum = llvm::ConstantInt::get(builder.getInt32Ty(),
                                                (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

            input >> arg;
            llvm::Value *src1RegNum = llvm::ConstantInt::get(builder.getInt32Ty(),
                                                (uint64_t)std::stoi(arg) - static_cast<int>(CMD_RAX));

            input >> arg;
            //llvm::Value *arg2 = llvm::ConstantInt::get(builder.getInt32Ty(), std::stoi(arg));

            llvm::Value *srcNum = llvm::ConstantInt::get(builder.getInt64Ty(),
                                                std::stof(arg));

            llvm::FunctionType *CalleType = llvm::FunctionType::get(
                    builder.getVoidTy(),
                    llvm::ArrayRef<llvm::Type *>( {builder.getInt32Ty(), builder.getInt64Ty()} ), 
                    false);

            std::string strCmdType = defineStrCmdType(curCmd);

            builder.CreateCall(module->getOrInsertFunction(
                       "func" + strCmdType, CalleType),
                        llvm::ArrayRef<llvm::Value *>(
                        {destRegNum, src1RegNum, srcNum} ) );
            
            continue;
        }
        fprintf(stderr, "UNDEFINIED FUNC [name = %s; cmd = %lg].\n", name, command);

        

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

    std::cout << "\n#[LLVM IR] END OF DUMP\n\n\n";

    for (int i = 0; i < REG_FILE_SIZE; i++)
        REG_FILE[i] = 0;

    llvm::verifyFunction(*mainFunc);

    std::cout << "#[LLVM EE] RUN:\n";

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
        std::cout << "[" << i << "] " << REG_FILE[i] << "\n";

    return;
}