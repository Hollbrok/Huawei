#include "cpu.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Program needs 1 argument (file-name).\n");
        exit(EXIT_FAILURE);
    }

    printf("CPU in progress..\n");

    FILE* text = fopen(argv[1], "rb");
	assert(text && "Can't open asm-file");

	Bytecode byte_class(text);
    byte_class.CPU();

    printf("DONE!!\n");

    return 0;
}
