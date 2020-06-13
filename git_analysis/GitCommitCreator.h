#pragma once
#include "GitCommit.h"
#include "GitLogNode.h"
#include "../platform_os/platform_os/PlatformOS.h"
#include <deque>

namespace GB {
	class GitCommitCreator
	{
	public:
		GitCommitCreator() = delete;
		GitCommitCreator(GitLogNode *terminal_node);
		bool process(PlatformOS &os);
	private:
		bool parse_nodes(GitLogNode* end_node_ptr);
		std::deque<GitCommit> commits_{};		
		bool nodes_ok_{false};
	};
}

