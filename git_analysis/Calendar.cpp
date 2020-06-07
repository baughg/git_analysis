#include "Calendar.h"
#include <cassert>

using namespace GB;

Calendar::Calendar(uint32_t year)
	: year_{year}
{
	generate();
}


Calendar::Day Calendar::day_of_month(const uint32_t &day, const Month &mon)
{
	const uint32_t month{ 
		static_cast<uint32_t>(mon) - static_cast<uint32_t>(Month::january) };

	static uint32_t t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
					   4, 6, 2, 4 };
	year_ -= month < 3;
	const uint32_t day_index{ (year_ + (year_ >> 2) - year_ / 100 +
		year_ / 400 + t[month - 1] + day) % 7 };

	return static_cast<Day>(static_cast<uint32_t>(Day::sunday) + day_index);
}

uint32_t Calendar::number_of_days(const Month &month)
{
	assert(month != Month::none);

	static std::map<Month, uint32_t> day_count_lut{
		{Month::january,31},
		{Month::february,28},
		{Month::march,31},
		{Month::april,30},
		{Month::may,31},
		{Month::june,30},
		{Month::july,31},
		{Month::august,31},
		{Month::september,30},
		{Month::october,31},
		{Month::november,30},
		{Month::december,31}
	};
	
	uint32_t days{ day_count_lut[month] };
	
	if (month == Month::february &&
		(year_ % 400 == 0 || (year_ % 4 == 0 && year_ % 100 != 0)))
	{
		days++;		
	}

	return days;
}

void Calendar::generate() {
	for (Month month{ Month::january }; month <= Month::december;
		month++) {

	}
}