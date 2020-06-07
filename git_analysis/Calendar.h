#pragma once
#include <cstdint>

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

		Calendar(uint32_t year);
	private:
		uint32_t year_{};
	};
}

