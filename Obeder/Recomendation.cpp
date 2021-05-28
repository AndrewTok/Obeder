#include "Recomendation.h"

Recomendation::Recomendation(std::string d_name, std::string c_name, int debt_sum)
{
	debtor_name = d_name;
	creditor_name = c_name;
	this->debt_sum = debt_sum;
}

std::string Recomendation::get_debtor_name() const
{
	return debtor_name;
}

std::string Recomendation::get_creditor_name() const
{
	return creditor_name;
}

int Recomendation::get_debt_sum() const
{
	return debt_sum;
}
