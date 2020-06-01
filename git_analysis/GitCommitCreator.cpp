#include "GitCommitCreator.h"

using namespace GB;

GitCommitCreator::GitCommitCreator(GitLogNode *terminal_node)
	: nodes_ok_{ parse_nodes(terminal_node)}
{
}

bool GitCommitCreator::parse_nodes(GitLogNode* end_node_ptr) {
	if (!end_node_ptr) {
		return false;
	}

	GitLogNode* current_node_ptr{ end_node_ptr };
	GitLogNode* start_node_ptr{};
	GitLogNode* start_commit_node_ptr{};

	uint32_t commit_count{};
	uint32_t node_count{1};
	while (current_node_ptr) {
		if (current_node_ptr->get_type() == GitLogNode::NodeType::commit) {
			commit_count++;
			start_commit_node_ptr = current_node_ptr;
		}
		node_count++;
		start_node_ptr = current_node_ptr;
		current_node_ptr = current_node_ptr->get_prev_node();
	}

	commits_.reserve(commit_count);
	current_node_ptr = start_commit_node_ptr;

	if (!current_node_ptr)
	{
		return false;
	}
	GitLogNode::NodeType current_type{ GitLogNode::NodeType::none };
	GitLogNode::NodeType node_type{ };
	std::string text{};
	int32_t commit_number{ static_cast<int32_t>(commit_count)};
	std::string whitespace{};

	for (uint32_t c{}; c < commit_count; ++c) {		
		GitCommit commit{};
		commit.set_commit_number(--commit_number);

		while (current_node_ptr) {
			node_type = current_node_ptr->get_type();

			switch (node_type) {
			case GitLogNode::NodeType::commit:
			case GitLogNode::NodeType::merge:
			case GitLogNode::NodeType::author:
			case GitLogNode::NodeType::date:
				if (current_type == GitLogNode::NodeType::commit) {
					commit.set_hash(text);
				}
				else if (current_type == GitLogNode::NodeType::merge) {
					commit.set_merge(text);
				}
				else if (current_type == GitLogNode::NodeType::author) {
					commit.set_author(text);
				}
				else if (current_type == GitLogNode::NodeType::date) {
					commit.set_date(text);
				}

				text.clear();
				whitespace.clear();
				current_type = node_type;
				break;

			case GitLogNode::NodeType::colon:
				text.append(":");
				whitespace.clear();
				break;

			case GitLogNode::NodeType::text:
				text.append(whitespace);
				text.append(current_node_ptr->get_text());
				whitespace = " ";
				break;
			}

			GitLogNode* retire_node{ current_node_ptr };
			current_node_ptr = current_node_ptr->get_next_node();
			delete retire_node;

			if (current_node_ptr &&
				current_node_ptr->get_type() == GitLogNode::NodeType::commit)
			{
				if (current_type == GitLogNode::NodeType::date) {
					commit.set_date(text);
				}

				text.clear();
				whitespace.clear();
				current_type = GitLogNode::NodeType::none;
				break;
			}
		}

		commits_.emplace_back(commit);
	}
	return true;
}
