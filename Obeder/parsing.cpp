#include "parsing.h"


std::vector<std::string> split_string(const std::string& str, const std::string split_symbols)
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

Note convert_line(const std::string& line)
{
	Note note;
	std::vector<std::string> words = split_string(line, " \t");
	note.ts = convert_str_to_time(words[0]);
	note.name = words[1];
	note.sum = stol(words[2]);
	return note;
}

time_t convert_str_to_time(const std::string& str)
{
	time_t ts;
	std::stringstream str_stream;
	str_stream << str;
	str_stream >> ts;
	return ts;
}
