#include "cpu.h"

static int SECOND_PRINT            = 0;

Bytecode::Bytecode(FILE* text) :
	unknown_command_(-666),
	bytecode_capacity_(0),
	error_state_(0),
	find_hlt_(0)
{
	assert(this && "You passed nullptr to Bytecode construct");
	assert(text && "You passed ");

    fseek(text, 0, SEEK_END);
    int file_length = ftell(text);
    fseek(text, 0, SEEK_SET);

    file_length++;

    char* buffer_char = (char*) calloc(file_length, sizeof(char));
    assert(buffer_char);

    fread(buffer_char, sizeof(char), file_length, text);

    int space_counter = 0;

    for(int index = 0; index < file_length; index++)
        if(buffer_char[index] == ' ')
            space_counter++;

    bytecode_capacity_ = space_counter;

   	data_ = (double*) calloc(bytecode_capacity_, sizeof(double));
    assert(data_ && "Can't calloc memory for data_");


    char* temp_pointer = buffer_char;
    int temp_iter = 0;
    while (*temp_pointer)
       data_[temp_iter++] = get_number(&temp_pointer);

    free(buffer_char);
    buffer_char = nullptr;
    bytecode_capacity_ = temp_iter;
    return;
}

Bytecode::~Bytecode()
{
    assert(this && "You paddes nullptr to destruct");

    free(data_);
    data_ = nullptr;

	error_state_ = -1;
	unknown_command_ = -666;

    bytecode_capacity_ = -1;
	find_hlt_ = -1;
}

void Bytecode::CPU()
{
    assert(this && "You passed nullptr to CPU()");
	using namespace my_commands;

    struct Rix rix_struct = {};

	Stack stk("def_stack", START_SIZE);
    Stack stk_call("stack_call", START_SIZE);

    char* OP = (char*) calloc(OP_SIZE, sizeof(char));
    assert(OP && "Can't calloc memory for OP");

	check_assem_id

    else for (int i = 1; !find_hlt_; i++)
    {
		int command = static_cast<int>(data_[i]);

        if (get_byte(command, BIT_PUSH))	// (command == CMD_PUSH) // PUSH
        {
			cmd_push_exe(command, i, &stk, &rix_struct, OP);//error_state_ += cmd_push_exe(command, i, &stk, &rix_struct, OP);
			i++;
        }

        else if(get_byte(command, BIT_POP))	//(command == CMD_POP) //POP
        {
			cmd_pop_exe(command, i, &stk, &rix_struct, OP);//error_state_ += cmd_pop_exe(command, i, &stk, &rix_struct, OP);
            i++;
        }

        else
        {
            switch(command)
            {
                case CMD_HLT:
                    find_hlt_ = true;
                    break;
                case CMD_ADD:
					stk_size_check(2, not enough numbers in the stack. Exit..\n, stk, get_cur_size())
                    else
                        stk.push(stk.pop() + stk.pop());
                    break;
                case CMD_MUL:
					stk_size_check(2, not enough numbers to mul\n, stk, get_cur_size())
                    else
                        stk.push(stk.pop() * stk.pop());
                    break;
                case CMD_DIV:
                {
					stk_size_check(2, not enough numbers to div\n, stk, get_cur_size())
                    else
                    {
                        double x1 = stk.pop();//pop_stack(stk);
                        double x2 = stk.pop();
                        printf("x1 = [%lg], x2 = [%lg]\n", x1, x2);
                        stk.push(x2 / x1);
                    }
                    break;
                }
                case CMD_SUB:
                {
					stk_size_check(2, not enough numbers to sub\n, stk, get_cur_size())
                    {
                        double x1 = stk.pop();
                        double x2 = stk.pop();
                        stk.push(x2 - x1);//push_stack(Stack, x2 - x1);
                    }
                    break;
                }
                case CMD_SIN:
                    stk.push(sin(stk.pop())); //push_stack(Stack, sin(pop_stack(Stack)));
                    break;
                case CMD_COS:
                    stk.push(cos(stk.pop())); //push_stack(Stack, cos(pop_stack(Stack)));
                    break;
                case CMD_POW:
                {
					stk_size_check(2, no numbers to pow\n, stk, get_cur_size())
                    {
                        double x1 = stk.pop();//pop_stack(Stack);
                        double x2 = stk.pop();//pop_stack(Stack);
                        stk.push(pow(x2, x1));//push_stack(Stack, pow(x2, x1));
                    }
                    break;
                }
                case CMD_SQRT:
                    stk.push(sqrt(stk.pop())); //push_stack(Stack, sqrt(pop_stack(Stack)));
                    break;
                case CMD_IN:
                {
                    printf("Enter number:");
                    double x1;
                    scanf("%lg", &x1);
                    stk.push(x1);//push_stack(Stack, x1);

                    break;
                }
                case CMD_OUT:
                {
					stk_size_check(1, not enough space in Stack to out. Exit..\n, stk, get_cur_size())
                    else
                    {
                        double x1 = stk.pop();//pop_stack(Stack);

                        if(!SECOND_PRINT)
                        {
                            FILE* result = fopen("results[for user].txt", "wb");
                            SECOND_PRINT = true;
                            fprintf(result, "[%lg]\n", x1);
                            fclose(result);
                        }

                        else
                        {
                            FILE* result = fopen("results[for user].txt", "a");
                            assert(result);
                            fprintf(result, "[%lg]\n", x1);
                            fclose(result);
                        }

                        stk.push(x1);//push_stack(Stack, x1);
                    }
                    break;
                }
                case CMD_DEL:
                    stk.pop();
                    //pop_stack(Stack);
                    break;
                case CMD_LN:
                    stk.push(log(stk.pop()));
                    //push_stack(Stack, log(pop_stack(Stack)));
                    break;
                case CMD_LOG10:
                    stk.push(log10(stk.pop()));
                    //push_stack(Stack, log10(pop_stack(Stack)));
                    break;
                case CMD_LOG2:
                    stk.push(log2(stk.pop()));
                    //push_stack(Stack, log2(pop_stack(Stack)));
                    break;
                case CMD_CALL:
                    stk_call.push(i);
                    //push_stack(Stack_call, i);
                    i = static_cast<int>(data_[i + 1]);
                    break;
                case CMD_RET:
                    if(stk_call.get_cur_size() < 1)
                    {
                        printf("no number in stack_call to return\n");
                        error_state_ += ERROR_STK_CALL_SIZE;
                        break;
                    }

                    i = static_cast<int>(stk_call.pop()) + 1;

                    break;
				case CMD_LABEL:
                    break;
				case CMD_JE:
				case CMD_JAB:
				case CMD_JAE:
				case CMD_JBE:
				case CMD_JA:
 				case CMD_JB:
				case CMD_JMP:
				{
					cmd_compair_exe(command, &stk, &i);//error_state_ += cmd_compair_exe(command, &stk, &i);
					break;
				}

				case CMD_ABS:
					stk_size_check(1, not enough numbers to abs\n, stk, get_cur_size())
                    else
                        stk.push(abs(stk.pop()));
                    break;
                default:
					error_state_ += ERROR_UNKNOWN_COM;
					unknown_command_ = command;

					break;
            }
        }
    }

	determine_status(&stk);

    free(OP);
    OP = nullptr;
}

void Bytecode::determine_status(Stack* stk)
{
	assert(this && "You passed nullptr to determine_status");

	using namespace my_commands;

	FILE* error = fopen("[!]DUMP_CPU.txt", "w");
	assert(error && "Can't open dump_cpu.txt");

    fprintf(error, "\tData of DUMP in CPU : %s (dd/mm/yy)\n\n", define_date());

	if(!error_state_)
		print_good(error);
	else
		for(int i = 0; i < NUMBER_OF_ERRORS; i++)
			if(get_byte(error_state_, i + 1))
				print_error(error, i + 1);

    if(stk->get_cur_size() == 0)
        fprintf(error, "Stack is empty\n");
	else
	{
        fprintf(error, "Numbers in Stack:\n");
        for(int i = 0; i < stk->get_cur_size(); i++)
            fprintf(error, "%d.[%lg]\n", i + 1, stk->get_data_num(i));
    }
    fclose(error);
}

void Bytecode::cmd_push_exe(int command, int i, Stack* stk, Rix* rix_struct, char* OP)
{
    using namespace my_commands;

	if (!get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER))
            stk->push(data_[i + 1]);
		else
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
                stk->push(rix_struct->rax);
			else if (get_byte(rix_number, BIT_RBX))
                stk->push(rix_struct->rbx);
			else if (get_byte(rix_number, BIT_RCX))
                stk->push(rix_struct->rcx);
			else if (get_byte(rix_number, BIT_RDX))
                stk->push(rix_struct->rdx);
		}
	}
		//* Проверки на значение в rix, чтобы не было OP[-1]
	else if (get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			double *temp_pointer = (double*) (OP + static_cast<int>(data_[i + 1]));
            stk->push(*temp_pointer);
        	//push_stack(Stack, *temp_pointer);
		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rax));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rbx));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rcx));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
            	double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rdx));
            	stk->push(*temp_pointer);
            }
		}
	}
		//* Проверки на значение в rix, чтобы не было OP[-1]
	else if (get_byte(command, BIT_C_OP) && !get_byte(command, BIT_D_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			char *temp_pointer = (char*) (OP + static_cast<int>(data_[i + 1]));
            stk->push(*temp_pointer);
		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rax));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rbx));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rcx));
            	stk->push(*temp_pointer);
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
            	char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct->rdx));
            	stk->push(*temp_pointer);
            }
		}
	}
	else
		error_state_ += ERROR_CMD_PUSH; //return ERROR_CMD_PUSH;
}

void Bytecode::cmd_pop_exe(int command, int i, Stack *stk, Rix *rix_struct, char *OP)
{
	assert(this && "You passed nullptr to cmd_pop_exe");
	assert(stk && "You passed nullptr Stack");
	assert(rix_struct && "You passed nullptr rix_struct");
	assert(OP && "You passed nullptr OP");

    using namespace my_commands;

	if(stk->get_cur_size() < 1)
    {
    	printf("stk size < 1. Not enough to pop\n");
        error_state_ += ERROR_STACK_SIZE;
    }

    else if (!get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER))
			stk->pop();//pop_stack(Stack);
		else if (!get_byte(command, BIT_NUMBER))
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
				rix_struct->rax = stk->pop();
			else if (get_byte(rix_number, BIT_RBX))
				rix_struct->rbx = stk->pop();
			else if (get_byte(rix_number, BIT_RCX))
				rix_struct->rcx = stk->pop();
			else if (get_byte(rix_number, BIT_RDX))
				rix_struct->rdx = stk->pop();
		}
	}

	else if (get_byte(command, BIT_D_OP) && !get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			double* temp_pointer = (double*) (OP + static_cast<int>(data_[i + 1]));
            *temp_pointer = static_cast<double> (stk->pop());

		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
                //if(rix_struct->rax < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rax));
                    *temp_pointer      = static_cast<double> (stk->pop());
                }
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
                //if(rix_struct->rbx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rbx));
                    *temp_pointer      = static_cast<double> (stk->pop());
                }
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
                //if(rix_struct->rcx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rcx));
                    *temp_pointer      = static_cast<double> (stk->pop());
                }
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
                //if(rix_struct->rdx < 0)
                //    EXIT_CONDITION = true;
                //else
                {
                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct->rdx));
                    *temp_pointer      = static_cast<double> (stk->pop());
                }
            }
		}
	}

	else if (!get_byte(command, BIT_D_OP) && get_byte(command, BIT_C_OP))
	{
		if (get_byte(command, BIT_NUMBER)) 												// NUMBER
		{
			char* temp_pointer = static_cast<char*> (OP + static_cast<int>(data_[i + 1]));
            *temp_pointer = static_cast<char> (stk->pop());

		}
		else if (!get_byte(command, BIT_NUMBER)) 		// NOT NUMBER == RIX
		{
			int rix_number = static_cast<int>(data_[i + 1]);

			if (get_byte(rix_number, BIT_RAX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rax));
            	*temp_pointer      = static_cast<char> (stk->pop());
            }
			else if (get_byte(rix_number, BIT_RBX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rbx));
            	*temp_pointer      = static_cast<char> (stk->pop());
            }
			else if (get_byte(rix_number, BIT_RCX))
			{
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rcx));
            	*temp_pointer      = static_cast<char> (stk->pop());
            }
			else if (get_byte(rix_number, BIT_RDX))
			{
                //printf("POP rdx = %lg\n", rix_struct->rdx);
            	char* temp_pointer = static_cast<char*> (OP + static_cast<int>(rix_struct->rdx));
            	*temp_pointer      = static_cast<char> (stk->pop());
            }
		}
	}
	else
		error_state_ += ERROR_CMD_POP; //return ERROR_CMD_POP;
}

void Bytecode::cmd_compair_exe(int command, Stack* stk, int *i)
{
	assert(this && "You passed nullptr to cmd_compair_exe");
	assert(stk && "You passed nullptr Stack");
	//assert(rix_struct && "You passed nullptr rix_struct");
	//assert(OP && "You passed nullptr OP");

    using namespace my_commands;


	if((stk->get_cur_size() < 2) && !get_byte(command, BIT_JUMP))
    {
        error_state_ += ERROR_COMPAIR;
		printf("You are trying to call conditional jump but stk cur_size is less than 2\n");
    }
    else if (get_byte(command, BIT_JUMP))
    {
        (*i) = static_cast<int>(data_[(*i) + 1]);
    }
	else
	{
        double x1 = stk->pop();//pop_stack(Stack);
        double x2 = stk->pop();//pop_stack(Stack);

        if (get_byte(command, BIT_EQUAL))
        {
            //printf("BIT EQUAL\n");
            if (get_byte(command, BIT_BELOW) && !get_byte(command, BIT_ABOVE)) 		// <=
            {
                //printf("JBE\n");
                if (x2 <= x1)
                    (*i) = static_cast<int>(data_[(*i) + 1]);
                else (*i)++;
            }
            else if (!get_byte(command, BIT_BELOW) && get_byte(command, BIT_ABOVE)) // >=
            {
                if (x2 >= x1)
                    (*i) = static_cast<int>(data_[(*i) + 1]);
                else (*i)++;
            }
            else if (!get_byte(command, BIT_BELOW) && !get_byte(command, BIT_ABOVE)) // ==
            {
                if (is_equal(x2,  x1))
                    (*i) = static_cast<int>(data_[(*i) + 1]);
                else (*i)++;
            }
            else if  (get_byte(command, BIT_BELOW) && get_byte(command, BIT_ABOVE)) // !=
            {
                if (!is_equal(x2, x1))
                    (*i) = static_cast<int>(data_[(*i) + 1]);
                else (*i)++;
			}
        }
        else if (get_byte(command, BIT_ABOVE))
        {
            if (x2 > x1)
                (*i) = static_cast<int>(data_[(*i) + 1]);
            else (*i)++;
        }
        else if (get_byte(command, BIT_BELOW))
        {
            if (x2 < x1)
                (*i) = static_cast<int>(data_[(*i) + 1]);
            else (*i)++;
        }
        else
			error_state_ += ERROR_COMPAIR;//return ERROR_COMPAIR;
    }

	return;
}

auto get_number(char** buffer) -> double
{
    ignore_spaces(buffer);
    double number = atof(*buffer);

    while (isdigit(**buffer) || (**buffer == '.') || (**buffer == '-'))
        (*buffer)++;

    ignore_spaces(buffer);
    return number;
}

auto ignore_spaces(char** buffer) -> void
{
    while (isspace(**buffer))
        (*buffer)++;
}

auto print_for_user(Stack* stk) -> void
{
    FILE* result = fopen("results[for user].txt", "wb");

    if(stk->get_cur_size() > 0)
    {
        int Size = stk->get_cur_size();
        int max_order = 0;

        while(Size > 9)
        {
            Size /= 10;
            max_order++;
        }

        for(int i = 0; i < stk->get_cur_size(); i++)
        {

            int order  = 0;
            int i_copy = i + 1;

            while(i_copy > 9)
            {
                i_copy /= 10;
                order++;
            }

            switch(order)
            {
                case 0:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, stk->get_data_num(i));
                    break;
                }

                case 1:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 1; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, stk->get_data_num(i));
                    break;
                }

                case 2:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 2; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, stk->get_data_num(i));
                    break;
                }
                case 3:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 3; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, stk->get_data_num(i));
                    break;
                }
                case 4:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 4; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, stk->get_data_num(i));
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "w");
                    fprintf(error, "Too many numbers in stack\n");
                    fclose(error);
                }
            }
        }
    }
    else
		fprintf(result, "Stack size is below than 1\n"
                         "Currently size of Stack is %d", stk->get_cur_size());

    fclose(result);
}

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;

    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

auto define_date() -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}

auto is_equal(double a, double b) -> bool
{
	double diff = fabs(a - b);

	if (diff <= absEpsilon)
		return true;

	return diff <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

auto print_good(FILE* error) -> void
{
	assert(error);

	fprintf(error, "CPU ended without any errors\n");

	return;
}

auto print_error(FILE* error, int error_bit) -> void
{
    using namespace my_commands;
	switch (error_bit)
    {
        case ID_ID:
            fprintf(error, "Unknown CPU ID. Unfortunately, this processor can only process the code of a certain assembler\n");
            break;
        case ID_STACK_SIZE:
            fprintf(error, "At some point in the program, you tried to take more from the stack than it can\n");
            fprintf(error, "For example: Stack size is 1, but u tried to do \"add\" or \"mul\", etc. \n");
            break;
        case ID_UNKNOWN_COM:
            fprintf(error, "The processor did not recognize one of the commands\n");
            break;
        case ID_CMD_PUSH:
            fprintf(error, "Something went wrong in processing command \"push\"\n");
            break;
        case ID_CMD_POP:
            fprintf(error, "Something went wrong in processing command \"pop\"\n");
            break;
        case ID_COMPAIR:
            fprintf(error, "Something went wrong in any type of compair. For example jbe, jab, jump, etc.\n");
            break;
        case ID_STK_CALL_SIZE:
            fprintf(error, "No number in stack_call to return\n");
            break;
        default:
            fprintf(error, "Error in ERRORS. IT means that program has unknown error\n");
            break;
	}
}

