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
	top_node_->is_root();
	node_lut_[root_node_name] = top_node_;

	for (auto &file : files) {
		std::deque<std::string> nodes{};
		const size_t node_count{ StringUtility::split_string(file, nodes, '/') };

		std::string node_name{ nodes.front() };
		nodes.pop_front();
		auto it{ node_lut_.find(node_name) };
		std::shared_ptr<GraphNode> node{};

		if (it == std::end(node_lut_)) {
			node = std::make_shared<GraphNode>(node_name);
			node_lut_[node_name] = node;
		}
		else {
			node = node_lut_[node_name];
		}

		top_node_->add_child(node);
		node->add_parent(top_node_);

		for (size_t n{ 1 }; n < node_count; ++n) {
			node_name.append("/");
			node_name.append(nodes.front());
			nodes.pop_front();

			std::shared_ptr<GraphNode> child_node{};

			auto it{ node_lut_.find(node_name) };

			if (it == std::end(node_lut_)) {
				child_node = std::make_shared<GraphNode>(node_name);
				node_lut_[node_name] = child_node;
			}
			else {
				child_node = node_lut_[node_name];
			}
						
			node->add_child(child_node);
			child_node->add_parent(node);
			node = child_node;
		}
	}

	for (;;) {
		bool do_process{ false };

		for (auto &it : node_lut_) {
			do_process |= it.second->process();
		}

		if (!do_process) {
			break;
		}
	}
	return true;
}