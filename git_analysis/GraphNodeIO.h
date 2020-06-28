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

	typedef struct {
		uint32_t node_count{};
		uint64_t offset_to_next_graph{};
	}graph_write_header;

	typedef struct {
		uint64_t node_id{};
		uint16_t name_len{};
		uint16_t short_name_len{};
	}graph_node_table;
	
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

