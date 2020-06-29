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
		uint64_t offset_to_next_graph{};
		uint32_t node_count{};
		uint32_t hash_len{};
		uint32_t file_count{};
		int32_t source_file_count{};
	}graph_write_header;

	typedef struct {
		uint64_t node_id{};
		uint32_t name_len : 16;
		uint32_t short_name_len : 16;
		uint32_t row{};
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
		std::string human_friendly_file_size(const uint64_t &sz);
		std::ofstream graph_stream_{};
		uint64_t file_size_{};
	};
}

