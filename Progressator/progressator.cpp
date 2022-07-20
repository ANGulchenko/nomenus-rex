#include "progressator.h"

#include <iostream>
#include <iomanip>
#include <thread>


Progressator::Progressator(int _total)
	: total {_total}
	, decimal_digits_in_total {0}
	, current {0}
	, time_previous {0}
	, time_per_inc {0}
{
	count_decimal_digits_in_total();
}

void	Progressator::setTotal(int _total)
{
	total = _total;
	count_decimal_digits_in_total();
}

void	Progressator::count_decimal_digits_in_total()
{
	decimal_digits_in_total = 0;
	int num = total;
	while (num != 0)
	{
		decimal_digits_in_total++;
		num /= 10;
	}
}

void	Progressator::inc(int _delta)
{
	uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (time_previous == 0)
	{
		time_previous = ms;
	}
	time_per_inc = (ms - time_previous) / 2;
	time_previous = ms;

	current += _delta;
}

void	Progressator::print()
{
	int percent = 100 * current / total;
	std::string percent_str_tiles(percent / 5, '=');
	std::string percent_str_postfix(20 - percent_str_tiles.size(), ' ');
	std::string percent_str_back = percent_str_tiles + percent_str_postfix;
	std::string percentage = std::to_string(percent)+"%";
	percent_str_back.replace((percent_str_back.size() - percentage.size())/2, percentage.size(), percentage);
	std::string percent_str = "["+percent_str_back+"]";

	std::cout << '\r'
			  <<"[" << std::setfill('0') << std::setw(decimal_digits_in_total) << current << "/" << total << "] "
			  << percent_str
			  << " ETC:"<< std::setfill('0') << std::setw(6) << time_per_inc * (total - current) << " msec.";
	std::flush(std::cout);
}

/*void	Progressator::test()
{
	Progressator progressator(1000);
	for (int i = 0; i < 1000; i++)
	{
		progressator.inc();
		progressator.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	std::cout << std::endl;
}*/
