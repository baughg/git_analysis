#include "GraphNode.h"

using namespace GB;

GraphNode::GraphNode(const std::string &name)
	: name_{ name }
{
}

void GraphNode::get_line_and_char_count(
	uint32_t &line_count, uint32_t &char_count)
{
	line_count += line_count_;
	char_count += character_count_;
}

bool GraphNode::process() {
	const bool has_children{ children_.size() > 0 };

	if (has_children) {
		uint32_t line_count{};
		uint32_t char_count{};

		for (auto &child : children_) {
			child->get_line_and_char_count(line_count, char_count);
		}

		if (line_count_ == line_count &&
			character_count_ == char_count) {
			return false;
		}
	}
	else if(!read_file_){
		IO::read_lines(name_, line_count_, character_count_);
		read_file_ = true;
	}
	else {
		return false;
	}

	return true;
}