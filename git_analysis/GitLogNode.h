#pragma once
#include <string>

namespace GB {
	class GitLogNode
	{	
	public:
		enum class NodeType { 
			none,commit,text,number,author,
			date,merge,colon,merge_pull
		};

		GitLogNode() = default;
		GitLogNode(const char *text_str);
		GitLogNode(NodeType type);
		void set_next_node(GitLogNode* node_ptr);
		void set_number(const int &num);
		void set_text(const char *text_str);
		GitLogNode* get_prev_node() { return prev_ptr_; }
		GitLogNode* get_next_node() { return next_ptr_; }
		NodeType get_type() { return type_; }
		std::string get_text() { return std::move(node_text_); }
	private:
		std::string node_text_{};
		int node_num_{ };
		NodeType type_{ NodeType::none };
		GitLogNode* next_ptr_{};
		GitLogNode* prev_ptr_{};
	};
}

