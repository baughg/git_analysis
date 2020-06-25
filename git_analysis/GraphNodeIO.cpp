#include "GraphNodeIO.h"
#include <cassert>

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

bool GraphNodeIO::write(CommitGraph &graph) {
	std::map<std::string, uint64_t> node_id_lut{};
	graph_write_header header{ static_cast<uint32_t>(graph.node_lut_.size()) };

	graph_stream_.write(reinterpret_cast<const char*>(&header),sizeof(header));

	for (auto &it : graph.node_lut_) {
		const uint64_t node_id{ it.second->node_id_ };
		const std::string node_name{ it.second->name_ };
		const std::string short_name{ it.second->short_name_ };

		node_id_lut[node_name] = node_id;

		graph_node_table table_entry{ node_id,
			static_cast<uint16_t>(node_name.length()),
			static_cast<uint16_t>(short_name.length())
		};

		graph_stream_.write(reinterpret_cast<const char*>(&table_entry), sizeof(table_entry));
		graph_stream_.write(node_name.c_str(), node_name.length());
		graph_stream_.write(short_name.c_str(), short_name.length());
	}

	graph_stream_.flush();
	return true;
}

void GraphNodeIO::close() {
	graph_stream_.close();
}
