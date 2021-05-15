#include <algorithm>

#include "Obeder.h"



Recomendation Obeder::get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	Recomendation recom = {};
	std::vector<Lunchmate> debt_arr = get_debt_arr(notes, begin, end);
	Recomendation_cell curr_recom;
	size_t debtor_index = 0, creditor_index = (debt_arr.size() - 1); // идем с двух концов отсортированного массива
	Lunchmate* curr_debtor, * curr_creditor; // по указателю чтобы мен€ть сам объект
	if (debt_arr.empty())
	{
		return recom;
	}
	while (debtor_index < creditor_index)
	{
		curr_debtor = &debt_arr[debtor_index];
		curr_creditor = &debt_arr[creditor_index];
		if (curr_debtor->total_pay_sum > 0 || curr_creditor->total_pay_sum < 0)
		{
			return Recomendation();
		}
		if (abs(curr_debtor->total_pay_sum) > curr_creditor->total_pay_sum)
		{
			curr_recom = { curr_debtor->name, curr_creditor->name, curr_creditor->total_pay_sum };
			recom.insert(curr_recom);
			curr_debtor->total_pay_sum += curr_creditor->total_pay_sum;
			curr_creditor->total_pay_sum = 0;
			creditor_index--;
		}
		else
		{
			curr_recom = { curr_debtor->name, curr_creditor->name, abs(curr_debtor->total_pay_sum) };
			recom.insert(curr_recom);
			curr_creditor->total_pay_sum += curr_debtor->total_pay_sum;
			curr_debtor->total_pay_sum = 0;
			debtor_index++;
		}
	}
	return recom;
}



std::vector<Obeder::Lunchmate> Obeder::get_debt_arr(const std::map<time_t, Operation>& notes, time_t begin, time_t end)
{
	debt_struct debts = {};
	if (begin > end)
	{
		return std::vector<Lunchmate>();
	}
	for (auto& note : notes)
	{
		if (note.first > end)
		{
			return std::vector<Lunchmate>();
		}
		if (note.first >= begin)
		{
			process_operation(debts, note.second);
		}
	}
	std::vector<Lunchmate> debt_arr = debts.debt_arr;
	std::sort(debt_arr.begin(), debt_arr.end(), [](const Lunchmate& lhs, const Lunchmate& rhs) {
		return lhs.total_pay_sum < rhs.total_pay_sum; });
	return debt_arr;
}



void Obeder::process_operation(debt_struct& debts, const Operation& oprtn)
{
	// если запись об участнике уже есть, то обновл€ем его итоговую сумму, иначе добавл€ем запись
	if (debts.name_index_table.find(oprtn.name) != debts.name_index_table.end()) 
	{
		debts.update(oprtn);
	}
	else
	{
		debts.insert(oprtn);
	}
}

/*

далее идут старые реализации

*/

//bool Obeder::mates_sum_cmp(const Lunchmate &lhs, const Lunchmate &rhs)
//{
//	return lhs.sum > rhs.sum;
//}
//
//bool Obeder::notes_ts_cmp(const Note &lhs, const Note &rhs)
//{
//	return lhs.ts > rhs.ts;
//}

//bool Obeder::init(std::istream& input)
//{
//	debt_arr.clear();
//	std::string line;
//	std::vector<std::string> words_in_line;
//	Note curr_note;
//	while (std::getline(input, line))
//	{
//		try
//		{
//			curr_note = convert_line(line);
//		}
//		catch (...)
//		{
//			notes.clear();
//			return false;
//		}
//		notes.insert(notes.end(), curr_note);
//	}
//	std::sort(notes.begin(), notes.end(), [](const Note& lhs, const Note& rhs) {
//		return lhs.ts < rhs.ts; });
//	return true;
//}
//
//void Obeder::print_recomendation(time_t begin, time_t end, std::ostream& output)
//{
//	construct_debt_arr(begin, end);
//	if (debt_arr.empty())
//	{
//		output << "err" << std::endl;
//		return;
//	}
//	size_t debtor_index = 0, creditor_index = (debt_arr.size() - 1); // идем с двух концов отсортированного массива
//	Lunchmate* curr_debtor, *curr_creditor; // по указателю чтобы мен€ть сам объект
//	while (debtor_index < creditor_index)
//	{
//		curr_debtor = &debt_arr[debtor_index];
//		curr_creditor = &debt_arr[creditor_index];
//		if (curr_debtor->total_pay_sum > 0 || curr_creditor->total_pay_sum < 0)
//		{
//			output << "err" << std::endl;
//			return;
//		}
//		if (abs(curr_debtor->total_pay_sum) > curr_creditor->total_pay_sum)
//		{
//			output << curr_debtor->name << " -> " << curr_creditor->name << " " << curr_creditor->total_pay_sum << "  kopecks " << std::endl;
//			curr_debtor->total_pay_sum += curr_creditor->total_pay_sum;
//			curr_creditor->total_pay_sum = 0;
//			creditor_index--;
//		}
//		else
//		{
//			output << curr_debtor->name << " -> " << curr_creditor->name << " " << abs(curr_debtor->total_pay_sum) << "  kopecks " << std::endl;
//			curr_creditor->total_pay_sum += curr_debtor->total_pay_sum;
//			curr_debtor->total_pay_sum = 0;
//			debtor_index++;
//		}
//	}
//}

//Recomendation Obeder::get_recomendation(std::vector<Note> notes, time_t begin, time_t end)
//{
//
//
//	return Recomendation();
//}

//void Obeder::construct_debt_arr(time_t begin, time_t end)
//{
//	if (begin > end)
//	{
//		return;
//	}
//	for (auto& note : notes)
//	{
//		if (note.ts > end)
//		{
//			return;
//		}
//		if (note.ts >= begin)
//		{
//			process_note(note);
//		}
//	}
//	std::sort(debt_arr.begin(), debt_arr.end(), [](const Lunchmate& lhs, const Lunchmate& rhs) {
//		return lhs.total_pay_sum < rhs.total_pay_sum; });
//}

//bool Obeder::update_mate_sum_in_debt_arr(size_t mate_index, int new_sum)
//{
//	if (mate_index >= debt_arr.size())
//	{
//		return false;
//	}
//	debt_arr[mate_index].sum = new_sum;
//	return true;
//}
//
//void Obeder::process_note(Note& note)
//{
//	std::map<std::string, size_t> name_index_table;
//	size_t index;
//	Lunchmate mate;
//	if (name_index_table.find(note.name) != name_index_table.end())
//	{
//		index = name_index_table[note.name];
//		mate = debt_arr[index];
//		mate.sum += note.sum;
//	}
//	else
//	{
//		mate.name = note.name;
//		mate.sum = note.sum;
//		index = debt_arr.size();
//		debt_arr.insert(debt_arr.end(), mate);
//		name_index_table[mate.name] = index;
//	}
//}

//void Obeder::process_operation(std::vector<Lunchmate>& debt_arr, Operation& oprtn)
//{
//}