#pragma once
#include <map>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>

#include "Recomendation.h"
#include "Operation.h"


class Obeder final
{
public:

	static std::vector<Recomendation> get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end);


private:

	struct Lunchmate // ��������������� ��������� ��� ������� ��������� � ����������
	{

		static bool is_smaller(const Lunchmate& lhs, const Lunchmate& rhs);

		std::string name; // person's id
		//int sum; // how much money mate has to receive or pay (negative) 
		int total_pay_sum; // how much money mate has to receive or pay (negative) 
	};

private:
	struct lunchmates // ��������������� ������������� ���������. ����� ��� ��������� ������ ��������� � ����������. lunchmates lunchmates_with_map
	{
		void insert(const Operation& operation);

		void update(const Operation& operation);

		void insert_or_update(const Operation& operation);

		std::vector<Lunchmate> get_lunchmates() const;

		void sort(bool cmp(const Lunchmate& lhs, const Lunchmate& rhs));

		bool is_empty() const;

		size_t size() const;

		Lunchmate& operator[](size_t index);

	private:
		std::vector<Lunchmate> lunchmates_arr; // ������ ��������� � ���������� was debt_arr
		std::unordered_map<std::string, size_t> name_index_table;
		// ����� �������� ������ Lunchmate'a � ������� debt_arr �� �����. �����, ����� �������� ������� � debt_arr
	};

	static lunchmates get_lunchmates(const std::map<time_t, Operation>& parsed_file, time_t begin, time_t end); // �������� ������ ��������� � ���������� std::vector<Lunchmate>

	static void process_operation(lunchmates& mapped_lunchmates, const Operation& operation); // ���������� ��������� �������� �� ������� ������ � ��������������� debt_arr (debts)

	static Recomendation process_debtor_and_creditor(Lunchmate& debtor, size_t& debtor_index, Lunchmate& creditor, size_t& creditor_index);

};

