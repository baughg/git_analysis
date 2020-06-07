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
	return true;
}
