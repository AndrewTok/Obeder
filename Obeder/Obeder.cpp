#include <algorithm>

#include "Obeder.h"

bool Obeder::init(std::istream& input)
{
	debt_arr.clear();
	std::string line;
	std::vector<std::string> words_in_line;
	Note curr_note;
	while (std::getline(input, line))
	{
		try
		{
			curr_note = convert_line(line);
		}
		catch (...)
		{
			notes.clear();
			return false;
		}
		notes.insert(notes.end(), curr_note);
	}
	std::sort(notes.begin(), notes.end(), notes_ts_cmp);
	return true;
}

void Obeder::print_recomendation(time_t begin, time_t end, std::ostream& output)
{
	construct_debt_arr(begin, end);
	if (debt_arr.empty())
	{
		output << "err" << std::endl;
		return;
	}
	size_t debtor_index = 0, creditor_index = (debt_arr.size() - 1); // идем с двух концов отсортированного массива
	Lunchmate* curr_debtor, *curr_creditor; // по указателю чтобы менять сам объект
	while (debtor_index < creditor_index)
	{
		curr_debtor = &debt_arr[debtor_index];
		curr_creditor = &debt_arr[creditor_index];
		if (curr_debtor->sum > 0 || curr_creditor->sum < 0)
		{
			output << "err" << std::endl;
			return;
		}
		if (abs(curr_debtor->sum) > curr_creditor->sum)
		{
			output << curr_debtor->name << " -> " << curr_creditor->name << curr_creditor->sum << " копеек " << std::endl;
			curr_debtor->sum += curr_creditor->sum;
			curr_creditor->sum = 0;
			creditor_index++;
		}
		else
		{
			output << curr_debtor->name << " -> " << curr_creditor->name << abs(curr_debtor->sum) << " копеек " << std::endl;
			curr_creditor->sum += curr_debtor->sum;
			curr_debtor->sum = 0;
			debtor_index++;
		}
	}
}

void Obeder::construct_debt_arr(time_t begin, time_t end)
{
	if (begin > end)
	{
		return;
	}
	for (auto& note : notes)
	{
		if (note.ts > end)
		{
			return;
		}
		if (note.ts >= begin)
		{
			process_note(note);
		}
	}
	std::sort(debt_arr.begin(), debt_arr.end(), mates_sum_cmp);
}

bool Obeder::update_mate_sum_in_debt_arr(size_t mate_index, int new_sum)
{
	if (mate_index >= debt_arr.size())
	{
		return false;
	}
	debt_arr[mate_index].sum = new_sum;
	return true;
}

void Obeder::process_note(Note note)
{
	std::map<std::string&, size_t> name_index_table;
	size_t index;
	Lunchmate mate;
	if (name_index_table.find(note.name) != name_index_table.end())
	{
		index = name_index_table[note.name];
		mate = debt_arr[index];
		mate.sum += note.sum;
	}
	else
	{
		mate.name = note.name;
		mate.sum = note.sum;
		index = debt_arr.size();
		debt_arr.insert(debt_arr.end(), mate);
		name_index_table[mate.name] = index;
	}
}

bool Obeder::mates_sum_cmp(Lunchmate& greater, Lunchmate less)
{
	if (greater.sum > less.sum)
	{
		return true;
	}
	return false;
}

bool Obeder::notes_ts_cmp(Note& greater, Note& less)
{
	if (greater.ts > less.ts)
	{
		return true;
	}
	return false;
}
