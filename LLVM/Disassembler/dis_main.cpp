#include "disassembler.h"

int main(int argc, char *argv[])
{
    printf("Disassembling in progress..\n");

    setlocale(LC_ALL, "russian");
 
    if (argc != 2)
    {
        fprintf(stderr, "program needs 1 argument (file-name).\n");
        return EXIT_FAILURE;
    }


    FILE* text = fopen(argv[1], "r");
    assert(text);

    FILE* result = fopen("disassem_result[for user].txt",   "wb");
    assert(result);

    struct Bytecode bytecode = {};
    make_bytecode(text, &bytecode);

    disassembler(&bytecode, result);

    printf("DONE!!\n");

    bytecode_destruct(&bytecode);

    return 0;
}
