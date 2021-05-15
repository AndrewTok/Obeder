#pragma once
#include <map>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>


/*
	Входные данные - это мапа записей, которые содержат информацию об операциях: время операции, имя того кто совершает операцию, сумма операции.
За один прием пищи об одном человеке может появиться как одна, так и две записи: либо человек только поел (тогда сумма < 0),
либо человек только заплатил (тогда сумма > 0), либо человек и поел и заплатил - тогда появляется две записи, одна соответсвует тому, что он поел,
а вторая тому, что заплатил.

	Если имя появляется в записи, то считается что этот человек знаком со всеми и сможет отдать долг любому из участников, т.к.
может сложиться ситуация, что за какой-то период времени два человека вместе не ели (например, 2 обеда и 2 человека по очереди отсутствовали),
но один должен другому. Если человек отсутсвовал, или присутсвовал, но ничего не заказал и не оплатил, то об этом визите этого человека записи не появляется.

	По этим данным строится массив должников и кредиторов (Lunchmate'ов): пробегаясь по входным данным для каждого Lunchmate'а складываются 
все суммы операций, в которых он участвовал, и получается итоговая сумма. Если итоговая сумма < 0 то он должник, иначе кредитор.
Далее пробегаясь по этому массиву строится конечная структура Recomendation в которой описывается кто, кому и сколько должен отдать. 
Должники отдают долг кредиторам, причем нет строгого закрепления кредитора за должником и наоборот. 

	Recomendation строится следующим образом: массив Lunchmate'ов отсортирован по возрастанию итоговой суммы. Далее он пробегается с двух концов
и должник с одного конца платит кредитору с другого, и эта операция записывается в Recomendation. 
Когда долг одного должника погашен, происходит переход к следующему по порядку должнику (без пересортировки массива). 
Аналогично с кредитором. Когда индексы совпадают, подсчет завершен. Если на каком - то этапе появляются итоговые суммы Lunchmate'ов с одинаковым знаком
(оба должники или оба кредиторы), то это значит, что на вход поступили некорректные данные (например, в один из обедов сумма долгов за еду 
не совпадает по модулю с суммой, которую заплатили за обед).

	В конечном итоге кредиторам поступает сумма, которую они переплатили (она автоматически получается складыванием всех сумма операций, 
в которых он участвовл), т.е. вся сумма заплченная за других участников (но не обязятельно именно эти участники платят им). 
Таким образом все кредиторы возвращают свою переплату, а все должники отдают долг. 
Причем в силу симметрии выплаты долгов (большему кредитору платит большой должник) количество операций (уплаты долгов) почти минимально (минимально, если 
на этапе построения Recomendation поддерживать массив Lunchmate'ов отсортированным). Т.о. значительно уменьшается количество операций без серьезных потерь
в производительности. (В сравнении с алгоритмом, когда долг пересчитывается отдельно за каждый обед).

*/


struct Operation
{
	std::string name; // person's id
	int pay_sum; // the sum of the operation
};

struct Recomendation_cell
{
	std::string debtor_name; // user_id должника
	std::string creditor_name; // user_id кредитора
	int debt_sum; // сумма долга
};

struct Recomendation
{
	std::vector<Recomendation_cell> recom_arr; // массив содержащий информацию о том, кто, кому и сколько должен
	void insert(Recomendation_cell recom)
	{
		recom_arr.insert(recom_arr.end(), recom);
	}
};

class Obeder final
{
public:

	Obeder() = default;
	~Obeder() = default;

	Recomendation get_recomendation(const std::map<time_t, Operation>& notes, time_t begin, time_t end);

	/* 
	далее идет старый интерфейс
	*/

	//bool init(std::istream& input); // to fullfill notes. not const because of getline using
	//void print_recomendation(time_t begin, time_t end, std::ostream& output);

protected:

	struct Lunchmate // вспомогательная структура для массива должников и кредиторов
	{
		std::string name; // person's id
		//int sum; // how much money mate has to receive or pay (negative) 
		int total_pay_sum; // how much money mate has to receive or pay (negative) 
	};

private:
	struct debt_struct // вспомогательная промежуточная структура. Через нее создается массив должников и кредиторов. 
	{
		std::vector<Lunchmate> debt_arr; // массив должников и кредиторов
		std::unordered_map<std::string, size_t> name_index_table; 
		// здесь хранится индекс Lunchmate'a в массиве debt_arr по имени. Нужен, чтобы ускорить вставку в debt_arr
		void insert(const Operation& oprtn)
		{
			size_t index;
			Lunchmate mate;
			mate.name = oprtn.name;
			mate.total_pay_sum = oprtn.pay_sum;
			index = debt_arr.size();
			debt_arr.insert(debt_arr.end(), mate);
			name_index_table[mate.name] = index;
		}

		void update(const Operation& oprtn)
		{
			size_t index = name_index_table[oprtn.name];
			debt_arr[index].total_pay_sum += oprtn.pay_sum;
		}
	};

	std::vector<Lunchmate> get_debt_arr(const std::map<time_t, Operation>& notes, time_t begin, time_t end); // получить массив должников и кредиторов

	void process_operation(debt_struct& debts, const Operation& oprtn); // обработать очередную операцию во входных данных и отредактировать debt_arr (debts)

	/*
	далее идет старый интерфейс
	*/


	//std::vector<Note> notes; // all notes from file
	//std::vector<Lunchmate> debt_arr; // arr of debt for certain range sorted by sum

	//void construct_debt_arr(time_t begin, time_t end);

	//bool update_mate_sum_in_debt_arr(size_t mate_index, int new_sum); // when debt_arr is constructing sometimes we need to update mate's debt

	//void process_note(Note& note); // use when debt_arr is constructing

/*	void process_operation(std::vector<Lunchmate>& debt_arr, Operation& oprtn);*/ // обработать очередную операцию во входных данных и отредактировать debt_arr 

	//int diff_mates_sum(Lunchmate& mate1, Lunchmate& mate2); // compares two sums and returns abs(difference)

	//bool mates_sum_cmp(const Lunchmate& greater, const Lunchmate& less);

	//bool notes_ts_cmp(const Note& greater, const Note& less);

};

