#include "Operation.h"

Operation::Operation(const std::string& name, int pay_sum)
{
	this->name = name;
	this->pay_sum = pay_sum;
}

Operation::Operation(const Operation& oper)
{
	name = oper.get_name();
	pay_sum = oper.get_pay_sum();
}

const std::string& Operation::get_name() const
{
	return name;
}

int Operation::get_pay_sum() const
{
	return pay_sum;
}
