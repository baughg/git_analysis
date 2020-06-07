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
	private:
		uint32_t year_{};
	};
}

