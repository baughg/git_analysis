#include "StringUtility.h"
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>

using namespace GB;


size_t StringUtility::split_string(
	const std::string &str, std::deque<std::string>& tokenVector, char token)
{	
	tokenVector.clear();
	auto lastPosition{ str.find_first_not_of(token, 0) };	
	auto position{ str.find_first_of(token, lastPosition) };

	while (std::string::npos != position || std::string::npos != lastPosition)
	{		
		tokenVector.push_back(str.substr(lastPosition, position - lastPosition));		
		lastPosition = str.find_first_not_of(token, position);	
		position = str.find_first_of(token, lastPosition);
	}

	return tokenVector.size();
}

void StringUtility::trim(std::string &s) {	
	s.erase(std::remove(std::begin(s), std::end(s), ' '), std::end(s));	
}