#include "GitCommitCreator.h"
#include "GraphNodeIO.h"
#include <iostream>
#include <fstream>

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

		// Fix the last commit
		if (current_node_ptr == nullptr) {
			commit.set_date(text);
		}
		commits_.emplace_front(commit);
	}

	std::cout << "\nFound " << commit_count << " commits." << std::endl;
	return true;
}

bool GitCommitCreator::process(PlatformOS &os) {
	if (!generate_commit_graph(os)) {
		return false;
	}

	return true;
}

bool GitCommitCreator::generate_commit_graph(PlatformOS &os)
{
	std::string batch_filename{ os.get_random_string(20) };
	batch_filename.append(os.get_shell_extension());
	std::string git_out_filename{ os.get_random_string(20) };
	std::string git_diff_filename{ git_out_filename };
	git_out_filename.append(".gof");
	git_diff_filename.append(".diff");
	std::string prev_hash_str{};
	CommitGraph *prev_commit_graph_ptr{};
	std::deque<CommitGraph*> graph_queue{};
	GraphNodeIO graph_io{};
	graph_io.open("GitCommitGraph.gb",static_cast<uint32_t>(commits_.size()),"master");

	for (auto &commit : commits_) {
		const std::string hash_str{ commit.get_hash() };

		if (prev_hash_str.length()) {
			std::ofstream batch_file{ batch_filename.c_str() };			
			batch_file << "git.exe diff --name-only " 
				<< prev_hash_str 
				<< " "
				<< hash_str << " >"
				<< git_diff_filename << std::endl;
			batch_file.close();
			os.run_application("", batch_filename);
		}

		{
			std::ofstream batch_file{ batch_filename.c_str() };			
			std::cout << "git checkout " << hash_str << std::endl;
			batch_file << "git.exe checkout " << hash_str << std::endl;
			batch_file << "git.exe ls-files >" << git_out_filename << std::endl;
			batch_file.close();
			os.run_application("", batch_filename);
		}

		commit.set_reference_graph(prev_commit_graph_ptr);
		commit.generate_graph(git_out_filename, git_diff_filename);
		prev_commit_graph_ptr = commit.get_reference_graph();
		graph_queue.push_back(prev_commit_graph_ptr);
		prev_hash_str = hash_str;

		if (graph_queue.size() >= 3) {
			graph_io.write(*graph_queue.front());
			graph_queue.pop_front();
		}
	}

	graph_io.close();
	std::ofstream batch_file{ batch_filename.c_str() };
	std::cout << "git checkout master" << std::endl;
	batch_file << "git.exe checkout master" << std::endl;	
	batch_file.close();
	os.run_application("", batch_filename);
	os.delete_file(batch_filename);
	os.delete_file(git_diff_filename);
	os.delete_file(git_out_filename);
	return true;
}
