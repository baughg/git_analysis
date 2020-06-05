#pragma once
#include <string>
#include <deque>

namespace GB {
	class StringUtility
	{
	public:
		static void split_string(
			const std::string &str,
			std::deque<std::string>& tokenVector,
			const char token);
	};
}

