#pragma once
#include "CommitGraph.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <map>
#include <deque>

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
		uint64_t row : 32;
		uint64_t graph_number : 32;
		uint32_t name_len : 16;
		uint32_t short_name_len : 16;		
	}name_length_info;

	typedef struct {
		uint64_t reference_node;
		uint32_t graph_number;
	}graph_name_reference;

	typedef union {
		name_length_info name_length;
		graph_name_reference graph_name_ref;
	}node_reference;

	typedef struct {		
		uint64_t node_id : 63;
		uint64_t soft_link : 1;
	}node_id_link;

	typedef union {
		node_id_link node_id_lnk;
		uint64_t node_id;
	}node_id_entry;

	typedef struct {
		node_id_entry node_id;
		node_reference reference;		
	}graph_node_table;
	
	typedef struct {
		uint64_t node_id{};
		uint32_t graph_number{};
	}graph_name_ref_str;

	typedef struct {
		std::string hash;
		uint64_t offset{};
	}commit_io;

	class GraphNodeIO
	{
	public:		
		bool open(const std::string &filename, 
			const uint32_t &commits, 
			const std::string &branch);
		bool write(CommitGraph &graph);
		void close();
		bool commit_processed(const std::string &hash, bool &write_graph);
	private:
		std::string filename_;
		std::string branch_;
		bool load_graphs(const uint32_t &commits);
		std::string human_friendly_file_size(const uint64_t &sz);
		std::fstream graph_stream_{};
		uint64_t file_size_{};
		std::map<std::string, uint64_t> graph_offset_lut_{};
		std::map<std::string, graph_name_ref_str> global_node_id_lut_{};
		std::deque<commit_io> graph_commit_io_;
	};
}

