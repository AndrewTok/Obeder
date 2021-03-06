#include "data_proccesing.h"


std::vector<std::string> split_string(const std::string& str, const std::string& split_symbols)
{
	std::string word;
	std::vector<std::string> words = {};
	size_t pos = 0;
	for (size_t num = 0;;) {
		pos = str.find_first_not_of(split_symbols, pos);
		if (pos == std::string::npos) { break; }
		size_t pos1 = str.find_first_of(split_symbols, pos);
		size_t len = (pos1 == std::string::npos) ? std::string::npos : pos1 - pos;
		word = str.substr(pos, len);
		words.insert(words.end(), word);
		if (pos1 == std::string::npos) { break; }
		pos = pos1;
	}
	return words;
}

input_note convert_line(const std::string& line)
{
	input_note note;
	std::vector<std::string> words = split_string(line, " \t");
	note.ts = convert_str_to_time(words.at(0));
	note.name = words.at(1);
	note.sum = stol(words.at(2));
	return note;
}

std::map<time_t, Operation> get_operation_map(std::istream& input)
{
	std::string line;
	//std::vector<std::string> words_in_line;
	std::map<time_t, Operation> oper_map;
	//Operation curr_oper;
	input_note curr_note;
	while (std::getline(input, line))
	{
		try
		{
			curr_note = convert_line(line);
		}
		catch (std::exception& excpt)
		{
			return std::map<time_t, Operation>();
		}
		Operation curr_oper = { curr_note.name, curr_note.sum };
		std::pair<time_t, Operation> curr_pair(curr_note.ts, curr_oper);
		oper_map.insert(curr_pair);
	}
	return oper_map;
}

time_t convert_str_to_time(const std::string& str)
{
	time_t ts;
	std::istringstream str_stream{ str };
	str_stream >> ts;
	return ts;
}
