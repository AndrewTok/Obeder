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

	void fill_names_with_operations(int start, int end, int first_multiplier, int second_multiplier)
	{
		names.clear();
		for (int i = start; i <= end; i++)
		{
			std::string op_name = std::to_string(i);
			names.insert(names.end(), op_name);
			operations.insert(operations.end(), Operation(op_name, first_multiplier*i));
			operations.insert(operations.end(), Operation(op_name, second_multiplier * i));
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

	bool check_all_debts(int name_debt_multiplier)
	{
		for (auto& user_name : names)
		{
			int total_payed_sum = 0;
			for (auto& recom : output)
			{
				if (recom.get_creditor_name() == user_name)
				{
					total_payed_sum += recom.get_debt_sum();
				}
				if (recom.get_debtor_name() == user_name)
				{
					total_payed_sum -= recom.get_debt_sum();
				}
			}
			if (!(name_debt_multiplier * stol(user_name) == total_payed_sum)) 
			{
				return false;
			}
		}
		return true;
	}

	void init_test_lots_person(int begin, int end, int total_sum)
	{
		fill_names_with_operations(begin, end, 1, 0);
		Operation pay_operation = { std::to_string(total_sum), total_sum };
		operations.insert(operations.end(), pay_operation);
		names.insert(names.end(), std::to_string(total_sum));
		fill_input();
		get_output(0, operations.size());
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

TEST_F(TestObeder, lots_of_persons_pay_for_themselves)
{
	fill_names_with_operations(-100, 100, 1, -1);
	fill_input();
	get_output(0, operations.size());
	ASSERT_TRUE(output.empty());
}

TEST_F(TestObeder, lots_of_persons_correct_input)
{
	/* имя связано с долгом как  3*имя = суммарный долг */

	fill_names_with_operations(-100, 100, -1, 4);
	fill_input();
	get_output(0, operations.size());
	ASSERT_EQ(output.size(), 100);
	ASSERT_TRUE(check_all_debts(3));
}
TEST_F(TestObeder, lots_of_persons_incorrect_input)
{
	
	fill_names_with_operations(-100, 100, -1, 4);
	Operation added_mistake = {"0", 42};
	operations.insert(operations.end(), added_mistake);
	fill_input();
	get_output(0, operations.size());
	ASSERT_TRUE(output.empty());
}

TEST_F(TestObeder, lots_of_persons_one_creditor_correct)
{
	init_test_lots_person(-99, 0, 4950);
	ASSERT_EQ(output.size(), 99);
	ASSERT_TRUE(check_all_debts(1));
}

TEST_F(TestObeder, lots_of_persons_one_creditor_inccorrect)
{

	init_test_lots_person(-99, 0, 99999);
	ASSERT_EQ(output.size(),0);
}

TEST_F(TestObeder, lots_of_persons_one_debtor_correct)
{
	init_test_lots_person(0, 99, -4950);
	ASSERT_EQ(output.size(), 99);
	ASSERT_TRUE(check_all_debts(1));
}

TEST_F(TestObeder, lots_of_persons_one_debtor_inccorrect)
{

	init_test_lots_person(0, 99, -99999);
	ASSERT_EQ(output.size(), 0);
}
TEST_F(TestObeder, incorrect_input)
{
	std::vector<Operation> _operations = { {"user1", -100}, {"user3", -200}, {"user1", 300}, {"user4", -150}, {"user5", -100}, {"user1", -400}, {"user6", 7650} };
	operations = _operations;
	fill_input();
	get_output(0, operations.size());
	ASSERT_TRUE(output.empty());
}