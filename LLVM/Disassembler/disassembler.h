#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

#include <time.h>
#include <locale.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <string.h>

#include <assert.h>

#include "disassembler_info.h"
#include "../Commands.h"

#include "../StackLib/stack.h"

auto make_bytecode(FILE* text, Bytecode* byte_struct) -> void;

auto bytecode_destruct(Bytecode* byte_struct) -> void;

auto disassembler(Bytecode* byte_struct, FILE* result) -> void;

auto ignore_spaces(char** buffer) -> void;

auto set_time(struct tm *time) -> char*;

auto define_date(void) -> char*;

auto get_number(char** buffer) -> double;


#endif // DISASSEMBLER_H_INCLUDED
