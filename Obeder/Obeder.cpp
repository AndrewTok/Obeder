#include <algorithm>

#include "Obeder.h"



std::vector<Recomendation> Obeder::get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	std::vector<Recomendation> recomendations = {};
	lunchmates lunchmates = get_lunchmates(notes, begin, end);
	if (lunchmates.is_empty())
	{
		return std::vector<Recomendation>();
	}
	size_t debtor_index = 0, creditor_index = (lunchmates.size() - 1); // идем с двух концов отсортированного массива
	while (debtor_index <= creditor_index)
	{
		Lunchmate& curr_debtor = lunchmates[debtor_index];
		Lunchmate& curr_creditor = lunchmates[creditor_index];
		if (curr_debtor.total_pay_sum > 0 || curr_creditor.total_pay_sum < 0)
		{
			return std::vector<Recomendation>();
		}
		Recomendation curr_recom = process_debtor_and_creditor(curr_debtor, debtor_index, curr_creditor, creditor_index);
		if (curr_recom.get_debt_sum() != 0)
		{
			recomendations.insert(recomendations.end(), curr_recom);
		}
	}
	return recomendations;
}



Obeder::lunchmates Obeder::get_lunchmates(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	lunchmates lmates;
	if (begin > end)
	{
		return lmates;
	}
	for (auto& [time_st, operation] : notes)

	{
		if (time_st > end)
		{
			break;
		}
		if (time_st >= begin)
		{
			process_operation(lmates, operation);
		}
	}
	//std::vector<Lunchmate> debt_arr = debts.get_lunchmates();
	//std::sort(debt_arr.begin(), debt_arr.end(), [](const Lunchmate& lhs, const Lunchmate& rhs) {
	//	return lhs.total_pay_sum < rhs.total_pay_sum; });
	lmates.sort(Lunchmate::is_smaller);
	return lmates;
}



void Obeder::process_operation(lunchmates& mapped_lunchmates, const Operation& operation)
{
	mapped_lunchmates.insert_or_update(operation);
}

Recomendation Obeder::process_debtor_and_creditor(Lunchmate& debtor, size_t& debtor_index, Lunchmate& creditor, size_t& creditor_index)
{
	int curr_pay_sum;
	if (abs(debtor.total_pay_sum) > creditor.total_pay_sum)
	{
		curr_pay_sum = creditor.total_pay_sum;
		creditor_index--;
	}
	else
	{
		curr_pay_sum = abs(debtor.total_pay_sum);
		debtor_index++;
	}
	Recomendation recom = { debtor.name, creditor.name, curr_pay_sum };
	debtor.total_pay_sum += curr_pay_sum;
	creditor.total_pay_sum -= curr_pay_sum;
	return recom;
}

void Obeder::lunchmates::insert(const Operation& operation)
{
	size_t index;
	Lunchmate mate = { operation.get_name(), operation.get_pay_sum() };
	index = lunchmates_arr.size();
	lunchmates_arr.insert(lunchmates_arr.end(), mate);
	name_index_table[mate.name] = index;
}

void Obeder::lunchmates::update(const Operation& operation)
{
	size_t index = name_index_table[operation.get_name()];
	lunchmates_arr[index].total_pay_sum += operation.get_pay_sum();
}

void Obeder::lunchmates::insert_or_update(const Operation& operation)
{
	if (name_index_table.find(operation.get_name()) != name_index_table.end())
	{
		update(operation);
	}
	else
	{
		insert(operation);
	}
}

std::vector<Obeder::Lunchmate> Obeder::lunchmates::get_lunchmates() const
{
	return lunchmates_arr;
}

void Obeder::lunchmates::sort(bool cmp(const Lunchmate& lhs, const Lunchmate& rhs))
{
	std::sort(lunchmates_arr.begin(), lunchmates_arr.end(), cmp);
}

bool Obeder::lunchmates::is_empty() const
{
	return lunchmates_arr.empty();
}

size_t Obeder::lunchmates::size() const
{
	return lunchmates_arr.size();
}

Obeder::Lunchmate& Obeder::lunchmates::operator[](size_t index)
{
	return lunchmates_arr.at(index);
}

bool Obeder::Lunchmate::is_smaller(const Lunchmate& lhs, const Lunchmate& rhs)
{
	return lhs.total_pay_sum < rhs.total_pay_sum;
}
