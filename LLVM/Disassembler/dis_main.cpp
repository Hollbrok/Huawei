#include "disassembler.h"

void getLlvm(Bytecode *bytecode, FILE *llResult);

int main(int argc, char *argv[])
{
    printf("Disassembling in progress..\n");

    setlocale(LC_ALL, "russian");
 
    if (argc != 2)
    {
        fprintf(stderr, "./dis <.hol file>.\n");
        return EXIT_FAILURE;
    }

    FILE* text = fopen(argv[1], "r");
    assert(text);

    FILE* disResult = fopen("disassem.asm", "wb");
    assert(disResult);

    FILE* llResult = fopen("xxx.ll", "wb");
    assert(llResult);

    //fprintf(result, "\n");

    struct Bytecode bytecode = {};
    make_bytecode(text, &bytecode);

    //disassembler(&bytecode, disResult);
    
    fprintf(stderr, "!!!!!");
    getLlvm(&bytecode, llResult);
    fprintf(stderr, "!!!!!");

    bytecode_destruct(&bytecode);

    pclose(text);
    pclose(disResult);
    pclose(llResult);

    printf("DONE!!\n");
    return 0;
}
