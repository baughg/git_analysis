#pragma once
#include <string>
#include <deque>
#include <cstdint>
#include <memory>
#include <map>
#include "IO.h"

namespace GB {
	class GraphNode
	{
	public:
		GraphNode(const std::string &name, const std::string &short_name);
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

		void set_next(std::shared_ptr<GraphNode> node) {
			next_ = node;
		}

		void set_previous(std::shared_ptr<GraphNode> node) {
			previous_ = node;
		}

		void is_root() { is_root_ = true; }
		void get_line_and_char_count(uint64_t &line_count, uint64_t &char_count);
		void set_line_and_char_count(uint64_t &line_count, uint64_t &char_count);
		bool process();
	private:
		std::shared_ptr<GraphNode> next_{};
		std::shared_ptr<GraphNode> previous_{};
		std::shared_ptr<GraphNode> parent_{};
		std::deque< std::shared_ptr<GraphNode>> children_{};
		std::map<std::shared_ptr<GraphNode>, uint32_t> child_map_{};
		std::string name_{};
		std::string short_name_{};
		bool is_root_{ false };
		uint64_t line_count_{};
		uint64_t character_count_{};
		bool read_file_{ false };
		bool terminal_node_{ false };
	};
}

