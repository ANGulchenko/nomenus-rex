#ifndef PROGRESSATOR_H
#define PROGRESSATOR_H

#include <chrono>

class Progressator
{
public:
	Progressator(int total);
	void	setTotal(int total);
	void	inc(int delta = 1);
	void	print();

	static	void	test();

private:
	void count_decimal_digits_in_total();

	int	total;
	int decimal_digits_in_total;
	int current;

	uint64_t time_previous;
	uint64_t time_per_inc;

};
#endif
