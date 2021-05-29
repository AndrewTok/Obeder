#include "pch.h"

#include <vector>
#include <map>
#include <string>
#include <ctime>


#include "Obeder.h"


class TestObeder : public ::testing::Test
{
protected:
	void SetUp()
	{

	}

	void TearDown()
	{

	}

	void get_output(time_t begin, time_t end)
	{
		output = Obeder::get_recomendation(input, begin, end);
	}

	void fill_names(size_t names_num)
	{
		names.clear();
		for (size_t i = 0; i < names_num; i++)
		{
			names.insert(names.end(), std::to_string(i));
		}
	}

	void fill_input()
	{
		for (time_t i = 0; i < operations.size(); i++)
		{
			std::pair<time_t, Operation> curr_pair = { i, operations[i] };
			input.insert(curr_pair);
		}
	}

	bool check_recomendation(const Recomendation& rec, const Recomendation& expected_recom)
	{
		return (rec.get_creditor_name() == expected_recom.get_creditor_name()) && (rec.get_debtor_name() == expected_recom.get_debtor_name()) && (rec.get_debt_sum() == expected_recom.get_debt_sum());
	}
	std::vector<std::string> names;
	std::vector<Operation> operations;
	std::vector<Recomendation> output;
	std::map<time_t, Operation> input;
};

TEST_F(TestObeder, empty_input)
{
	output = Obeder::get_recomendation(input, 0, 10);
	ASSERT_TRUE(output.empty());
}

TEST_F(TestObeder, one_person_correct_input)
{
	std::string name = "name";
	for (int i = 0; i < 100; i++)
	{
		std::pair<time_t, Operation> curr_note = { time_t(i), {name, i} };
		input.insert(curr_note);
	}

	for (int i = 0; i < 100; i++)
	{
		std::pair<time_t, Operation> curr_note = { time_t(i) + 100, {name, -i} };
		input.insert(curr_note);
	}
	get_output(0, 200);
	ASSERT_TRUE(output.empty());
}

TEST_F(TestObeder, one_person_incorrect_input)
{
	std::string name = "name";
	for (int i = 0; i < 100; i++)
	{
		std::pair<time_t, Operation> curr_note = { time_t(i), {name, i} };
		input.insert(curr_note);
	}

	get_output(0, 100);
	ASSERT_TRUE(output.empty());
}


TEST_F(TestObeder, two_persons_correct_input)
{
	std::vector<Operation> _operations = { {"user1", -100}, {"user2", -200}, {"user1", 300}, {"user1", -150}, {"user1", -100}, {"user1", -400}, {"user2", 650} };
	operations = _operations;
	Recomendation expected_recom = { "user1", "user2", 450 };
	fill_input();
	get_output(0, operations.size());
	ASSERT_EQ(output.size(), 1);
	ASSERT_TRUE(check_recomendation(output[0], expected_recom));
	get_output(0, 2);
	ASSERT_EQ(output.size(), 1);
	expected_recom = { "user2", "user1", 200 };
	ASSERT_TRUE(check_recomendation(output[0], expected_recom));
}

TEST_F(TestObeder, incorrect_input)
{
	std::vector<Operation> _operations = { {"user1", -100}, {"user3", -200}, {"user1", 300}, {"user4", -150}, {"user5", -100}, {"user1", -400}, {"user6", 7650} };
	operations = _operations;
	fill_input();
	get_output(0, operations.size());
	ASSERT_TRUE(output.empty());
}