#include "Operation.h"

Operation::Operation(std::string name, int pay_sum)
{
	this->name = name;
	this->pay_sum = pay_sum;
}

std::string Operation::get_name() const
{
	return name;
}

int Operation::get_pay_sum() const
{
	return pay_sum;
}
