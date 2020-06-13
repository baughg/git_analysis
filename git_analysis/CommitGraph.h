#pragma once
#include "IO.h"
#include "GraphNode.h"
#include <string>
#include <map>

namespace GB {
	class CommitGraph
	{
	public:
		bool build(const std::string &filelist);
	private:
		std::shared_ptr<GraphNode> top_node_{};
		std::map<std::string, std::shared_ptr<GraphNode>> node_lut_{};
	};
}

