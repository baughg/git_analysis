#include "GraphNode.h"
#include "StringUtility.h"
#include <iostream>

using namespace GB;

const std::map<std::string, GraphNode::SourceCodeType> GraphNode::source_lut_{
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
	{"csh",GraphNode::SourceCodeType::csh},
	{"md",GraphNode::SourceCodeType::readme},
	{"txt",GraphNode::SourceCodeType::txt},
	{"m",GraphNode::SourceCodeType::matlab},
	{"proto",GraphNode::SourceCodeType::proto},
	{"rgi",GraphNode::SourceCodeType::rgi}
};

uint64_t GraphNode::global_node_id_ = 0;

GraphNode::GraphNode(const std::string &name,const std::string &short_name)
	: name_{ name },
	short_name_{ short_name },
	node_id_ { global_node_id_++ }
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

void GraphNode::clear() {
	children_.clear();
	parent_ = nullptr;
	child_map_.clear();
}

void GraphNode::get_serialised_entry(
	graph_node_entry &entry,
	std::vector<uint64_t> &child_nodes) {
	entry.children = static_cast<uint32_t>(children_.size());
	child_nodes.clear();
	child_nodes.reserve(entry.children);
	entry.parent = parent_ != nullptr ? parent_->node_id_ : ~0ULL;
	entry.line_count = line_count_;
	entry.character_count = character_count_;
	entry.line_count_deltan = line_count_deltan_;
	entry.line_count_deltap = line_count_deltap_;
	entry.character_count_deltan = character_count_deltan_;
	entry.character_count_deltap = character_count_deltap_;
	entry.type = static_cast<uint32_t>(source_type_);

	for (const auto &it : children_) {
		child_nodes.push_back(it->node_id_);
	}
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
		std::deque<std::string> filename_comp{};
		StringUtility::split_string(short_name_, filename_comp, '.');
		line_count_ = 0;
		character_count_ = 0;

		if (filename_comp.size()) {
			auto it{ source_lut_.find(filename_comp.back()) };

			if (it != std::end(source_lut_)) {
				source_type_ = it->second;
				IO::read_lines(name_, line_count_, character_count_);
			}
		}

		if (source_type_ == SourceCodeType::none) {
			std::cout << "Skip read: " << short_name_ << std::endl;
		}
		read_file_ = true;
	}
	else {
		return false;
	}

	return true;
}