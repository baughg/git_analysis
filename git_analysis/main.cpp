#include "platform_os/WindowsPlatform.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool read_text(
	const std::string &filename, std::string &content);

//int main() {
//	std::string git_log_filename{ "C:\\development\\nce\\vscxggbwkfnqduxwfnfo" };
//	std::string content{};
//
//	read_text(git_log_filename, content);
//	return 0;
//}

bool read_text(
	const std::string &filename, std::string &content)
{
	std::ifstream text_file(filename.c_str());	

	if (!text_file.good())
		return false;

	std::string line_in{};
	
	while (!text_file.eof()) {
		getline(text_file, line_in);
		content.append(line_in);
		content.append("\n");
	}

	return true;
}