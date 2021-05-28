#pragma once
#include <string>
struct Operation
{
	Operation(std::string name, int pay_sum);
	std::string get_name() const;
	int get_pay_sum() const;
private:
	std::string name; // person's id
	int pay_sum; // the sum of the operation
};