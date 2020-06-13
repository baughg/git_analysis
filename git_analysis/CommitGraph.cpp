#include "CommitGraph.h"
#include "StringUtility.h"
#include <deque>

using namespace GB;

bool CommitGraph::build(const std::string &filelist) {
	std::deque<std::string> files{};

	if (!IO::read_text(filelist, files)) {
		return false;
	}
	const std::string  root_node_name{ "root" };

	top_node_ = std::make_shared<GraphNode>(root_node_name);
	node_lut_[root_node_name] = top_node_;

	for (auto &file : files) {
		std::deque<std::string> nodes{};
		const size_t node_count{ StringUtility::split_string(file, nodes, '/') };

		std::string node_name{ nodes.front() };
		nodes.pop_front();
		auto node = std::make_shared<GraphNode>(node_name);
		node_lut_[node_name] = node;
		top_node_->add_child(node);
		node->add_parent(top_node_);

		for (size_t n{ 1 }; n < node_count; ++n) {

		}
	}
	return true;
}