#pragma once
#include <map>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>


/*
	������� ������ - ��� ���� �������, ������� �������� ���������� �� ���������: ����� ��������, ��� ���� ��� ��������� ��������, ����� ��������.
�� ���� ����� ���� �� ����� �������� ����� ��������� ��� ����, ��� � ��� ������: ���� ������� ������ ���� (����� ����� < 0),
���� ������� ������ �������� (����� ����� > 0), ���� ������� � ���� � �������� - ����� ���������� ��� ������, ���� ������������ ����, ��� �� ����,
� ������ ����, ��� ��������.

	���� ��� ���������� � ������, �� ��������� ��� ���� ������� ������ �� ����� � ������ ������ ���� ������ �� ����������, �.�.
����� ��������� ��������, ��� �� �����-�� ������ ������� ��� �������� ������ �� ��� (��������, 2 ����� � 2 �������� �� ������� �������������),
�� ���� ������ �������. ���� ������� �����������, ��� ������������, �� ������ �� ������� � �� �������, �� �� ���� ������ ����� �������� ������ �� ����������.

	�� ���� ������ �������� ������ ��������� � ���������� (Lunchmate'��): ���������� �� ������� ������ ��� ������� Lunchmate'� ������������ 
��� ����� ��������, � ������� �� ����������, � ���������� �������� �����. ���� �������� ����� < 0 �� �� �������, ����� ��������.
����� ���������� �� ����� ������� �������� �������� ��������� Recomendation � ������� ����������� ���, ���� � ������� ������ ������. 
�������� ������ ���� ����������, ������ ��� �������� ����������� ��������� �� ��������� � ��������. 

	Recomendation �������� ��������� �������: ������ Lunchmate'�� ������������ �� ����������� �������� �����. ����� �� ����������� � ���� ������
� ������� � ������ ����� ������ ��������� � �������, � ��� �������� ������������ � Recomendation. 
����� ���� ������ �������� �������, ���������� ������� � ���������� �� ������� �������� (��� �������������� �������). 
���������� � ����������. ����� ������� ���������, ������� ��������. ���� �� ����� - �� ����� ���������� �������� ����� Lunchmate'�� � ���������� ������
(��� �������� ��� ��� ���������), �� ��� ������, ��� �� ���� ��������� ������������ ������ (��������, � ���� �� ������ ����� ������ �� ��� 
�� ��������� �� ������ � ������, ������� ��������� �� ����).

	� �������� ����� ���������� ��������� �����, ������� ��� ����������� (��� ������������� ���������� ������������ ���� ����� ��������, 
� ������� �� ���������), �.�. ��� ����� ���������� �� ������ ���������� (�� �� ����������� ������ ��� ��������� ������ ��). 
����� ������� ��� ��������� ���������� ���� ���������, � ��� �������� ������ ����. 
������ � ���� ��������� ������� ������ (�������� ��������� ������ ������� �������) ���������� �������� (������ ������) ����� ���������� (����������, ���� 
�� ����� ���������� Recomendation ������������ ������ Lunchmate'�� ���������������). �.�. ����������� ����������� ���������� �������� ��� ��������� ������
� ������������������. (� ��������� � ����������, ����� ���� ��������������� �������� �� ������ ����).

*/


struct Operation
{
	std::string name; // person's id
	int pay_sum; // the sum of the operation
};

struct Recomendation_cell
{
	std::string debtor_name; // user_id ��������
	std::string creditor_name; // user_id ���������
	int debt_sum; // ����� �����
};

struct Recomendation
{
	std::vector<Recomendation_cell> recom_arr; // ������ ���������� ���������� � ���, ���, ���� � ������� ������
	void insert(Recomendation_cell recom)
	{
		recom_arr.insert(recom_arr.end(), recom);
	}
};

class Obeder final
{
public:

	Obeder() = default;
	~Obeder() = default;

	Recomendation get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end);

	/* 
	����� ���� ������ ���������
	*/

	//bool init(std::istream& input); // to fullfill notes. not const because of getline using
	//void print_recomendation(time_t begin, time_t end, std::ostream& output);

protected:

	struct Lunchmate // ��������������� ��������� ��� ������� ��������� � ����������
	{
		std::string name; // person's id
		//int sum; // how much money mate has to receive or pay (negative) 
		int total_pay_sum; // how much money mate has to receive or pay (negative) 
	};

private:
	struct debt_struct // ��������������� ������������� ���������. ����� ��� ��������� ������ ��������� � ����������. 
	{
		std::vector<Lunchmate> debt_arr; // ������ ��������� � ����������
		std::unordered_map<std::string, size_t> name_index_table; 
		// ����� �������� ������ Lunchmate'a � ������� debt_arr �� �����. �����, ����� �������� ������� � debt_arr
		void insert(const Operation& oprtn)
		{
			size_t index;
			Lunchmate mate;
			mate.name = oprtn.name;
			mate.total_pay_sum = oprtn.pay_sum;
			index = debt_arr.size();
			debt_arr.insert(debt_arr.end(), mate);
			name_index_table[mate.name] = index;
		}

		void update(const Operation& oprtn)
		{
			size_t index = name_index_table[oprtn.name];
			debt_arr[index].total_pay_sum += oprtn.pay_sum;
		}
	};

	std::vector<Lunchmate> get_debt_arr(const std::map<time_t, Operation>& notes, time_t begin, time_t end); // �������� ������ ��������� � ����������

	void process_operation(debt_struct& debts, const Operation& oprtn); // ���������� ��������� �������� �� ������� ������ � ��������������� debt_arr (debts)

	/*
	����� ���� ������ ���������
	*/


	//std::vector<Note> notes; // all notes from file
	//std::vector<Lunchmate> debt_arr; // arr of debt for certain range sorted by sum

	//void construct_debt_arr(time_t begin, time_t end);

	//bool update_mate_sum_in_debt_arr(size_t mate_index, int new_sum); // when debt_arr is constructing sometimes we need to update mate's debt

	//void process_note(Note& note); // use when debt_arr is constructing

/*	void process_operation(std::vector<Lunchmate>& debt_arr, Operation& oprtn);*/ // ���������� ��������� �������� �� ������� ������ � ��������������� debt_arr 

	//int diff_mates_sum(Lunchmate& mate1, Lunchmate& mate2); // compares two sums and returns abs(difference)

	//bool mates_sum_cmp(const Lunchmate& greater, const Lunchmate& less);

	//bool notes_ts_cmp(const Note& greater, const Note& less);

};

