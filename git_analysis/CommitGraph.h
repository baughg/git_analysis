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
	private:
		std::shared_ptr<GraphNode> top_node_{};
		std::map<std::string, std::shared_ptr<GraphNode>> node_lut_{};
		CommitGraph* reference_graph_ptr_{};
		std::string commit_hash_{};
	};
}

