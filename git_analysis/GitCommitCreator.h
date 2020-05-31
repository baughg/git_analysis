#pragma once
#include "GitCommit.h"
#include "GitLogNode.h"

namespace GB {
	class GitCommitCreator
	{
	public:
		GitCommitCreator() = delete;
		GitCommitCreator(GitLogNode *terminal_node);
	private:
		GitLogNode* terminal_node_{};
	};
}

