#pragma once
#include <string>
#include <cstdint>

namespace GB {
	class GitCommit
	{	
	private:
		std::string author_{};
		std::string email_{};
		std::string message_{};
		std::string hash_{};
		std::string date_str_{};
		uint32_t commit_number_{};
	};
}

