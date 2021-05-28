#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <map>

#include "Obeder.h"

struct file_line
{
	time_t ts; // timestamp of the operation
	std::string name; // person's id
	int sum; // the sum of the operation
};

std::vector<std::string> split_string(const std::string& str, const std::string& split_symbols);

file_line convert_line(const std::string& line);

std::map<time_t, Operation> get_operation_map(std::istream& input);

time_t convert_str_to_time(const std::string& str);

