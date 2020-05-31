#pragma once
#include <string>

namespace GB {
	class GitLogNode
	{	
	public:
		enum class NodeType { 
			none,commit,text,number,author,
			email,date,time,time_zone,merge,
			colon
		};

		GitLogNode() = default;
		GitLogNode(const char *text_str);
		GitLogNode(NodeType type);
		void set_next_node(GitLogNode* node_ptr);
		void set_number(const int &num);
		void set_text(const char *text_str);
	private:
		std::string node_text_{};
		int node_num_{ };
		NodeType type_{ NodeType::none };
		GitLogNode* next_ptr_{};
		GitLogNode* prev_ptr_{};
	};
}

