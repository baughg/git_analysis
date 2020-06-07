#include "GitCommit.h"
#include "StringUtility.h"

using namespace GB;

void GitCommit::set_date(const std::string &date_str) {
	auto position{ date_str.find_first_of('+') };
	position += 5;

	date_str_ = date_str.substr(0, position);
	date_time_.date_time_from_string(date_str_);
	const auto msg_len{ date_str.length() - position };
	message_ = date_str.substr(position + 1, msg_len);
}

void GitCommit::set_author(const std::string &author) { 
	std::deque<std::string> str_comp{};
	StringUtility::split_string(author, str_comp, '<');
	author_ = str_comp.front();

	StringUtility::trim(author_);

	email_ = str_comp.back();
	email_ = email_.substr(0, email_.length() - 1);
	StringUtility::trim(email_);
}