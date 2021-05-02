#pragma once
#include <map>
#include <string>
#include <time.h>
#include <vector>
#include <iostream>

#include "parsing.h"

struct Note
{
	time_t ts; // timestamp of the operation
	std::string name; // person's id
	int sum; // the sum of the operation
};

struct Lunchmate
{
	std::string name; // person's id
	int sum; // how much money mate has to receive or pay (negative) 
};

class Obeder final
{
public:
	Obeder() = default;
	~Obeder() = default;

	bool init(const std::istream& input); // to fullfill notes
	void print_recomendation(time_t begin, time_t end, std::ostream& output);
	

private:
	std::vector<Note> notes; // all notes from file
	std::vector<Lunchmate> debt_arr; // arr of debt for certain range sorted by sum

	void construct_debt_arr(time_t begin, time_t end);

	void update_mate_sum_in_debt_arr(size_t mate_index, int new_sum); // when debt_arr is constructing sometimes we need to update mate's debt

	void insert_note(Note note);

	std::string get_recomendation();

	int diff_mates_sum(Lunchmate& mate1, Lunchmate& mate2); // compares two sums and returns abs(difference)


};

