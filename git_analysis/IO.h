#pragma once
#include <string>
#include <deque>

namespace GB {
	class IO
	{
	public:
		static bool read_text(
			const std::string &filename, 
			std::deque<std::string> & lines);
	};
}

