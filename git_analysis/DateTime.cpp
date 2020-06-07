#include "DateTime.h"
#include "StringUtility.h"
#include <map>
#include <cassert>

using namespace GB;
std::map<uint32_t, Calendar> DateTime::calender_{};

bool DateTime::date_time_from_string(const std::string &str)
{
	static std::map<std::string, Day> day_lut{
		{"Sun",Day::sunday},
		{"Mon",Day::monday},
		{"Tue",Day::tuesday},
		{"Wed",Day::wednesday},
		{"Thu",Day::thursday},
		{"Fri",Day::friday},
		{"Sat",Day::saturday },
	};

	static std::map<std::string, Month> month_lut{
		{"Jan",Month::january},
		{"Feb",Month::february},
		{"Mar",Month::march},
		{"Apr",Month::april},
		{"May",Month::may},
		{"Jun",Month::june},
		{"Jul",Month::july },
		{"Aug",Month::august },
		{"Sep",Month::september },
		{"Oct",Month::october },
		{"Nov",Month::november },
		{"Dec",Month::december }
	};

	std::deque<std::string> date_time_comp{};

	StringUtility::split_string(str, date_time_comp, ' ');

	if (date_time_comp.size() < 6) {
		return false;
	}

	day_ = day_lut[date_time_comp[0]];
	assert(day_ != Day::none);


	month_ = month_lut[date_time_comp[1]];
	assert(month_ != Month::none);


	const uint32_t year{ static_cast<uint32_t>(atoi(date_time_comp[4].c_str())) };

	assert(year);
	year_ = year;
	auto it{ calender_.find(year) };

	if (it == std::end(calender_)) {
		calender_[year] = Calendar{ year };
	}
	const uint32_t day{ static_cast<uint32_t>(atoi(date_time_comp[2].c_str())) };
	day_of_month_ = day;
	week_of_year_ = calender_[year].get_week_of_year(day_of_month_,month_);
	return true;
}
