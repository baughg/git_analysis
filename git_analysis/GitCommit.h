#pragma once
#include <string>
#include <cstdint>

namespace GB {
	class GitCommit
	{	
	public:
		void set_hash(const std::string &hash) { hash_ = hash; }
		void set_author(const std::string &author);
		void set_date(const std::string &date_str);
		void set_message(const std::string &message) { message_ = message; }
		void set_merge(const std::string &merge) { merge_ = merge; }
		void set_commit_number(const int32_t &num) { commit_number_ = num; }
	private:
		std::string author_{};
		std::string email_{};
		std::string message_{};
		std::string hash_{};
		std::string date_str_{};
		std::string merge_{};
		int32_t commit_number_{};
	};
}

