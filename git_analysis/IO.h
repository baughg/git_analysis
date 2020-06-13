#pragma once
#include <cstdint>
#include <string>
#include <deque>

namespace GB {
	class IO
	{
	public:
		static bool read_text(
			const std::string &filename, 
			std::deque<std::string> &lines);

		static bool read_lines(
			const std::string &filename,
			uint32_t &line_count,
			uint32_t &char_count);
	};
}

