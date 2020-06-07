#pragma once
#include <string>
#include <map>
#include "Calendar.h"

namespace GB {
	class DateTime
	{
		using Day = Calendar::Day;
		using Month = Calendar::Month;
	public:
		bool date_time_from_string(const std::string &str);			
	private:
		Day day_{};
		Calendar::Month month_{};
		uint32_t day_of_month_{};
		uint32_t week_of_year_{};
		uint32_t year_{};
		static std::map<uint32_t, Calendar> calender_;
	};
}

