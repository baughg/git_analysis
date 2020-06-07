#pragma once
#include <string>

namespace GB {
	class DateTime
	{
		enum class Day {
			sunday,
			monday,
			tuesday,
			wednesday,
			thursday,
			friday,
			saturday
		};

		enum class Month {
			january,
			february,
			march,
			april,
			may,
			june,
			july,
			august,
			september,
			october,
			november,
			december
		};
	public:
		bool date_time_from_string(const std::string &str);
	private:
		Day day_{};
		Month month_{};
		uint32_t day_of_month_{};
		uint32_t week_of_year_{};
		uint32_t year_{};
	};
}

