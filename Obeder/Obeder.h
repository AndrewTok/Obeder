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

	struct Lunchmate // вспомогательная структура для массива должников и кредиторов
	{

		static bool is_smaller(const Lunchmate& lhs, const Lunchmate& rhs);

		std::string name; // person's id
		//int sum; // how much money mate has to receive or pay (negative) 
		int total_pay_sum; // how much money mate has to receive or pay (negative) 
	};

private:
	struct lunchmates // вспомогательная промежуточная структура. Через нее создается массив должников и кредиторов. lunchmates lunchmates_with_map
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
		std::vector<Lunchmate> lunchmates_arr; // массив должников и кредиторов was debt_arr
		std::unordered_map<std::string, size_t> name_index_table;
		// здесь хранится индекс Lunchmate'a в массиве debt_arr по имени. Нужен, чтобы ускорить вставку в debt_arr
	};

	static lunchmates get_lunchmates(const std::map<time_t, Operation>& parsed_file, time_t begin, time_t end); // получить массив должников и кредиторов std::vector<Lunchmate>

	static void process_operation(lunchmates& mapped_lunchmates, const Operation& operation); // обработать очередную операцию во входных данных и отредактировать debt_arr (debts)

	static Recomendation process_debtor_and_creditor(Lunchmate& debtor, size_t& debtor_index, Lunchmate& creditor, size_t& creditor_index);

};

