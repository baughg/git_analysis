#pragma once
#include "IO.h"
#include "GraphNode.h"
#include <string>
#include <map>

namespace GB {
	class CommitGraph
	{
	public:
		bool build(const std::string &filelist, const std::string &filediff);
		void set_reference_graph(CommitGraph *reference_graph) {
			reference_graph_ptr_ = reference_graph;
		}
		void set_hash(const std::string &hash) {
			commit_hash_ = hash;
		}
		void set_commit_number(const uint32_t &num) {
			commit_number_ = num;
		}
		uint32_t get_commit_number() {
			return commit_number_;
		}
	private:
		std::shared_ptr<GraphNode> top_node_{};
		std::map<std::string, std::shared_ptr<GraphNode>> node_lut_{};
		CommitGraph* reference_graph_ptr_{};
		std::string commit_hash_{};
		uint32_t file_count_{};
		int32_t source_code_file_count_{};
		uint32_t commit_number_{};
		friend class GraphNodeIO;
	};
}

