#include "assem.h"

//static int NEW_COMMAND_ERROR = 0; // If unknown command -- leave from assembling
static int IS_LAST_COMMAND_PUSH = 0;
static int IS_LAST_COMMAND_JMP = 0;

Text::Text(FILE *user_code, bool need_dump) :
	data_(nullptr),
	lines_(nullptr),
	num_of_lines_(0),
	length_file_(0),
	user_code_(user_code)
{
	assert(this && "You passed nullptr to Text construct\n");
	assert(user_code && "You must pass file* to text construct");

	// get_length(user_code);
	get_num_of_lines_buffer_length_of_file();

	lines_ = (Line*) calloc(num_of_lines_ + 1, sizeof(Line));
	assert(lines_ && "Can't calloc memory for lines_");

	Line *lines = (Line *)calloc(num_of_lines_ + 1, sizeof(Line));
	assert(lines && "Can't calloc memory for lines");

	get_lines_pointer(lines);

	free(lines);
	lines = nullptr;

	if(need_dump)
		dump();
}

int Text::get_length_file()
{
	assert(this && "You passed nullptr to get_length\n");
	return length_file_;
}

int Text::get_num_of_lines()
{
	assert(this && "You passed nullptr to get_num_of_line\n");
	return num_of_lines_;
}

int Text::get_length(size_t number_of_line)
{
	assert(this && "You passed nullptr to get_length\n");
	return lines_[number_of_line].length;
}

char Text::get_char(size_t number_of_line, size_t number_of_char)
{
	assert(this && "You passed nullptr to get_char\n");
	return lines_[number_of_line].line[number_of_char];
}

void Text::get_num_of_lines_buffer_length_of_file()
{
	// assert(user_code && "You must pass file* to get_num_of_lines");

	fseek(user_code_, 0, SEEK_END);
	long file_length = ftell(user_code_);
	fseek(user_code_, 0, SEEK_SET);

	file_length++;

	data_ = (char*) calloc(file_length, sizeof(char));
	assert(data_ && "Can't calloc memory for data_\n");

	while ((data_[length_file_] = fgetc(user_code_)) && (!feof(user_code_)))
	{
		if (data_[length_file_] == '\n')
			num_of_lines_++;
		length_file_++;
	}
}

void Text::get_lines_pointer(Line *lines)
{
	long length = 0;
	long n_structs = 0;

	while (true)
	{
		int iter_length = 0;

		while (isspace(data_[length]))
			length++;

		if (length >= length_file_)
			break;

		lines[n_structs].line = &(data_)[length];

		while ((data_[length] != '\n') && (length < length_file_))
		{
			if ((data_[length] == COMMENT_SYMBOL))
			{
				while (data_[length] != '\n')
					length++;
				break;
			}
			length++;
			iter_length++;
		}

		lines[n_structs].length = iter_length;
		while (isspace(lines[n_structs].line[lines[n_structs].length - 1]))
			(lines[n_structs].length)--;

		n_structs++;
	}

	for (int j = 0; j < n_structs; j++)
	{
		lines_[j].line = lines[j].line;
		lines_[j].length = lines[j].length;
	}

	num_of_lines_ = n_structs;
}

void Text::dump()
{
	FILE *info = fopen("[!]info.txt", "w");
	assert(info);

	fprintf(info, "Number of lines      = %d\n", num_of_lines_);
	fprintf(info, "Length of the file   = %d\n", length_file_);

	for (int x = 0; x < num_of_lines_; x++)
	{
		fprintf(info, "lines[%d]. [", x + 1);

		for (int y = 0; y < lines_[x].length; y++)
			fprintf(info, "%c", lines_[x].line[y]);

		fprintf(info, "]\n");
		fprintf(info, "\tlength = %ld\n\n", lines_[x].length);
	}

	fclose(info);
}

Text::~Text()
{
	assert(this && "Ypu passed nullptr to destructor\n");
	free(data_);
	data_ = nullptr;

	free(lines_);
	lines_ = nullptr;

	num_of_lines_ = -1;
	length_file_  = -1;

}

Code::Code(Text* text_class, bool need_dump) :
	data_(nullptr),
	length_(0),
	terms_(0),
	cur_size_(0)
{
	assert(text_class && "You passed nullptr(text_class) to Code construct\n");
	assert(this && "You passed nullptr to Code construct\n");

	length_ = text_class->get_length_file() + 3; // may be without + 3

	data_ = (char *)calloc(length_, sizeof(char));
	assert(data_ && "Can't calloc data_ in Code construct\n");

	int cur_size = 0;

	for (int x = 0; x < text_class->get_num_of_lines(); x++)
	{
		for (int y = 0; y < text_class->get_length(x); y++)
		{
			data_[cur_size++] = text_class->get_char(x, y);
			if (isspace(text_class->get_char(x, y)))
				while (isspace(text_class->get_char(x, y + 1)))
					y++;
		}
		if ((cur_size < length_ + 1))
			data_[cur_size++] = ' ';
	}

	data_[cur_size] = '\0';
	int terms = 0;
	terms_ = terms;

	for (int i = 0; i < length_; i++)
		if (data_[i] == ' ')
		{
			terms_++;
			while (isspace(data_[i]))
				i++;
		}

	if(need_dump)
		dump();
}

void Code::dump()
{
	assert(this && "You passed nullptr to dump\n");

	FILE *code_text = fopen("[!]code_dump.txt", "w");
	assert(code_text && "Can't open [!]code_dump.txt in Code dump\n");

	fwrite(data_, sizeof(char), strlen(data_), code_text); // length_ -> strlen(data_);
}

Code::~Code()
{
	assert(this && "You passed nullptr to Code destruct\n");

	free(data_);
	data_ = nullptr;

	length_ 	= -1;
	terms_	 	= -1;
	cur_size_ 	= -1;
}

int Code::get_terms()
{
	assert(this && "You passed nullptr to get_terms\n");
	return terms_;
}

void Code::set_cur_size(size_t number)
{
	assert(this && "You passed nullptr to set_cur_size\n");
	cur_size_ = number;
}

int Code::get_cur_size()
{
	assert(this && "You passed nullptr to get_cur_size\n");
	return cur_size_;
}

char Code::get_char(size_t number)
{
	return data_[number];
}

/* return bytecode of register ("rax" ==> CMD_RAX) */
static my_commands::Commands getReg(const char *reg)
{
	using namespace my_commands;

	if (!strcmp(reg, "rax"))
		return CMD_RAX;
	else if (!strcmp(reg, "rbx"))
		return CMD_RBX;
	else if (!strcmp(reg, "rcx"))
		return CMD_RCX;
	else if (!strcmp(reg, "rdx"))
		return CMD_RDX;
	else 
		return CMD_ERROR;
}

static my_commands::Commands getCommand(const char *cmd)
{
	using namespace my_commands;

	if (!strcmp(cmd, "add"))
		return CMD_ADD;
	else if (!strcmp(cmd, "mul"))
		return CMD_MUL;
	else if (!strcmp(cmd, "div"))
		return CMD_DIV;
	else if (!strcmp(cmd, "sub"))
		return CMD_SUB;
	else if (!strcmp(cmd, "addi"))
		return CMD_ADDI;
	else if (!strcmp(cmd, "muli"))
		return CMD_MULI;
	else if (!strcmp(cmd, "divi"))
		return CMD_DIVI;
	else if (!strcmp(cmd, "subi"))
		return CMD_SUBI;
	else if (!strcmp(cmd, "sin"))
		return CMD_SIN;
	else if (!strcmp(cmd, "cos"))
		return CMD_COS;
	else if (!strcmp(cmd, "pow"))
		return CMD_POW;
	else if (!strcmp(cmd, "sqrt"))
		return CMD_SQRT;
	else if (!strcmp(cmd, "in"))
		return CMD_IN;
	else if (!strcmp(cmd, "out"))
		return CMD_OUT;
	else if (!strcmp(cmd, "del"))
		return CMD_DEL;
	else if (!strcmp(cmd, "abs"))
		return CMD_ABS;
	else if (!strcmp(cmd, "ln"))
		return CMD_LN;
	else if (!strcmp(cmd, "hlt"))
		return CMD_HLT;
	else return CMD_ERROR;
}

Bytecode::Bytecode(Code *code_class, bool need_debug) :
    error_state_(0),
    bytecode_capacity_(0),
	unknown_command_(0)
{
	assert(code_class && "You passed nullptr(code_class) to Bytecode construct\n");
	assert(this && "You passed nullptr(this) to Bytecode \n");

	using namespace my_commands;

	Label *labels = (Label *)calloc(MAX_LABELS + 1, sizeof(Label));
	assert(labels);

	int amount_labels = get_labels(labels, code_class);

	data_ = (double *) calloc(code_class->get_terms(), sizeof(double));
	assert(data_ && "Can't calloc memory for data_\n");

	int flags_size = 0;
	code_class->set_cur_size(0); // cur_size = 0;

	char *temp = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
	assert(temp && "Can't calloc memory for tmp varaible");

	FILE *assembler_txt = fopen("xxx", "w");
	assert(assembler_txt && "Can't open file assembler_code.txt");

    print_assem_id(assembler_txt)

	for (int i = 0; i < code_class->get_terms(); i++, bytecode_capacity_++)
	{
		if (error_state_)
		{
            fprintf(stderr, "Find error state.\n");
			break;
        }

		get_lexeme(code_class, temp);

		if (!strcmp(temp, "add") || !strcmp(temp, "mul") || !strcmp(temp, "sub") || !strcmp(temp, "div")) /* commands with 3 regs args*/
		{
			Commands cmdType = getCommand(temp);

			get_lexeme(code_class, temp);
			int destReg = static_cast<int>(getReg(temp));

			get_lexeme(code_class, temp);
			int src1Reg = static_cast<int>(getReg(temp));

			get_lexeme(code_class, temp);
			int src2Reg = static_cast<int>(getReg(temp));

			fprintf(assembler_txt, "%d %d %d %d ", cmdType, destReg, src1Reg, src2Reg);

			i += 3;
		}
		else if (!strcmp(temp, "addi") || !strcmp(temp, "muli") || !strcmp(temp, "subi") || !strcmp(temp, "divi")) /* commands with 3 regs args*/
		{
			Commands cmdType = getCommand(temp);

			get_lexeme(code_class, temp);
			int destReg = static_cast<int>(getReg(temp));

			get_lexeme(code_class, temp);
			int src1Reg = static_cast<int>(getReg(temp));

			get_lexeme(code_class, temp);
			double number = static_cast<double>(std::atof(temp));

			fprintf(assembler_txt, "%d %d %d %lg ", cmdType, destReg, src1Reg, number);

			i += 3;
		}
		cmd_check(sin, CMD_SIN);
		cmd_check(cos, CMD_COS);
		cmd_check(pow, CMD_POW);
		cmd_check(sqrt, CMD_SQRT);
		cmd_check(in, CMD_IN);
		cmd_check(out, CMD_OUT);
		cmd_check(del, CMD_DEL);
		cmd_check(abs, CMD_ABS);
		cmd_check(ln, CMD_LN);
		cmd_check(log10, CMD_LOG10);
		cmd_check(log2, CMD_LOG2);
		cmd_check(ret, CMD_RET);
		cmd_check(hlt, CMD_HLT);


		transition_check(je, CMD_JE)
		transition_check(jab, CMD_JAB)
		transition_check(jae, CMD_JAE)
		transition_check(jbe, CMD_JBE)
		transition_check(ja, CMD_JA)
		transition_check(jb, CMD_JB)
		transition_check(call, CMD_CALL)
		transition_check(jmp, CMD_JMP)
		else if (temp[strlen(temp) - 1] == ':')
			data_[i] = CMD_LABEL;
		else if (IS_LAST_COMMAND_JMP)
		{
			temp++;

			bool FIND_LABEL = false;
			for (int index = 0; index < amount_labels; index++)
				if (!strcmp(temp, labels[index].name))
				{
					data_[i] = labels[index].adress;
					FIND_LABEL = true;
					break;
				}
			if (!FIND_LABEL)
			{
				set_error_state(get_error_state() + ERROR_FIND_LABEL);
				break;
			}
			temp--;
			IS_LAST_COMMAND_JMP = false;
		}
		else
		{
			set_error_state(get_error_state() + error_process(i, temp));
			set_unknown_command(temp);
			free(temp);
			temp = nullptr;
			break;
		}
	}

	if (get_error_state())
	{
		free(labels);
        free(temp);

        FILE *assembler_txt = fopen("xxx", "w");
        assert(assembler_txt && "Can't open file assembler_code.txt");

        fprintf(assembler_txt, "666");

		determine_status();
		fclose(assembler_txt);
		return;

	}

	determine_status();

	free(labels);
	free(temp);

	fclose(assembler_txt);
}

void Bytecode::set_error_state(int number)
{
	assert(this && "You passed nullptr to set_error_state");
	error_state_ = number;
}

int Bytecode::get_error_state()
{
	assert(this && "You passed nullptr to get_error_state");
	return error_state_;

}

char* Bytecode::get_unknown_command()
{
	assert(this && "You passed nullptr to get_unknown_command\n");
	return unknown_command_;
}

void Bytecode::set_unknown_command(char* command)
{
	assert(this && "You passed nullptr(this) to set_unknown command");
	assert(command && "You passed nullptr command\n");

	if (unknown_command_ == nullptr)
	{
		unknown_command_ = (char*) calloc(MAX_SIZE_COMMAND, sizeof(char));
		assert(unknown_command_ && "Can't calloc memory for unknown_command_\n");
	}

	for(int i = 0; i < strlen(command); i++)
		unknown_command_[i] = command[i];
}

void Bytecode::determine_status()
{
	using namespace my_commands;

	FILE *error = fopen("[!]DUMP_ASSEM.txt", "w");
	assert(error);

	fprintf(error, "\tData of DUMP in Assembler : %s (dd/mm/yy)\n\n", define_date());

	if (!error_state_)
		print_good(error);
	else
		for (int i = 0; i < NUMBER_OF_ERRORS; i++)
			if (get_byte(error_state_, i + 1))
				print_error(error, i + 1);

	fclose(error);
}

void Bytecode::print_good(FILE* error)
{
	assert(error && "file is nullptr\n");
	fprintf(error, "Assem ended without any errors\n");
}

void Bytecode::print_error(FILE* error, int error_bit)
{
	using namespace my_commands;
	switch (error_bit)
	{
	case ID_UNKNOWN_COM:
		fprintf(error, "Assembler met unknown command #%s#\n", unknown_command_);
		break;
	case ID_FIND_LABEL:
		fprintf(error, "Cann't find label\n");
		break;
	case ID_PROC_BRACKET:
		fprintf(error, "Error in processing of brackets\n");
		break;
	case ID_UNK_BRACKET:
		fprintf(error, "Unknown type of bracket\n");
		break;
	default:
		fprintf(error, "Error in ERRORS. IT means that program has unknown error\n");
		break;
	}
}

Bytecode::~Bytecode()
{
	assert(this && "You passed nullptr to destruct\n");

	free(data_);
	//data_ = nullptr;

	free(unknown_command_);
	//unknown_command_ = nullptr;

	//bytecode_capacity_ = -1;
	//error_state_ = -1;
}

void get_lexeme(Code *code_class, char *temp)
{
	int j = 0;
	for (j = 0; j < MAX_SIZE_COMMAND; j++)
	{
		if (code_class->get_char(j + code_class->get_cur_size()) != ' ')
			temp[j] = code_class->get_char(j + code_class->get_cur_size());
		else
		{
			temp[j] = '\0';
			break;
		}
	}

	code_class->set_cur_size(code_class->get_cur_size() + j + 1); //code_class->cur_size += j + 1;
}

auto get_labels(Label *labels, Code *code_class) -> number_of_labels
{
	/*for (int y = 0; y < MAX_LABELS; y++)
	{
		labels[y].name = (char *)calloc(MAX_LABEL_SIZE, sizeof(char));
		assert(labels[y].name);
	}*/

	char *temp = (char *)calloc(MAX_SIZE_COMMAND, sizeof(char));
	assert(temp);

	int cur_labels = 0;
	code_class->set_cur_size(0);

	for (int i = 0; i < code_class->get_terms(); i++)
	{
		get_lexeme(code_class, temp);

		if (temp[strlen(temp) - 1] == ':')
		{
			strncpy(labels[cur_labels].name, temp, strlen(temp) - 1);

			labels[cur_labels].length = strlen(temp);

			labels[cur_labels++].adress = i;
		}

	}

	code_class->set_cur_size(0);
	free(temp);

	return cur_labels; //int amount_labels = cur_labels;
}

void Bytecode::set_int_to_data(size_t number, int digit)
{
	assert(this && "You passed nullptr to sen_int_to_data\n");
	data_[number] = digit;
}

auto error_process(int i, char *temp) -> int
{
	using namespace my_commands;

	/*FILE* error = fopen("[!]ERRORS.txt", "w");
	assert(error);

    fprintf(error, "\n\tData of error : %s (dd/mm/yy)\n\n", define_date());
    fprintf(error, "Assembler doesn't know some command..\n");
    fprintf(error, "bytecode[%d] = [%s]\n", i, temp);
    fclose(error); */

	printf("Unknown command error\n");

	return ERROR_UNKNOWN_COM;
}

auto set_time(struct tm *time) -> char *
{
	char string_time[size_time] = {};
	char *tmp = nullptr;

	strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

	tmp = (char *)malloc(sizeof(string_time));
	strcpy(tmp, string_time);

	return tmp;
}

auto define_date(void) -> char*
{
	const time_t timer = time(nullptr);
	struct tm *local_time = localtime(&timer);

	return set_time(local_time);
}

auto define_specs(int *SPEC_NUMBER, int *SPEC_REGIST, char spec) -> int
{
	using namespace my_commands;

	if (spec == '[')
	{
		*SPEC_NUMBER = OP_DOUBLE_NUMBER;
		*SPEC_REGIST = OP_DOUBLE_REGIST;
	}
	else if (spec == '(')
	{
		*SPEC_NUMBER = OP_CHAR_NUM;
		*SPEC_REGIST = OP_CHAR_REG;
	}
	else
	{
		printf("Something went wrong in define bracket\n");
		return ERROR_UNK_BRACKET;
	}
	return 0;
}

auto get_byte(int digit, int number_of_bit) -> bool
{
    return (bool((1 << (number_of_bit - 1))  &  digit));
}
