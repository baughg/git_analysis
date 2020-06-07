#include "Calendar.h"
#include <cassert>

using namespace GB;

Calendar::Calendar(uint32_t year)
	: year_{year}
{
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

