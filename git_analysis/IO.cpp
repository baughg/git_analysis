#include "IO.h"
#include <fstream>

using namespace GB;

bool IO::read_lines(
	const std::string &filename,
	uint32_t &line_count,
	uint32_t &char_count) {
	std::ifstream text_file(filename.c_str());
	line_count = 0;
	char_count = 0;

	if (!text_file.good()) {
		return false;
	}

	while (!text_file.eof()) {
		std::string line_in{};
		getline(text_file, line_in);
		line_count++;
		char_count += static_cast<uint32_t>(line_in.length());
	}

	return line_count > 0;
}

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


