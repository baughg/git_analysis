#include "GitCommit.h"
#include "StringUtility.h"

using namespace GB;

void GitCommit::set_author(const std::string &author) { 
	std::deque<std::string> str_comp{};
	StringUtility::split_string(author, str_comp, '<');
	author_ = str_comp.front();

	StringUtility::trim(author_);

	email_ = str_comp.back();
	email_ = email_.substr(0, email_.length() - 1);
	StringUtility::trim(email_);
}