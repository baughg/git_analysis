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
		enum class SourceCodeType {
			none, cpp, cc, c, h, hpp, hh,
			assembly, v, sv, makefile, inc, py, sh, csh, bat,
			readme, txt, matlab, proto, rgi
		};
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
			line_count_deltan_ = static_cast<int64_t>(line_count_)
				- static_cast<int64_t>(node->line_count_);

			character_count_deltan_ = static_cast<int64_t>(character_count_)
				- static_cast<int64_t>(node->character_count_);
		}

		void set_previous(std::shared_ptr<GraphNode> node) {
			line_count_deltap_ = static_cast<int64_t>(line_count_) 
				- static_cast<int64_t>(node->line_count_);

			character_count_deltap_ = static_cast<int64_t>(character_count_) 
				- static_cast<int64_t>(node->character_count_);			
		}

		void is_root() { is_root_ = true; }
		void get_line_and_char_count(uint64_t &line_count, uint64_t &char_count);
		void set_line_and_char_count(uint64_t &line_count, uint64_t &char_count);
		bool process();
		bool is_source_code() { return source_type_ != SourceCodeType::none; }
		bool is_terminal_node() { return terminal_node_; }
		void set_terminal_node(const bool &terminal) { terminal_node_ = terminal; }
		SourceCodeType get_source_type() { return source_type_; }
		void set_source_type(const SourceCodeType &type) { source_type_ = type; }
	private:		
		std::shared_ptr<GraphNode> parent_{};
		std::deque< std::shared_ptr<GraphNode>> children_{};
		std::map<std::shared_ptr<GraphNode>, uint32_t> child_map_{};
		std::string name_{};
		std::string short_name_{};
		bool is_root_{ false };
		uint64_t line_count_{};
		uint64_t character_count_{};
		int64_t line_count_deltap_{};
		int64_t character_count_deltap_{};
		int64_t line_count_deltan_{};
		int64_t character_count_deltan_{};
		bool read_file_{ false };
		bool terminal_node_{ false };
		SourceCodeType source_type_{ SourceCodeType::none };
		const static std::map<std::string, SourceCodeType> source_lut_;
		uint64_t node_id_{};
		static uint64_t global_node_id_;	
		friend class GraphNodeIO;
	};
}

