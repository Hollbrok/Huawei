#include "disassembler.h"

auto make_bytecode(FILE* text, Bytecode* byte_struct) -> void
{
    fseek(text, 0, SEEK_END);
    int file_length = ftell(text);
    fseek(text, 0, SEEK_SET);
    file_length++;


    char* buffer_char = (char*) calloc(file_length, sizeof(char));
    assert(buffer_char);

    fread(buffer_char, sizeof(char), file_length, text);

    //fprintf(disResult, "\n");

    int space_counter = 0;
    for(int index = 0; index < file_length; index++)
        if(buffer_char[index] == ' ')
            space_counter++;
    
    //fprintf(disResult, "\n");

    byte_struct->bytecode_capacity = space_counter;

    fprintf(stderr, "calloc %d elems (bytes = %d)\n", byte_struct->bytecode_capacity, byte_struct->bytecode_capacity * sizeof(double));
    fprintf(stderr, "file_length = %d\n", file_length);

    byte_struct->data     = (double*) calloc(byte_struct->bytecode_capacity, sizeof(double));
    assert(byte_struct->data);

    //fprintf(disResult, "\n");


    int ass_cur_size = 0;

    //fprintf("");

    while (*buffer_char)//&& (curLength != file_length))
    {
        double temp_val = get_number(&buffer_char);
        
        byte_struct->data[ass_cur_size++] = temp_val;
        ignore_spaces(&buffer_char);
    }


    fprintf(stderr, "4\n");
    byte_struct->bytecode_capacity = ass_cur_size;
    fprintf(stderr, "ass_cur_size = %d\n", ass_cur_size);

    return;
}

auto ignore_spaces(char** buffer) -> void
{
    while (isspace(**buffer))
        (*buffer)++;
}

auto bytecode_destruct(Bytecode* byte_struct) -> void //перед free нужно еще пройтись по всем элементам занулить их
{
    assert(byte_struct);

    free(byte_struct->data);
    byte_struct->data = nullptr;

    byte_struct->bytecode_capacity = 0;
}

auto disassembler(Bytecode* byte_struct, FILE* disResult) -> void
{
    assert(byte_struct);
    assert(disResult);

    int skip_first  = -1;
    int skip_second = -1;

    if (byte_struct->data[0] != ASSEM_ID)
    {
        fprintf(stderr, "Incorrect assem ID.\n");
        return;
    }

    using namespace my_commands;

    for (int i = 1; i < byte_struct->bytecode_capacity; i++)
    {
        fprintf(stderr, "i = %d\n", i);


        int command = static_cast<int>(byte_struct->data[i]);
        
        fprintf(stderr, "TEST1\n");

        fprintf(stderr, "byte_struct->data[%d] = %d\n", i, command);
        
        switch(static_cast<int>(command))
        {
            case static_cast<int>(Commands::CMD_HLT):/*hlt*/
            {
                fprintf(disResult, "hlt\n");
                break;
            }
            case static_cast<int>(Commands::CMD_PUSH):/*push(то есть error, так как если push, то сюда не должно дойти)*/
            {
                FILE* error = fopen("ERROR_PRINT.txt", "ab");
                fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                fprintf(error, "ERROR in LINE %d", __LINE__);
                fclose(error);
                break;
            }
            case static_cast<int>(Commands::CMD_ADD):/*add*/
            {
                fprintf(disResult, "add\n");
                break;
            }
            case static_cast<int>(Commands::CMD_MUL):/*mul*/
            {
                fprintf(disResult, "mul\n");
                break;
            }
            case static_cast<int>(Commands::CMD_DIV):/*div*/
            {
                fprintf(disResult, "div\n");
                break;
            }
            case static_cast<int>(Commands::CMD_SUB):/*sub*/
            {
                fprintf(disResult, "sub\n");
                break;
            }
            case static_cast<int>(Commands::CMD_SIN):/*sin*/
            {
                fprintf(disResult, "sin\n");
                break;
            }
            case static_cast<int>(Commands::CMD_COS):/*cos*/
            {
                fprintf(disResult, "cos\n");
                break;
            }
            case static_cast<int>(Commands::CMD_POW):/*pow*/
            {
                fprintf(disResult, "pow\n");
                break;
            }
            case static_cast<int>(Commands::CMD_SQRT):/*sqrt*/
            {
                fprintf(disResult, "sqrt\n");
                break;
            }
            case static_cast<int>(Commands::CMD_IN):/*in*/
            {
                fprintf(stderr, "before IN\n");
                fprintf(disResult, "in\n");
                fprintf(stderr, "after IN is printfed\n");
                    
                break;
            }
            case static_cast<int>(Commands::CMD_OUT):/*out*/
            {
                fprintf(disResult, "out\n");
                break;
            }
            case static_cast<int>(Commands::CMD_DEL):/*del*/
            {
                fprintf(disResult, "del\n");
                break;
            }
            case static_cast<int>(Commands::CMD_LN):/*ln*/
            {
                fprintf(disResult, "ln\n");
                break;
            }
            case static_cast<int>(Commands::CMD_LOG10):/*log10*/
            {
                fprintf(disResult, "log10\n");
                break;
            }
            case static_cast<int>(Commands::CMD_LOG2):/*log2*/
            {
                fprintf(disResult, "log2\n");
                break;
            }
            case static_cast<int>(Commands::CMD_JMP):/*23*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "jmp :LAB%d\n", number_command);
                break;
            }
            case static_cast<int>(Commands::CMD_LABEL):/*22*/
            {
                fprintf(disResult, "LAB%d:\n", i);
                break;
            }
            case static_cast<int>(Commands::CMD_JE):/*24 ==*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "je :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_JAB):/*25 !=*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "jab :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_JAE):/*26 >=*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "jae :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_JBE):/*27 <=*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "jbe :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_JA):/*28 >*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "ja :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_JB):/*29*/
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "jb :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            case static_cast<int>(Commands::CMD_RET):
            {
                fprintf(disResult, "ret\n");
                break;
            }
            case static_cast<int>(Commands::CMD_CALL):
            {
                int number_command = static_cast<int>(byte_struct->data[i + 1]);
                fprintf(disResult, "call :LAB%d\n", number_command);
                skip_first = i + 1;
                break;
            }
            default:
            {
                FILE* error = fopen("[!]ERRORS.txt", "ab");
                fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                fprintf(error, "Disassemblers doesn't know this command..\n");
                fprintf(error, "bytecode[%d] = [%d]", i, static_cast<int>(byte_struct->data[i]));
                fclose(error);
            }
        }
        fprintf(stderr, "after else\n");
    }

    return;
}

auto get_number(char** buffer) -> double
{
    ignore_spaces(buffer);
    
    double number = atof(*buffer);

    if (**buffer == '-')
    {
        number *= -1;
        (*buffer)++;
    }

    while (isdigit(**buffer) || (**buffer == ',') || (**buffer == '.'))
        (*buffer)++;


    //ignore_spaces(buffer);
    return number;
}

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;


    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    fprintf(stderr, "malloc string_time = %d\n", string_time);
    tmp = (char*) malloc(sizeof(string_time));
    assert(tmp);

    strcpy(tmp, string_time);

    return(tmp);
}

auto define_date(void) -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}
