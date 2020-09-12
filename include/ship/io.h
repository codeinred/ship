#pragma once
#include <ship/pair.hpp>
#include <ship/io/popen_file.hpp>

#include <conduit/generator.hpp>

#include <filesystem>

#include <string_view>
#include <unordered_map>
#include <vector>

namespace ship {

namespace fs = std::filesystem;
}
namespace ship::io {
fs::path normalize(fs::path const& p);

conduit::generator<std::string_view> read_file(FILE* file);
conduit::generator<std::string_view> read_lines(FILE* file);

conduit::generator<pair<std::string_view, fs::path>>
list_libraries(fs::path path);

auto get_dependency_graph(std::vector<fs::path> files)
    -> std::unordered_map<std::string,
                          std::vector<pair<std::string, fs::path>>>;
} // namespace ship::io
