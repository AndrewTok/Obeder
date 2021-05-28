#pragma once
#include<string>

struct Recomendation
{
	Recomendation(std::string d_name, std::string c_name, int debt_sum);
	std::string get_debtor_name() const;
	std::string get_creditor_name() const;
	int get_debt_sum() const;
private:
	std::string debtor_name; // user_id должника
	std::string creditor_name; // user_id кредитора
	int debt_sum; // сумма долга
};