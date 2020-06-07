#include "Calendar.h"
#include <cassert>
#include <vector>

using namespace GB;

Calendar::Calendar(uint32_t year)
	: year_{year}
{
	generate();
}

Calendar::Day Calendar::day_of_month(const uint32_t &day, const Month &mon)
{
	const uint32_t month{ 
		static_cast<uint32_t>(mon) };

	uint32_t year{ year_ };
	static uint32_t t[] = { 0, 3, 2, 5, 0, 3, 5, 1,
					   4, 6, 2, 4 };
	year -= month < 3;
	const uint32_t day_index{ (year + (year >> 2) - year / 100 +
		year / 400 + t[month - 1] + day) % 7 };

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

Calendar::Month &operator++(Calendar::Month &e)
{
	if (e == Calendar::Month::december) {
		e = Calendar::Month::january;
		return e;
	}
	e = Calendar::Month(static_cast<std::underlying_type<Calendar::Month>::type>(e) + 1);
	return e;
}

uint32_t Calendar::get_week_of_year(
	const uint32_t &day, const Month &month) {
	const uint32_t key{ (static_cast<uint32_t>(month) << 16) | day };

	assert(week_lut_.find(key) != std::end(week_lut_));
	return week_lut_[key];
}

void Calendar::generate() {
	const static std::vector<Month> months{
		Month::january,
		Month::february,
		Month::march,
		Month::april,
		Month::may,
		Month::june,
		Month::july,
		Month::august,
		Month::september,
		Month::october,
		Month::november,
		Month::december
	};

	uint32_t week{ 0 };
	const Day first_day_of_year{ day_of_month(1,Month::january) };
	week += static_cast<uint32_t>(first_day_of_year != Day::sunday);	

	for (auto month : months) {
		const uint32_t days{ number_of_days(month) };
		const uint32_t month_key{ static_cast<uint32_t>(month) << 16 };

		for (uint32_t day{ 1 }; day <= days; ++day) {
			Day day_of_week{ day_of_month(day,month) };

			if (day_of_week == Day::sunday) {
				week++;
			}

			const uint32_t month_day_key{ month_key | day };
			week_lut_[month_day_key] = week;
		}
	}
}