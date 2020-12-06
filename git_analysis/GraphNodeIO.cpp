#include "GraphNodeIO.h"
#include <iostream>
#include <cassert>
#include <sstream>

using namespace GB;

bool GraphNodeIO::open(
	const std::string &filename,
	const uint32_t &commits,
	const std::string &branch)
{
	filename_ = filename;
	branch_ = branch;	
	const bool graph_loaded{ load_graphs(commits) };

	graph_stream_.open(filename,
		std::ios_base::binary | std::ios::out | std::ios_base::in);

	assert(graph_stream_.is_open());
	const auto end_pos{ graph_stream_.tellp() };
	graph_stream_.seekp(0, std::ios::beg);
	graph_stream_header header{};

	header.commits = commits;
	std::string branch_name{ branch };

	if (branch_name.length() >= sizeof(header.branch)) {
		branch_name = branch_name.substr(sizeof(header.branch) - 1);
	}

	strcpy_s(header.branch, branch_name.c_str());
	graph_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));

	if (graph_loaded) {
		graph_stream_.seekp(0, std::ios::end);
		uint32_t mark{ 0xbeaadcff };
		graph_stream_.write(reinterpret_cast<const char*>(&mark), sizeof(mark));
		graph_stream_.flush();
	}
	return true;
}

std::string GraphNodeIO::human_friendly_file_size(const uint64_t &sz) {
	std::stringstream str{};
	uint64_t sz_uint{ sz };

	if (sz >= (1ULL << 30)) {
		sz_uint >>= 30;
		str << sz_uint << "GB";
	}
	else if (sz >= (1ULL << 20)) {
		sz_uint >>= 20;
		str << sz_uint << "MB";
	}
	else if (sz >= (1ULL << 10)) {
		sz_uint >>= 10;
		str << sz_uint << "KB";
	}
	else {
		str << sz_uint << "B";
	}
	
	return str.str();
}

bool GraphNodeIO::load_graphs(const uint32_t &commits)
{		
	graph_stream_.open(filename_,
		std::ios_base::binary | std::ios::in);

	if (!graph_stream_.is_open())
		return false;

	graph_stream_header header{};	
	graph_stream_.read(reinterpret_cast<char*>(&header), sizeof(header));

	if (!header.commits)
	{
		graph_stream_.close();
		return false;
	}

	int32_t commits_in_header{ static_cast<int32_t>(header.commits) };
	graph_write_header write_header{};

	graph_stream_.read(reinterpret_cast<char*>(&write_header), sizeof(write_header));
	std::vector<char> commit_hash(write_header.hash_len + 1);
	graph_stream_.read(commit_hash.data(), write_header.hash_len);
	std::string commit_str{ std::string(commit_hash.data()) };
	graph_offset_lut_[commit_str] = sizeof(header);
	graph_commit_io_.push_back(commit_io{ commit_str,sizeof(header) });
	commits_in_header--;
	uint64_t total_offset{ 
		sizeof(header) + sizeof(write_header) + 
		write_header.offset_to_next_graph  + static_cast<uint64_t>(write_header.hash_len)};

	graph_stream_.seekp(write_header.offset_to_next_graph, std::ios::cur);

	while (commits_in_header) {		
		graph_stream_.read(reinterpret_cast<char*>(&write_header), sizeof(write_header));
		graph_stream_.read(commit_hash.data(), write_header.hash_len);
		std::string commit_str{ std::string(commit_hash.data()) };
		graph_offset_lut_[commit_str] = total_offset;
		graph_commit_io_.push_back(commit_io{ commit_str,total_offset });
		total_offset += sizeof(write_header);
		total_offset += static_cast<uint64_t>(write_header.hash_len);
		total_offset += write_header.offset_to_next_graph;
		graph_stream_.seekp(write_header.offset_to_next_graph, std::ios::cur);
		commits_in_header--;
	}

	graph_stream_.close();
	return true;
}

bool GraphNodeIO::write(CommitGraph &graph) {
	std::map<std::string, uint64_t> node_id_lut{};
	const uint32_t commit_no{ graph.get_commit_number() };
	graph_write_header header{
		0,
		static_cast<uint32_t>(graph.node_lut_.size()),
	static_cast<uint32_t>(graph.commit_hash_.length()),
	graph.file_count_,
	graph.source_code_file_count_ };

	const auto header_pos{ graph_stream_.tellp() };
	graph_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
	graph_stream_.write(graph.commit_hash_.c_str(), graph.commit_hash_.length());

	size_t bytes_written{};
	uint32_t row{};
	for (auto &it : graph.node_lut_) {
		const uint64_t node_id{ it.second->node_id_ };
		const std::string node_name{ it.second->name_ };
		const std::string short_name{ it.second->short_name_ };
		auto nit{ global_node_id_lut_.find(node_name) };
		const bool have_name_reference{ nit != global_node_id_lut_.end() };
		
		if (!have_name_reference) {
			global_node_id_lut_[node_name] = graph_name_ref_str{ node_id, commit_no };
		}
		
		node_id_lut[node_name] = node_id;

		/*graph_node_table table_entry{ node_id,
			row++,
			commit_no,
			static_cast<uint16_t>(node_name.length()),
			static_cast<uint16_t>(short_name.length())			
		};*/

		graph_node_table table_entry{};

		bytes_written += sizeof(table_entry);

		if (have_name_reference) {
			const graph_name_ref_str node_ref{ global_node_id_lut_[node_name] };
			table_entry.node_id.node_id_lnk.soft_link = 1;
			table_entry.node_id.node_id_lnk.node_id = node_id;
			table_entry.reference.graph_name_ref.graph_number = node_ref.graph_number;
			table_entry.reference.graph_name_ref.reference_node = node_ref.node_id;
			graph_stream_.write(reinterpret_cast<const char*>(&table_entry), sizeof(table_entry));
		}
		else {	
			table_entry.node_id.node_id = node_id;
			table_entry.reference.name_length.row = row++;
			table_entry.reference.name_length.name_len = static_cast<uint16_t>(node_name.length());
			table_entry.reference.name_length.short_name_len = static_cast<uint16_t>(short_name.length());
			table_entry.reference.name_length.graph_number = commit_no;

			bytes_written += table_entry.reference.name_length.name_len;
			bytes_written += table_entry.reference.name_length.short_name_len;

			graph_stream_.write(reinterpret_cast<const char*>(&table_entry), sizeof(table_entry));
			graph_stream_.write(node_name.c_str(), node_name.length());
			graph_stream_.write(short_name.c_str(), short_name.length());
		}
	}

	graph_node_entry entry{};
	std::vector<uint64_t> child_nodes{};

	for (auto &it : graph.node_lut_) {
		it.second->get_serialised_entry(entry, child_nodes);
		graph_stream_.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
		bytes_written += sizeof(entry);

		if (entry.children) {
			graph_stream_.write(reinterpret_cast<const char*>(&child_nodes[0]),
				sizeof(uint64_t)*entry.children);

			bytes_written += (sizeof(uint64_t) * entry.children);
		}
	}

	file_size_ += bytes_written;
	std::cout << "Write " << graph.commit_hash_ << " graph size: " 
		<< human_friendly_file_size(bytes_written) << " file size: "
		<< human_friendly_file_size(file_size_)
		<< std::endl;

	const auto end_pos{ graph_stream_.tellp() };
	graph_stream_.seekp(header_pos, std::ios::beg);	
	header.offset_to_next_graph = static_cast<uint64_t>(bytes_written);
	graph_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
	graph_stream_.seekp(end_pos, std::ios::beg);
	graph_stream_.flush();

	for (auto &it : graph.node_lut_) {
		it.second->clear();
		graph.node_lut_[it.first] = nullptr;
	}

	graph.node_lut_.clear();
	graph.top_node_ = nullptr;
	return true;
}

void GraphNodeIO::close() {
	graph_stream_.close();
}
