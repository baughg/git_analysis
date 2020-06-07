#pragma once
#include <cstdint>
#include <map>

namespace GB {
	class Calendar
	{
	public:
		enum class Day {
			none,
			sunday,
			monday,
			tuesday,
			wednesday,
			thursday,
			friday,
			saturday
		};

		enum class Month {
			none,
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
		
		Calendar() = default;		
		explicit Calendar(uint32_t year);
		uint32_t number_of_days(const Month &month);
		Calendar::Day day_of_month(const uint32_t &day, const Month &month);
		uint32_t get_week_of_year(const uint32_t &day, const Month &month);
	private:
		uint32_t year_{};
		void generate();
		std::map<uint32_t, uint32_t> week_lut_;
	};	
}

