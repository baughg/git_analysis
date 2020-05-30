#pragma once
#include <string>

namespace GB {
	class GitLogNode
	{	
		enum class NodeType { none,commit,text,number,author,email,date,time,time_zone };
	public:
		GitLogNode() = default;
		GitLogNode(const char *text_str);
	private:
		std::string node_text_{};
		int node_num_{ };
		NodeType type_{ NodeType::none };
	};
}

