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
	private:
		uint32_t year_{};
		void generate();
	};

	Calendar::Month& operator++(Calendar::Month& e)
	{
		if (e == Calendar::Month::december) {
			throw std::out_of_range("for Month& operator ++ (Month&)");
		}
		e = Calendar::Month(static_cast<std::underlying_type<Calendar::Month>::type>(e) + 1);
		return e;
	}
}

