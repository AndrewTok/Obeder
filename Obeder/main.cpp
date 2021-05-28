#include "Obeder.h"
#include "data_proccesing.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream inp("input.txt");
	stringstream out;

	Obeder ob;

	map<time_t, Operation> oper_map = get_operation_map(inp);

	std::vector<Recomendation> recom = ob.get_recomendation(oper_map, 0, 35);

	for (auto& rec : recom)
	{
		out << rec.get_debtor_name() << " -> " << rec.get_creditor_name() << " " << rec.get_debt_sum() << "  kopecks " << std::endl;
	}

	cout << out.str() << endl;
	
	inp.close();

	return 0;
}