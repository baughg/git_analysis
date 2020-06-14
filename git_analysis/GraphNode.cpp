#include "GraphNode.h"

using namespace GB;

std::map<std::string, GraphNode::SourceCodeType> GraphNode::source_lut_{
	{"cpp",GraphNode::SourceCodeType::cpp},
	{"hpp",GraphNode::SourceCodeType::hpp},
	{"asm",GraphNode::SourceCodeType::assembly},
	{"c",GraphNode::SourceCodeType::c},
	{"cc",GraphNode::SourceCodeType::cc},
	{"h",GraphNode::SourceCodeType::h},
	{"hh",GraphNode::SourceCodeType::hh},
	{"v",GraphNode::SourceCodeType::v},
	{"sv",GraphNode::SourceCodeType::sv},
	{"py",GraphNode::SourceCodeType::py},
	{"inc",GraphNode::SourceCodeType::inc},
	{"Makefile",GraphNode::SourceCodeType::makefile},
	{"sh",GraphNode::SourceCodeType::sh},
	{"bat",GraphNode::SourceCodeType::bat},
	{"csh",GraphNode::SourceCodeType::csh}
};

GraphNode::GraphNode(const std::string &name,const std::string &short_name)
	: name_{ name },
	short_name_{ short_name }
{
}

void GraphNode::set_line_and_char_count(
	uint64_t &line_count, uint64_t &char_count)
{
	line_count_ = line_count;
	character_count_ = char_count;
	read_file_ = true;
}

void GraphNode::get_line_and_char_count(
	uint64_t &line_count, uint64_t &char_count)
{
	line_count += line_count_;
	char_count += character_count_;
}

bool GraphNode::process() {
	const bool has_children{ children_.size() > 0 };
	terminal_node_ |= children_.size() == 1 && parent_ != nullptr;

	if (has_children) {
		uint64_t line_count{};
		uint64_t char_count{};

		for (auto &child : children_) {
			child->get_line_and_char_count(line_count, char_count);
		}

		if (line_count_ == line_count &&
			character_count_ == char_count) {
			return false;
		}

		line_count_ = line_count;
		character_count_ = char_count;
	}
	else if(!read_file_){
		terminal_node_ = true;		
		IO::read_lines(name_, line_count_, character_count_);
		read_file_ = true;
	}
	else {
		return false;
	}

	return true;
}