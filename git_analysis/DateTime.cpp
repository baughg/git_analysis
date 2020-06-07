#include "DateTime.h"
#include "StringUtility.h"
#include <map>

using namespace GB;

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

	if (day_ == Day::none) {
		day_lut.erase(date_time_comp[0]);
		return false;
	}

	month_ = month_lut[date_time_comp[1]];

	if(month_ == Month::none) {
		month_lut.erase(date_time_comp[1]);
		return false;
	}

	return true;
}
