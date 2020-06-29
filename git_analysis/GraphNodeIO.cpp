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
	graph_stream_.open(filename, std::ios_base::binary);

	assert(graph_stream_.is_open());

	graph_stream_header header{};
	header.commits = commits;
	std::string branch_name{ branch };

	if (branch_name.length() >= sizeof(header.branch)) {
		branch_name = branch_name.substr(sizeof(header.branch) - 1);
	}

	strcpy_s(header.branch, branch_name.c_str());
	graph_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
	return true;
}

std::string GraphNodeIO::human_friendly_file_size(const uint64_t &sz) {
	std::stringstream str{};
	uint64_t sz_uint{ sz };

	if (sz >= (1ULL << 30)) {
		sz_uint >= 30;
		str << sz_uint << "GB";
	}
	else if (sz >= (1ULL << 20)) {
		sz_uint >= 20;
		str << sz_uint << "MB";
	}
	else if (sz >= (1ULL << 10)) {
		sz_uint >= 10;
		str << sz_uint << "KB";
	}
	else {
		str << sz_uint << "B";
	}
	
	return str.str();
}

bool GraphNodeIO::write(CommitGraph &graph) {
	std::map<std::string, uint64_t> node_id_lut{};

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

		node_id_lut[node_name] = node_id;

		graph_node_table table_entry{ node_id,
			static_cast<uint16_t>(node_name.length()),
			static_cast<uint16_t>(short_name.length()),
			row++
		};

		bytes_written += sizeof(table_entry);
		bytes_written += table_entry.name_len;
		bytes_written += table_entry.name_len;

		graph_stream_.write(reinterpret_cast<const char*>(&table_entry), sizeof(table_entry));
		graph_stream_.write(node_name.c_str(), node_name.length());
		graph_stream_.write(short_name.c_str(), short_name.length());
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
	graph_stream_.seekp(header_pos);
	header.offset_to_next_graph = static_cast<uint64_t>(bytes_written);
	graph_stream_.write(reinterpret_cast<const char*>(&header), sizeof(header));
	graph_stream_.seekp(end_pos);
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
