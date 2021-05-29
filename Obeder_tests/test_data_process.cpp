#include "pch.h"


#include "pch.h"
#include "data_proccesing.h"
#include <iostream>

class TestSplitStr : public ::testing::Test
{
protected:
	void SetUp()
	{
		input = "";
		output = {};
		//input_stream << input;
		test_output = {};
	}
	void TearDown()
	{
	}
	std::string input;
	std::stringstream input_stream;
	std::vector<double> output;
	std::vector<double> test_output;

};

class TestParser: public ::testing::Test
{
protected:
	void SetUp()
	{
	}
	void TearDown()
	{
	}

	bool try_convert_line()
	{
		bool success;
		try
		{
			note = convert_line(line);
			success = true;
		}
		catch (std::exception& exc)
		{
			success = false;
		}
		return success;
	}
	bool check_note(const input_note& expected_note)
	{
		return (note.name == expected_note.name) && (note.sum == expected_note.sum) && (note.ts == expected_note.ts);
	}
	void get_oper_map()
	{
		oper_map = get_operation_map(input);
	}
	void fill_correct_lines_and_notes()
	{
		std::string curr_line;
		input_note curr_note;
		for (int i = 0; i < 100; i++)
		{
			std::string index_string = std::to_string(i);
			curr_line = index_string + " " + index_string + " " + index_string;
			curr_note = { i, index_string, i };
			correct_lines.insert(correct_lines.end(), curr_line);
			test_notes.insert(test_notes.end(), curr_note);
		}
	}

	bool check_oper_maps()
	{
		if (test_oper_map.size() != oper_map.size())
		{
			return false;
		}
		for (auto& [time, test_operation] : test_oper_map)
		{
			if (oper_map.find(time) == oper_map.end())
			{
				return false;
			}
			Operation& curr_oper = oper_map.at(time);
			if (curr_oper.get_name() != test_operation.get_name())
			{
				return false;
			}
			if (curr_oper.get_pay_sum() != test_operation.get_pay_sum())
			{
				return false;
			}
		}
		return true;
	}
	input_note note;
	std::string line;
	std::stringstream input;
	std::map<time_t, Operation> oper_map;
	std::map<time_t, Operation> test_oper_map;
	std::vector<input_note> test_notes;
	std::vector<std::string> correct_lines;

};

TEST_F(TestParser, empty_input) {
	line.clear();
	bool success = try_convert_line();
	ASSERT_FALSE(success);
}

TEST_F(TestParser, incorrect_input) {
	std::vector<std::string> incorrect_lines = { "incorrect", "150namesum", "150 name sum", "            \t      " };
	bool success;
	for (auto& curr_line : incorrect_lines)
	{
		line = curr_line;
		success = try_convert_line();
		ASSERT_FALSE(success);
	}
}

TEST_F(TestParser, correct_input) {
	fill_correct_lines_and_notes();
	bool success;
	for (int i = 0; i < 100; i++)
	{
		line = correct_lines[i];
		success = try_convert_line();
		ASSERT_TRUE(success);
		ASSERT_TRUE(check_note(test_notes[i]));
	}
}

TEST_F(TestParser, test_get_operation_map)
{
	fill_correct_lines_and_notes();
	std::string str_input;
	
	ASSERT_EQ(correct_lines.size(), test_notes.size());
	for (size_t i = 0; i < correct_lines.size(); i++)
	{
		str_input += correct_lines[i] + "\n";
		input_note curr_note = test_notes[i];
		std::pair<time_t, Operation> curr_pair = { curr_note.ts, {curr_note.name, curr_note.sum} };
		test_oper_map.insert(curr_pair);
	}
	input << str_input;
	get_oper_map();
	ASSERT_TRUE(check_oper_maps());

}

TEST_F(TestSplitStr, split_empty_line) {
	input.clear();
	std::vector<std::string> words = split_string(input, " \t\n");
	ASSERT_TRUE(words.empty());
}

TEST_F(TestSplitStr, split_only_spaces) {
	input = "       \n         \t		";
	std::vector<std::string> words = split_string(input, " \t\n");
	ASSERT_TRUE(words.empty());
}

TEST_F(TestSplitStr, split_one_word) {
	input = "  word  \n ";
	std::vector<std::string> words = split_string(input, " \t\n");
	ASSERT_EQ(words.size(), 1);
	ASSERT_EQ(input.substr(2, 4), words[0]);
}

TEST_F(TestSplitStr, split_lots_of_words) {
	input_stream.clear();
	for (size_t i = 0; i < 100; i++) {
		input_stream << i << " ";
	}
	input = input_stream.str();
	std::vector<std::string> words = split_string(input, " \t\n");
	ASSERT_EQ(words.size(), 100);
	for (size_t i = 0; i < 100; i++) {
		ASSERT_EQ(stol(words[i]), i);
	}
}