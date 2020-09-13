#pragma once
#include <ship/io/popen_file.hpp>
#include <ship/io/all_exist.hpp>
#include <ship/pair.hpp>

#include <conduit/generator.hpp>

#include <filesystem>

#include <string_view>
#include <unordered_map>
#include <vector>


namespace ship {
namespace fs = std::filesystem;
}
namespace ship::io {

auto normalize(fs::path const& p) -> fs::path;

auto read_file(FILE* file) -> conduit::generator<std::string_view>;
auto read_lines(FILE* file) -> conduit::generator<std::string_view>;
} // namespace ship::io

namespace ship::elf {
auto dependency_graph(std::vector<fs::path> files)
    -> std::unordered_map<std::string,
                          std::vector<pair<std::string, fs::path>>>;

auto libraries(fs::path path)
    -> conduit::generator<pair<std::string_view, fs::path>>;

auto is_elf(fs::path const& path) -> bool;
auto enumerate(fs::path const& paths) -> conduit::generator<fs::path>;
auto enumerate(char** paths) -> conduit::generator<fs::path>;
} // namespace ship::elf
