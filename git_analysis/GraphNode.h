#pragma once
#include <string>
#include <deque>
#include <memory>

namespace GB {
	class GraphNode
	{
	public:
		GraphNode(const std::string &name);
		void add_child(std::shared_ptr<GraphNode> node) {
			children_.push_back(node);
		}

		void add_parent(std::shared_ptr<GraphNode> node) {
			parent_ = node;
		}
	private:
		std::shared_ptr<GraphNode> parent_{};
		std::deque< std::shared_ptr<GraphNode>> children_{};
		std::string name_{};
	};
}

