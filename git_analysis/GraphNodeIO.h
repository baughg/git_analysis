#pragma once
#include "CommitGraph.h"
#include <cstdlib>
#include <fstream>
#include <string>

namespace GB {
	typedef struct {
		uint32_t magic_marker{ 0xBEAD };
		uint32_t commits{};
		char branch[256];
	}graph_stream_header;


	class GraphNodeIO
	{
	public:		
		bool open(const std::string &filename, 
			const uint32_t &commits, 
			const std::string &branch);
		bool write(CommitGraph &graph);
		void close();
	private:
		std::ofstream graph_stream_{};
	};
}

