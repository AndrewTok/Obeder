#pragma once
#include <map>
#include <string>
#include <ctime>
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

	bool init(std::istream& input); // to fullfill notes. not const because of getline using
	void print_recomendation(time_t begin, time_t end, std::ostream& output);
	

private:
	std::vector<Note> notes; // all notes from file
	std::vector<Lunchmate> debt_arr; // arr of debt for certain range sorted by sum

	void construct_debt_arr(time_t begin, time_t end);

	bool update_mate_sum_in_debt_arr(size_t mate_index, int new_sum); // when debt_arr is constructing sometimes we need to update mate's debt

	void process_note(Note note); // use when debt_arr is constructing

	std::string get_recomendation();

	//int diff_mates_sum(Lunchmate& mate1, Lunchmate& mate2); // compares two sums and returns abs(difference)

	bool mates_sum_cmp(Lunchmate& greater, Lunchmate less);

	bool notes_ts_cmp(Note& greater, Note& less);

};

