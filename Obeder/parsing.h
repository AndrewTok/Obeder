#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <sstream>


#include "Obeder.h" // нормально спроектировать класс

std::vector<std::string> split_string(const std::string& str, const std::string split_symbols);

Note convert_line(const std::string& line);

time_t convert_str_to_time(const std::string& str);

