#include "IO.h"
#include <fstream>

using namespace GB;

bool IO::read_text(
	const std::string &filename, std::deque<std::string> &lines)
{
	lines.clear();
	std::ifstream text_file(filename.c_str());
		
	if (!text_file.good()) {
		return false;
	}
	
	while (!text_file.eof()) {
		std::string line_in{};
		getline(text_file, line_in);
		lines.emplace_back(std::move(line_in));
	}

	return lines.size() > 0;
}


