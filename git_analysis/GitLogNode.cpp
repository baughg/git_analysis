#include "GitLogNode.h"

using namespace GB;

GitLogNode::GitLogNode(const char *text_str) 
	: node_text_ { text_str }
{
}


void GitLogNode::set_next_node(GitLogNode* node_ptr) {
	next_ptr_ = node_ptr;
	node_ptr->prev_ptr_ = this;
}

void GitLogNode::set_number(const int &num) {
	node_num_ = num;
}

