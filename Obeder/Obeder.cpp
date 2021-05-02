#include "Obeder.h"

bool Obeder::init(const std::istream& input)
{

	return false;
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

}

void Obeder::update_mate_sum_in_debt_arr(size_t mate_index, int new_sum)
{
}

void Obeder::insert_note(Note note)
{
}
