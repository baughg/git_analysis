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
			uint64_t &line_count,
			uint64_t &char_count);
	};
}

