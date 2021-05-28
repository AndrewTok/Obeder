#include <algorithm>

#include "Obeder.h"



std::vector<Recomendation> Obeder::get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	std::vector<Recomendation> recomendations = {};
	std::vector<Lunchmate> lunchmates = get_lunchmates(notes, begin, end);
	if (lunchmates.empty())
	{
		return std::vector<Recomendation>();
	}
	size_t debtor_index = 0, creditor_index = (lunchmates.size() - 1); // идем с двух концов отсортированного массива
	while (debtor_index < creditor_index)
	{
		Lunchmate& curr_debtor = lunchmates[debtor_index];
		Lunchmate& curr_creditor = lunchmates[creditor_index];
		if (curr_debtor.total_pay_sum > 0 || curr_creditor.total_pay_sum < 0)
		{
			return std::vector<Recomendation>();
		}
		Recomendation curr_recom = process_debtor_and_creditor(curr_debtor, debtor_index, curr_creditor, creditor_index);
		recomendations.insert(recomendations.end(), curr_recom);

	}
	return recomendations;
}



std::vector<Obeder::Lunchmate> Obeder::get_lunchmates(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	lunchmates_with_map debts = {};
	if (begin > end)
	{
		return std::vector<Lunchmate>();
	}
	for (auto& [time_st, operation] : notes)

	{
		if (time_st > end)
		{
			return std::vector<Lunchmate>();
		}
		if (time_st >= begin)
		{
			process_operation(debts, operation);
		}
	}
	std::vector<Lunchmate> debt_arr = debts.get_lunchmates();
	std::sort(debt_arr.begin(), debt_arr.end(), [](const Lunchmate& lhs, const Lunchmate& rhs) {
		return lhs.total_pay_sum < rhs.total_pay_sum; });
	return debt_arr;
}



void Obeder::process_operation(lunchmates_with_map& mapped_lunchmates, const Operation& operation)
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

void Obeder::lunchmates_with_map::insert(const Operation& operation)
{
	size_t index;
	Lunchmate mate = { operation.get_name(), operation.get_pay_sum() };
	index = lunchmates.size();
	lunchmates.insert(lunchmates.end(), mate);
	name_index_table[mate.name] = index;
}

void Obeder::lunchmates_with_map::update(const Operation& operation)
{
	size_t index = name_index_table[operation.get_name()];
	lunchmates[index].total_pay_sum += operation.get_pay_sum();
}

void Obeder::lunchmates_with_map::insert_or_update(const Operation& operation)
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

std::vector<Obeder::Lunchmate> Obeder::lunchmates_with_map::get_lunchmates() const
{
	return lunchmates;
}

