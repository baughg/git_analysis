#pragma once
#include <string>
#include <deque>
#include <cstdint>
#include <memory>
#include <map>

namespace GB {
	class GraphNode
	{
	public:
		GraphNode(const std::string &name);
		void add_child(std::shared_ptr<GraphNode> node) {
			auto it{ child_map_.find(node) };

			if (it != std::end(child_map_)) {
				return;
			}

			child_map_[node]++;
			children_.push_back(node);
		}

		void add_parent(std::shared_ptr<GraphNode> node) {
			parent_ = node;
		}

		void is_root() { is_root_ = true; }
	private:
		std::shared_ptr<GraphNode> parent_{};
		std::deque< std::shared_ptr<GraphNode>> children_{};
		std::map<std::shared_ptr<GraphNode>, uint32_t> child_map_{};
		std::string name_{};
		bool is_root_{ false };
	};
}

