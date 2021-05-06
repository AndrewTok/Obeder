#include "Obeder.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream inp("input.txt");
	stringstream out;

	Obeder ob;

	ob.init(inp);

	ob.print_recomendation(0, 30, out);

	cout << out.str() << endl;
	
	inp.close();

	return 0;
}