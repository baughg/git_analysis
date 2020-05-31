#pragma once
#include "GitCommit.h"
#include "GitLogNode.h"
#include <vector>

namespace GB {
	class GitCommitCreator
	{
	public:
		GitCommitCreator() = delete;
		GitCommitCreator(GitLogNode *terminal_node);
	private:
		bool parse_nodes(GitLogNode* end_node_ptr);
		std::vector<GitCommit> commits_{};		
		bool nodes_ok_{false};
	};
}

