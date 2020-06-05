#include "GitCommit.h"
#include "StringUtility.h"

using namespace GB;

void GitCommit::set_author(const std::string &author) { 
	std::deque<std::string> str_comp{};
	StringUtility::split_string(author, str_comp, '<');
	author_ = author; 
}